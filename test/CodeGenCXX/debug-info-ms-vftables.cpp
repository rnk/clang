// RUN: %clang_cc1 %s -triple=i686-pc-windows-msvc -debug-info-kind=limited -gcodeview -emit-llvm -o - | FileCheck %s

namespace diamond {
struct A { virtual void f(); };
struct B : virtual A { virtual void g(); };
struct C : virtual A { virtual void h(); };
struct D : B, C { virtual void j(); };
D d;
}

// CHECK: distinct !DICompositeType(tag: DW_TAG_structure_type, name: "D",
// CHECK-SAME:    elements: ![[elements:[0-9]+]],
// CHECK-SAME:    identifier: ".?AUD@diamond@@")

// CHECK: ![[elements]] = !{!{{[0-9]+}}, !{{[0-9]+}}, ![[vt_D_B:[0-9]+]], ![[vt_D_A:[0-9]+]], ![[vt_D_C:[0-9]+]], !{{[0-9]+}}}

// CHECK: ![[vt_D_B]] = !DIDerivedType(tag: DW_TAG_LLVM_msvftable, name: "??_7D@diamond@@6BB@1@@",
// CHECK: ![[vt_D_A]] = !DIDerivedType(tag: DW_TAG_LLVM_msvftable, name: "??_7D@diamond@@6BA@1@@",
// CHECK: ![[vt_D_C]] = !DIDerivedType(tag: DW_TAG_LLVM_msvftable, name: "??_7D@diamond@@6BC@1@@",

// This test tries to exercise all four of the possible MS ABI manglings for
// C++ virtual methods:
// - normal methods
// - virtual dtors
// - virtual dtor thunks
// - virtual method thunks
namespace thunks {
struct A {
  virtual ~A();
  virtual void f();
};
struct B {
  virtual ~B();
  virtual void f();
};
struct C : A, B {
  virtual ~C();
  virtual void f();
};
C c;
}

// CHECK: ![[C_thunks:[0-9]+]] = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "C",
// CHECK-SAME:    elements: ![[elements:[0-9]+]],
// CHECK-SAME:    identifier: ".?AUC@thunks@@")

// CHECK: ![[elements]] = !{!{{[0-9]+}}, !{{[0-9]+}}, ![[vt_C_A:[0-9]+]], ![[vt_C_B:[0-9]+]], !{{[0-9]+}}, !{{[0-9]+}}}

// CHECK: ![[vt_C_A]] = !DIDerivedType(tag: DW_TAG_LLVM_msvftable, name: "??_7C@thunks@@6BA@1@@",
// CHECK-SAME:   extraData: ![[vt_C_A_methods:[0-9]+]])

// CHECK: ![[vt_C_A_methods]] = !{!"??_GC@thunks@@UAEPAXI@Z", !"?f@C@thunks@@UAEXXZ"}

// CHECK: ![[vt_C_B]] = !DIDerivedType(tag: DW_TAG_LLVM_msvftable, name: "??_7C@thunks@@6BB@1@@",
// CHECK-SAME:   extraData: ![[vt_C_B_methods:[0-9]+]])

// CHECK: ![[vt_C_B_methods]] = !{!"??_EC@thunks@@W3AEPAXI@Z", !"?f@C@thunks@@W3AEXXZ"}
