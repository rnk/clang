//===--- MSVCOptions.td - Options for clang-cl ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the options accepted by clang-cl.
//
//===----------------------------------------------------------------------===//

// Define special MSVC option kinds with higher precedence (-1) than the normal
// options.  If there is a conflict on the command line, it can be resolved by
// escaping it with -Xclang or -Xclang-driver.
// FIXME: Add some new escape to let the driver interpret it?

class MSVCFlag<string name> : Option<["/", "-"], name, KIND_FLAG>;
class MSVCJoined<string name> : Option<["/", "-"], name, KIND_JOINED>;
class MSVCJoinedOrSeparate<string name> :
    Option<["/", "-"], name, KIND_JOINED_OR_SEPARATE>;

// MSVC compatible options.  These are only enabled when clang is invoked as
// clang-cl, or when it is passed -ccc-msvc.  Always put the "/" prefix first so
// the help text renders with it.

// Escape for any gcc-style driver options that conflict with MSVC options,
// such as -MD.
// FIXME: Should -Xclang-driver always go to the driver, even on non-Windows?
// If so, remove the MSVCOption flag.
def Xclang_driver : Separate<["-"], "Xclang-driver">, Flags<[MSVCOption]>;

let Flags = [MSVCOption, DriverOption] in {

def _QUESTION : MSVCFlag<"?">,
  HelpText<"Print help for MSVC options">;
def showIncludes : MSVCFlag<"showIncludes">,
  HelpText<"Print include paths to stdout">;
def Fo : MSVCJoined<"Fo">,
  HelpText<"Output file or directory for multiple source files">;

// We can't accept -MD or -MT since they are already used for depfiles.  Even
// though -MDd and -MTd don't conflict, don't accept them for consistency.
def _SLASH_MD  : MSVCFlag<"MD">,  HelpText<"Use the dynamic, release Visual C runtime">;
def _SLASH_MT  : MSVCFlag<"MT">,  HelpText<"Use the static, release Visual C runtime">;
def _SLASH_MDd : MSVCFlag<"MDd">, HelpText<"Use the dynamic, debug Visual C runtime">;
def _SLASH_MTd : MSVCFlag<"MTd">, HelpText<"Use the static, debug Visual C runtime">;

// Optimization aliases.  We can't alias many of these because -O is joined and
// these are not.
def _SLASH_O : MSVCJoined<"O">, Alias<O>,
  HelpText<"Optimization level">;
def Ob0 : MSVCFlag<"Ob0">, Alias<fno_inline_functions>,
  HelpText<"Disables all inlining aside from __forceinline">;
def Ob : MSVCJoined<"Ob">,
  HelpText<"Controls inlining, unsupported">;
def Ox : MSVCFlag<"Ox">,
  HelpText<"Maximum optimizations (-O3)">;
def Od : MSVCFlag<"Od">,
  HelpText<"Disable optimizations">;
def Ot : MSVCFlag<"Ot">,
  HelpText<"Optimize for speed">;
def Oy : MSVCFlag<"Oy">, Alias<fomit_frame_pointer>,
  HelpText<"Enable frame pointer omission">;
def Oy_ : MSVCFlag<"Oy-">, Alias<fno_omit_frame_pointer>,
  HelpText<"Disable frame pointer omission">;
// Not implemented:
// /Og - global optimizations
// /Oi[-] - enable/disable intrinsics

// Language options.  These should mostly be aliases.
def GR : MSVCFlag<"GR">, Alias<frtti>, HelpText<"Enable RTTI">;
def GR_ : MSVCFlag<"GR-">, Alias<fno_rtti>, HelpText<"Disable RTTI">;

// Simple aliases.
def _SLASH_c : MSVCFlag<"c">, Alias<c>;
def _SLASH_I : MSVCJoined<"I">, Alias<I>;
def _SLASH_D : MSVCJoinedOrSeparate<"D">, Alias<D>;
def _SLASH_U : MSVCJoinedOrSeparate<"U">, Alias<U>;
def _SLASH_W1 : MSVCFlag<"W1">;    // -Wall
def _SLASH_W2 : MSVCFlag<"W2">;    // -Wall
def _SLASH_W3 : MSVCFlag<"W3">;    // -Wall
def _SLASH_WX : MSVCFlag<"WX">;    // -Werror
def _SLASH_WX_ : MSVCFlag<"WX-">;  // -Wno-error

// Compatibility no-ops.

def nologo : MSVCFlag<"nologo">;
def analyze_ : MSVCFlag<"analyze-">;

// Unsupported

def Zi : MSVCFlag<"Zi">;
def Zc_ : MSVCJoined<"Zc:">;
def fp_ : MSVCJoined<"fp:">;
def Fd : MSVCJoined<"Fd">;
def GS : MSVCFlag<"GS">;
def EH : MSVCJoined<"EH">;
def Gd : MSVCFlag<"Gd">;
def Gz : MSVCFlag<"Gz">;
def Gr : MSVCFlag<"Gr">;
def Gm : MSVCFlag<"Gm">;
def Gm_ : MSVCFlag<"Gm-">;
def GZ : MSVCFlag<"GZ">;
def RTC : MSVCJoined<"RTC">;
def TP : MSVCFlag<"TP">, HelpText<"Compile all files as C++">;
def TC : MSVCFlag<"TC">, HelpText<"Compile all files as C">;
def errorReport_ : MSVCJoined<"errorReport:">;

} // let Flags = [MSVCOption, DriverOption]
