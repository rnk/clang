//===--- MSVCOptions.cpp - Clang Assembler Options Table -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MSVCOptions.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Option/Option.h"
using namespace clang;
using namespace clang::driver;
using namespace llvm::opt;
using namespace clang::driver::msvcoptions;
// MSVC option aliases are aliases into clang options, so we need to pull in
// clang::driver::options.
using namespace clang::driver::options;

#define PREFIX(NAME, VALUE) const char *const NAME[] = VALUE;
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, FLAGS, PARAM, \
               HELPTEXT, METAVAR)
#include "clang/Driver/MSVCOptions.inc"
#undef OPTION
#undef PREFIX

static const OptTable::Info MSVCInfoTable[] = {
#define PREFIX(NAME, VALUE)
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, FLAGS, PARAM, \
               HELPTEXT, METAVAR)   \
  { PREFIX, NAME, HELPTEXT, METAVAR, OPT_##ID, Option::KIND##Class, PARAM, \
    FLAGS, OPT_##GROUP, OPT_##ALIAS },
#include "clang/Driver/MSVCOptions.inc"
};

namespace {

class MSVCOptTable : public OptTable {
public:
  MSVCOptTable()
    : OptTable(MSVCInfoTable, sizeof(MSVCInfoTable) / sizeof(MSVCInfoTable[0]))
  {}
};

}

OptTable *clang::driver::createMSVCOptTable() {
  return new MSVCOptTable();
}
