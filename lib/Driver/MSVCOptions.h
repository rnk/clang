//===--- MSVCOptions.h - Clang Assembler Options Table ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_DRIVER_MSVCOPTIONS_H
#define CLANG_DRIVER_MSVCOPTIONS_H

#include "clang/Driver/Options.h"

namespace clang {
namespace driver {
namespace msvcoptions {
  enum ID {
    // Keep the enum disjoint.
    BeforeFirstMSVCOption = clang::driver::options::LastOption,
#define PREFIX(NAME, VALUE)
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, FLAGS, PARAM, \
               HELPTEXT, METAVAR) OPT_##ID,
#include "clang/Driver/MSVCOptions.inc"
    LastMSVCOption
#undef OPTION
#undef PREFIX
  };
}

llvm::opt::OptTable *createMSVCOptTable();
}
}

#endif
