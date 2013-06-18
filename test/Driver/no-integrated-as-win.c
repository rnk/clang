// RUN: %clang -target x86_64-pc-win32 -### -no-integrated-as %s -c 2>&1 | FileCheck %s
// CHECK: there is no external assembler we can use on windows

// But there is for mingw.  The source file should only be mentioned once for
// the compile step.
// RUN: %clang -target i686-pc-mingw32 -### -no-integrated-as %s -c 2>&1 | FileCheck -check-prefix=MINGW %s
// MINGW: "-cc1"
// MINGW: no-integrated-as-win.c
// MINGW-NOT: no-integrated-as-win.c
