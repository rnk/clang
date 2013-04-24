// RUN: %clang_cc1 %s -fms-extensions -fsyntax-only -dump-tokens 2>&1 | FileCheck -check-prefix TOK %s
// RUN: %clang_cc1 %s -fms-extensions -emit-llvm -o - | FileCheck %s

#pragma comment(lib, "msvcrt.lib")
// CHECK-TOK: annot_pragma_mscomment
// CHECK-TOK: string_literal 'msvcrt.lib'

#define BAR "2"
#pragma comment(linker," /bar=" BAR)
// CHECK-TOK: annot_pragma_mscomment
// CHECK-TOK: string_literal ' /bar=2'

// CHECK: !llvm.module.flags = !{!0}
// CHECK: !0 = metadata !{i32 6, metadata !"Linker Options", metadata ![[link_opts:[0-9]+]]}
// CHECK: ![[link_opts]] = metadata !{metadata ![[msvcrt:[0-9]+]], metadata ![[bar:[0-9]+]]}
// CHECK: ![[msvcrt]] = metadata !{metadata !"/DEFAULTLIB:msvcrt.lib"}
// CHECK: ![[bar]] = metadata !{metadata !" /bar=2"}
