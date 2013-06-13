//===--- MSVCOptions.td - Options for clang-cl ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the options accepted by clang -ccc-msvc.
//
//===----------------------------------------------------------------------===//

// Include the common option parsing interfaces.
include "llvm/Option/OptParser.td"

/////////
// Options

// See the comments about option id naming in Options.td.  We disambiguate
// between MSVC options and clang options by prefixing with _SLASH_.  This
// allows us to pull both option enums into the same namespace, which is how we
// implement cross namespace aliasing.

def ccc_msvc : Flag<["-"], "ccc-msvc">,
  HelpText<"Act as Microsoft's cl.exe compiler.  Must be the first argument.">;

// MSVC compatible options.  These are only enabled in special circumstances.
// Always put the "/" prefix first so the help text renders with it.

def _QUESTION : Flag<["/", "-"], "?">,
  HelpText<"Print help for MSVC options">;
def showIncludes : Flag<["/", "-"], "showIncludes">,
  HelpText<"Print include paths to stdout">;

def _SLASH_MD  : Flag<["/", "-"], "MD">,  HelpText<"Use the dynamic, release Visual C runtime">;
def _SLASH_MT  : Flag<["/", "-"], "MT">,  HelpText<"Use the static, release Visual C runtime">;
def _SLASH_MDd : Flag<["/", "-"], "MDd">, HelpText<"Use the dynamic, debug Visual C runtime">;
def _SLASH_MTd : Flag<["/", "-"], "MTd">, HelpText<"Use the static, debug Visual C runtime">;

// Optimization aliases.  We can't alias these because -O is joined and these
// are not.
def _SLASH_O1 : Flag<["/"], "O1">,
  HelpText<"Enables optimizations that run quickly">;
def Ob : Joined<["/", "-"], "Ob">,
  HelpText<"inlining: /Ob0 is off, /Ob1 or greater is on">;
def _SLASH_Os : Flag<["/"], "Os">,
  HelpText<"Optimize for small code size">;
def Ox : Flag<["/", "-"], "Ox">,
  HelpText<"Maximum optimizations (-O3)">;
def _SLASH_O2 : Flag<["/"], "O2">,
  HelpText<"Optimize for speed">;
def Od : Flag<["/", "-"], "Od">,
  HelpText<"Disable optimizations">;
def Ot : Flag<["/", "-"], "Ot">,
  HelpText<"Optimize for speed">;
def Oy : Flag<["/", "-"], "Oy">, Alias<fomit_frame_pointer>,
  HelpText<"Enable frame pointer omission">;
def Oy_ : Flag<["/", "-"], "Oy-">, Alias<fno_omit_frame_pointer>,
  HelpText<"Disable frame pointer omission">;
// Not implemented:
// /Og - global optimizations
// /Oi[-] - enable/disable intrinsics

// Language options.  These should mostly be aliases.
def GR : Flag<["/", "-"], "GR">, Alias<frtti>, HelpText<"Enable RTTI">;
def GR_ : Flag<["/", "-"], "GR-">, Alias<fno_rtti>, HelpText<"Disable RTTI">;
