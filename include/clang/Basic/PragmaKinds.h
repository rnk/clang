//===--- PragmaKinds.h - Enum values for pragmas ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Provides enums used across Lex, Parse, and Sema.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_PRAGMAKINDS_H
#define LLVM_CLANG_BASIC_PRAGMAKINDS_H

namespace clang {

enum PragmaMSCommentKind {
  PCK_Unknown,
  PCK_Linker,   // #pragma comment(linker, ...)
  PCK_Lib,      // #pragma comment(lib, ...)
  PCK_Compiler, // #pragma comment(compiler, ...)
  PCK_ExeStr,   // #pragma comment(exestr, ...)
  PCK_User      // #pragma comment(user, ...)
};

// TODO: Consider moving some of the pragma kind enums from Sema here.

}  // end namespace clang

#endif
