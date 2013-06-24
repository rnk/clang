// Test some simple aliases.  Constructing the compile job re-orders the
// arguments, hence -DAG.
//
// RUN: %clang -ccc-msvc /Ox /Oy- /Ob0 /GR- /MDd -c %s -### 2>&1 | FileCheck %s
// CHECK-DAG: "-O3"
// CHECK-DAG: "-mdisable-fp-elim"
// CHECK-DAG: "-fno-inline-functions"
// CHECK-DAG: "-fno-rtti"
// CHECK-DAG: "-D" "_DLL"
// CHECK-DAG: "-D" "_DEBUG"
// CHECK-DAG: "-include" "{{.*}}msvcrt_picker.h"

// By default (no -ccc-msvc arg), the driver should not try to emulate MSVC.
//
// RUN: %clang /Od /GR- /MDd -c %s -### 2>&1 | FileCheck -check-prefix=NOTMSVC %s
// NOTMSVC: error: no such file or directory: '/Od'
// NOTMSVC: error: no such file or directory: '/GR-'
// NOTMSVC: error: no such file or directory: '/MDd'
