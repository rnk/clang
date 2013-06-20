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

def msvc_driver_Group : OptionGroup<"<clang driver internal options>">,
  HelpText<"clang-cl options">;
class MSVCDriverOpt : Group<msvc_driver_Group>, Flags<[MSVCOption, DriverOption]>;

// Define special MSVC option kinds with higher precedence (-1) than the normal
// options.  If there is a conflict on the command line, it can be resolved by
// escaping it with -Xclang.
// FIXME: Add some new escape to let the driver interpret it?

def KIND_MSVC_FLAG : OptionKind<"Flag", -1>;
def KIND_MSVC_JOINED : OptionKind<"Joined", -1>;

class MSVCFlag<string name> : Option<["/", "-"], name, KIND_MSVC_FLAG>, MSVCDriverOpt;
class MSVCJoined<string name> : Option<["/", "-"], name, KIND_MSVC_JOINED>, MSVCDriverOpt;

// So far we haven't needed Separate or the other kinds of flags.

// MSVC compatible options.  These are only enabled when clang is invoked as
// clang-cl, or when it is passed -ccc-msvc.  Always put the "/" prefix first so
// the help text renders with it.

let Flags = [MSVCOption, DriverOption] in {

def _QUESTION : MSVCFlag<"?">,
  HelpText<"Print help for MSVC options">;
def showIncludes : MSVCFlag<"showIncludes">,
  HelpText<"Print include paths to stdout">;

// We can't accept -MD or -MT since they are already used for depfiles.  Even
// though -MDd and -MTd don't conflict, don't accept them for consistency.
def _SLASH_MD  : MSVCFlag<"MD">,  HelpText<"Use the dynamic, release Visual C runtime">;
def _SLASH_MT  : MSVCFlag<"MT">,  HelpText<"Use the static, release Visual C runtime">;
def _SLASH_MDd : MSVCFlag<"MDd">, HelpText<"Use the dynamic, debug Visual C runtime">;
def _SLASH_MTd : MSVCFlag<"MTd">, HelpText<"Use the static, debug Visual C runtime">;

// Optimization aliases.  We can't alias these because -O is joined and these
// are not.
def _SLASH_O1 : MSVCFlag<"O1">,
  HelpText<"Enables optimizations that run quickly">;
def Ob : MSVCJoined<"Ob">,
  HelpText<"inlining: /Ob0 is off, /Ob1 or greater is on">;
def _SLASH_Os : MSVCFlag<"Os">,
  HelpText<"Optimize for small code size">;
def Ox : MSVCFlag<"Ox">,
  HelpText<"Maximum optimizations (-O3)">;
def _SLASH_O2 : MSVCFlag<"O2">,
  HelpText<"Optimize for speed">;
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

} // let Flags = [MSVCOption, DriverOption]
