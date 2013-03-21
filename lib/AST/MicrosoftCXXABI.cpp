//===------- MicrosoftCXXABI.cpp - AST support for the Microsoft C++ ABI --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This provides C++ AST support targeting the Microsoft Visual C++
// ABI.
//
//===----------------------------------------------------------------------===//

#include "CXXABI.h"
#include "clang/AST/Attr.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/Type.h"
#include "clang/Basic/TargetInfo.h"

using namespace clang;

namespace {
class MicrosoftCXXABI : public CXXABI {
  ASTContext &Context;
public:
  MicrosoftCXXABI(ASTContext &Ctx) : Context(Ctx) { }

  unsigned getMemberPointerSize(const MemberPointerType *MPT) const;

  CallingConv getDefaultMethodCallConv(bool isVariadic) const {
    if (!isVariadic && Context.getTargetInfo().getTriple().getArch() == llvm::Triple::x86)
      return CC_X86ThisCall;
    else
      return CC_C;
  }

  bool isNearlyEmpty(const CXXRecordDecl *RD) const {
    // FIXME: Audit the corners
    if (!RD->isDynamicClass())
      return false;

    const ASTRecordLayout &Layout = Context.getASTRecordLayout(RD);
    
    // In the Microsoft ABI, classes can have one or two vtable pointers.
    CharUnits PointerSize = 
      Context.toCharUnitsFromBits(Context.getTargetInfo().getPointerWidth(0));
    return Layout.getNonVirtualSize() == PointerSize ||
      Layout.getNonVirtualSize() == PointerSize * 2;
  }    
};
}

// getNumBases() seems to only give us the number of direct bases, and not the
// total.  This function tells us if we inherit from anybody that uses MI.
static bool recordHasMultipleInheritance(const CXXRecordDecl *RD) {
  while (RD->getNumBases() > 0) {
    if (RD->getNumBases() > 1)
      return true;
    assert(RD->getNumBases() == 1);
    RD = RD->bases_begin()->getType()->getAsCXXRecordDecl();
  }
  return false;
}

unsigned MicrosoftCXXABI::getMemberPointerSize(const MemberPointerType *MPT) const {
  QualType Pointee = MPT->getPointeeType();
  CXXRecordDecl *RD = MPT->getClass()->getAsCXXRecordDecl();
  attr::Kind Inheritance;

  if (RD->getTypeForDecl()->isIncompleteType()) {
    if (Attr *IA = RD->getAttr<SingleInheritanceAttr>())
      Inheritance = IA->getKind();
    else if (Attr *IA = RD->getAttr<MultipleInheritanceAttr>())
      Inheritance = IA->getKind();
    else if (Attr *IA = RD->getAttr<VirtualInheritanceAttr>())
      Inheritance = IA->getKind();
    else {
      // This last case seems like an error, but MSVC allocates one more slot
      // than is used for virtual inheritance.  We follow suit for
      // compatibility.
      // FIXME: Issue a warning.
      return 3 + int(Pointee->isFunctionType());
    }
  } else {
    if (RD->getNumVBases() > 0)
      Inheritance = attr::VirtualInheritance;
    else if (recordHasMultipleInheritance(RD))
      Inheritance = attr::MultipleInheritance;
    else
      Inheritance = attr::SingleInheritance;
  }

  switch (Inheritance) {
  case attr::SingleInheritance:
    return 1;
  case attr::MultipleInheritance:
    return 1 + int(Pointee->isFunctionType());
  case attr::VirtualInheritance:
    return 2 + int(Pointee->isFunctionType());
  default:
    llvm_unreachable("unknown inheritance model");
    return 0;
  }
}

CXXABI *clang::CreateMicrosoftCXXABI(ASTContext &Ctx) {
  return new MicrosoftCXXABI(Ctx);
}

