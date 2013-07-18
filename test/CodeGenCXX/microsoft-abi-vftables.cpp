// RUN: cl -c %s -Fo%t.obj
// RUN: dumpbin /symbols %t.obj | cut -d '|' -f 2 | sort | FileCheck %s
// RUNX: %clang_cc1 %s -fno-rtti -cxx-abi microsoft -triple=i386-pc-win32 -emit-llvm -o - | FileCheck %s

// See microsoft-abi-structors.cpp for constructor codegen tests.

namespace Test1 {
// Classic diamond, fully virtual.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual A { virtual int c(); };
struct D : virtual B, virtual C { virtual int d(); };
D d; // Force vbtable emission.

// Layout should be:
// D: vbptr D
//    int d
// A: int a
// B: vbptr B
//    int b
// C: vbptr C
//    int c

// CHECK-DAG: ??_7D@Test1@@6B01@@
// CHECK-DAG: ??_7D@Test1@@6BB@1@@
// CHECK-DAG: ??_7D@Test1@@6BC@1@@
// CHECK-DAG: ??_7C@Test1@@6B@
// CHECK-DAG: ??_7B@Test1@@6B@
}

namespace Test2 {
// Classic diamond, only A is virtual.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual A { virtual int c(); };
struct D : B, C { virtual int d(); };
D d; // Force vbtable emission.

// Layout should be:
// B: vbptr B
//    int b
// C: vbptr C
//    int c
// D: int d
// A: int a

// CHECK-DAG: ??_7D@Test2@@6BB@1@@
// CHECK-DAG: ??_7D@Test2@@6BC@1@@
// CHECK-DAG: ??_7C@Test2@@6B@
// CHECK-DAG: ??_7B@Test2@@6B@
}

namespace Test3 {
struct A { virtual int a(); };
struct B { virtual int b(); };
struct C : virtual A, virtual B { virtual int c(); };
C c;

// CHECK-DAG: ??_7C@Test3@@6B@
}

namespace Test4 {
// Test reusing a vbptr from a non-virtual base.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : B, virtual A { virtual int c(); };
C c; // Force vbtable emission.

// CHECK-DAG: ??_7C@Test4@@6B@
// CHECK-DAG: ??_7B@Test4@@6B@
}

namespace Test5 {
// Test multiple base subobjects of the same type when that type has a virtual
// base.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : B { virtual int c(); };
struct D : B, C { virtual int d(); };
D d; // Force vbtable emission.

// CHECK-DAG: ??_7D@Test5@@6BB@1@@
// CHECK-DAG: ??_7D@Test5@@6BC@1@@
// CHECK-DAG: ??_7C@Test5@@6B@
// CHECK-DAG: ??_7B@Test5@@6B@
}

namespace Test6 {
// Test that we skip unneeded base path component names.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : B { virtual int c(); };
struct D : B, C { virtual int d(); };
struct E : D { virtual int e(); };
struct F : E, B, C { virtual int f(); };
struct G : F, virtual E { virtual int g(); };
G g;

// CHECK-DAG: ??_7G@Test6@@6BB@1@E@1@F@1@@
// CHECK-DAG: ??_7G@Test6@@6BC@1@E@1@F@1@@
// CHECK-DAG: ??_7G@Test6@@6BB@1@F@1@@
// CHECK-DAG: ??_7G@Test6@@6BC@1@F@1@@
// CHECK-DAG: ??_7G@Test6@@6BB@1@E@1@@
// CHECK-DAG: ??_7G@Test6@@6BC@1@E@1@@
// CHECK-DAG: ??_7F@Test6@@6BB@1@E@1@@
// CHECK-DAG: ??_7F@Test6@@6BC@1@E@1@@
// CHECK-DAG: ??_7F@Test6@@6BB@1@@
// CHECK-DAG: ??_7F@Test6@@6BC@1@@
// CHECK-DAG: ??_7C@Test6@@6B@
// CHECK-DAG: ??_7B@Test6@@6B@
// CHECK-DAG: ??_7E@Test6@@6BB@1@@
// CHECK-DAG: ??_7E@Test6@@6BC@1@@
// CHECK-DAG: ??_7D@Test6@@6BB@1@@
// CHECK-DAG: ??_7D@Test6@@6BC@1@@
}

namespace Test7 {
// Test a non-virtual base which reuses the vbptr of another base.
struct A { virtual int a(); };
struct B { virtual int b(); };
struct C { virtual int c(); };
struct D : virtual A { virtual int d(); };
struct E : B, D, virtual A, virtual C { virtual int e(); };
E o;

// CHECK-DAG: ??_7E@Test7@@6B@
// CHECK-DAG: ??_7D@Test7@@6B@
}

namespace Test8 {
// Test a virtual base which reuses the vbptr of another base.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : B { virtual int c(); };
struct D : virtual C { virtual int d(); };
D o;

// CHECK-DAG: ??_7D@Test8@@6B01@@
// CHECK-DAG: ??_7D@Test8@@6BC@1@@
// CHECK-DAG: ??_7C@Test8@@6B@
// CHECK-DAG: ??_7B@Test8@@6B@
}

namespace Test9 {
// D has to add to B's vbtable because D has more morally virtual bases than B.
// D then takes B's vbptr and the vbtable is named for D, not B.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); };
struct BB : B { int bb; };  // Indirection =/
struct D : BB, C { virtual int d(); };
struct E : virtual D { };
E e;

// CHECK-DAG: ??_7E@Test9@@6B01@@
// CHECK-DAG: ??_7E@Test9@@6BD@1@@
// CHECK-DAG: ??_7E@Test9@@6BC@1@@
// CHECK-DAG: ??_7E@Test9@@6BB@1@@
// CHECK-DAG: ??_7D@Test9@@6B@
// CHECK-DAG: ??_7D@Test9@@6BC@1@@
// CHECK-DAG: ??_7D@Test9@@6BB@1@@
// CHECK-DAG: ??_7C@Test9@@6B01@@
// CHECK-DAG: ??_7C@Test9@@6BB@1@@
// CHECK-DAG: ??_7BB@Test9@@6B@
// CHECK-DAG: ??_7B@Test9@@6B@
}

namespace Test10 {
struct A { virtual int a(); };
struct B { virtual int b(); };
struct C : virtual A { virtual int c(); };
struct D : B, C { virtual int d(); };
D d;

// CHECK-DAG: ??_7D@Test10@@6B@
// CHECK-DAG: ??_7C@Test10@@6B@

}

namespace Test11 {
// Typical diamond with an extra single inheritance indirection for B and C.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual A { virtual int c(); };
struct D : B { virtual int d(); };
struct E : C { virtual int e(); };
struct F : D, E { virtual int f(); };
F f;

// CHECK-DAG: ??_7F@Test11@@6BD@1@@
// CHECK-DAG: ??_7F@Test11@@6BE@1@@
// CHECK-DAG: ??_7E@Test11@@6B@
// CHECK-DAG: ??_7C@Test11@@6B@
// CHECK-DAG: ??_7D@Test11@@6B@
// CHECK-DAG: ??_7B@Test11@@6B@

}

namespace Test12 {
// Another vbptr inside a virtual base.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); };
struct D : C, B { virtual int d(); };
struct E : D, C, B { virtual int e(); };
E e;

// CHECK-DAG: ??_7E@Test12@@6BC@1@D@1@@
// CHECK-DAG: ??_7E@Test12@@6BB@1@D@1@@
// CHECK-DAG: ??_7E@Test12@@6BD@1@@
// CHECK-DAG: ??_7E@Test12@@6BC@1@@
// CHECK-DAG: ??_7E@Test12@@6BB@1@@
// CHECK-DAG: ??_7C@Test12@@6B01@@
// CHECK-DAG: ??_7C@Test12@@6BB@1@@
// CHECK-DAG: ??_7D@Test12@@6BC@1@@
// CHECK-DAG: ??_7D@Test12@@6BB@1@@
// CHECK-DAG: ??_7D@Test12@@6B@
// CHECK-DAG: ??_7B@Test12@@6B@
}

namespace Test13 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); };
struct D : virtual C { virtual int d(); };
struct E : D, C, B { virtual int e(); };
E e;

// CHECK-DAG: ??_7E@Test13@@6BD@1@@
// CHECK-DAG: ??_7E@Test13@@6BC@1@D@1@@
// CHECK-DAG: ??_7E@Test13@@6BB@1@D@1@@
// CHECK-DAG: ??_7E@Test13@@6BC@1@@
// CHECK-DAG: ??_7E@Test13@@6BB@1@@
// CHECK-DAG: ??_7D@Test13@@6B@
// CHECK-DAG: ??_7D@Test13@@6BC@1@@
// CHECK-DAG: ??_7D@Test13@@6BB@1@@
// CHECK-DAG: ??_7C@Test13@@6B01@@
// CHECK-DAG: ??_7C@Test13@@6BB@1@@
// CHECK-DAG: ??_7B@Test13@@6B@
}

namespace Test14 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); };
struct D : virtual C { virtual int d(); };
struct E : D, virtual C, virtual B { virtual int e(); };
E e;

// CHECK-DAG: ??_7E@Test14@@6B@
// CHECK-DAG: ??_7E@Test14@@6BC@1@@
// CHECK-DAG: ??_7E@Test14@@6BB@1@@
// CHECK-DAG: ??_7D@Test14@@6B@
// CHECK-DAG: ??_7D@Test14@@6BC@1@@
// CHECK-DAG: ??_7D@Test14@@6BB@1@@
// CHECK-DAG: ??_7C@Test14@@6B01@@
// CHECK-DAG: ??_7C@Test14@@6BB@1@@
// CHECK-DAG: ??_7B@Test14@@6B@
}

namespace Test15 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual A { virtual int c(); };
struct D : virtual B { virtual int d(); };
struct E : D, C, B { virtual int e(); };
E e;

// CHECK-DAG: ??_7E@Test15@@6BD@1@@
// CHECK-DAG: ??_7E@Test15@@6BB@1@D@1@@
// CHECK-DAG: ??_7E@Test15@@6BC@1@@
// CHECK-DAG: ??_7E@Test15@@6BB@1@@
// CHECK-DAG: ??_7C@Test15@@6B@
// CHECK-DAG: ??_7D@Test15@@6B01@@
// CHECK-DAG: ??_7D@Test15@@6BB@1@@
// CHECK-DAG: ??_7B@Test15@@6B@
}

namespace Test16 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); }; // ambig
struct D : virtual C { virtual int d(); };
struct E : virtual D { virtual int e(); }; // ambig
struct F : E, D, C, B { virtual int f(); };  // ambig
F f;

// CHECK-DAG: ??_7F@Test16@@6BE@1@@
// CHECK-DAG: ??_7F@Test16@@6BD@1@E@1@@
// CHECK-DAG: ??_7F@Test16@@6BC@1@E@1@@
// CHECK-DAG: ??_7F@Test16@@6BB@1@E@1@@
// CHECK-DAG: ??_7F@Test16@@6BD@1@@
// CHECK-DAG: ??_7F@Test16@@6BC@1@@
// CHECK-DAG: ??_7F@Test16@@6BB@1@@
// CHECK-DAG: ??_7E@Test16@@6B01@@
// CHECK-DAG: ??_7E@Test16@@6BD@1@@
// CHECK-DAG: ??_7E@Test16@@6BC@1@@
// CHECK-DAG: ??_7E@Test16@@6BB@1@@
// CHECK-DAG: ??_7D@Test16@@6B@
// CHECK-DAG: ??_7D@Test16@@6BC@1@@
// CHECK-DAG: ??_7D@Test16@@6BB@1@@
// CHECK-DAG: ??_7C@Test16@@6B01@@
// CHECK-DAG: ??_7C@Test16@@6BB@1@@
// CHECK-DAG: ??_7B@Test16@@6B@
}

namespace Test17 {
// This test case has an interesting alternating pattern of using "vbtable of B"
// and "vbtable of C for C".  This may be the key to the underlying algorithm.
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); }; // ambig
struct D : virtual C { virtual int d(); };
struct E : virtual D { virtual int e(); }; // ambig
struct F : virtual E { virtual int f(); };
struct G : virtual F { virtual int g(); }; // ambig
struct H : virtual G { virtual int h(); };
struct I : virtual H { virtual int i(); }; // ambig
struct J : virtual I { virtual int j(); };
struct K : virtual J { virtual int k(); }; // ambig
K k;

// CHECK-DAG: ??_7K@Test17@@6B01@@
// CHECK-DAG: ??_7J@Test17@@6B@
// CHECK-DAG: ??_7I@Test17@@6B01@@
// CHECK-DAG: ??_7H@Test17@@6B@
// CHECK-DAG: ??_7G@Test17@@6B01@@
// CHECK-DAG: ??_7F@Test17@@6B@
// CHECK-DAG: ??_7E@Test17@@6B01@@
// CHECK-DAG: ??_7D@Test17@@6B@
// CHECK-DAG: ??_7C@Test17@@6B01@@
// CHECK-DAG: ??_7B@Test17@@6B@
}

namespace Test18 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : B { virtual int c(); };
struct D : C, B { virtual int d(); };
struct E : D, C, B { virtual int e(); };
E e;

// CHECK-DAG: ??_7E@Test18@@6BC@1@D@1@@
// CHECK-DAG: ??_7E@Test18@@6BB@1@D@1@@
// CHECK-DAG: ??_7E@Test18@@6BC@1@@
// CHECK-DAG: ??_7E@Test18@@6BB@1@@
// CHECK-DAG: ??_7B@Test18@@6B@
// CHECK-DAG: ??_7C@Test18@@6B@
// CHECK-DAG: ??_7D@Test18@@6BC@1@@
// CHECK-DAG: ??_7D@Test18@@6BB@1@@
}

namespace Test19 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : virtual B { virtual int c(); };
struct D : virtual C, virtual B { virtual int d(); };
struct E : virtual D, virtual C, virtual B { virtual int e(); };
E e;

// CHECK-DAG: ??_7E@Test19@@6B01@@
// CHECK-DAG: ??_7E@Test19@@6BD@1@@
// CHECK-DAG: ??_7E@Test19@@6BC@1@@
// CHECK-DAG: ??_7E@Test19@@6BB@1@@
// CHECK-DAG: ??_7D@Test19@@6B@
// CHECK-DAG: ??_7D@Test19@@6BC@1@@
// CHECK-DAG: ??_7D@Test19@@6BB@1@@
// CHECK-DAG: ??_7C@Test19@@6B01@@
// CHECK-DAG: ??_7C@Test19@@6BB@1@@
// CHECK-DAG: ??_7B@Test19@@6B@
}

namespace Test20 {
// E has no direct vbases, but it adds to C's vbtable anyway.
struct A { virtual int a(); };
struct B { virtual int b(); };
struct C : virtual A { virtual int c(); };
struct D : virtual B { virtual int d(); };
struct E : C, D { virtual int e(); };
E f;

// CHECK-DAG: ??_7E@Test20@@6BC@1@@
// CHECK-DAG: ??_7E@Test20@@6BD@1@@
// CHECK-DAG: ??_7D@Test20@@6B@
// CHECK-DAG: ??_7C@Test20@@6B@
}

namespace Test21 {
struct A { virtual int a(); };
struct B : virtual A { virtual int b(); };
struct C : B { virtual int c(); };
struct D : B { virtual int d(); };
struct E : C, D { virtual int e(); };
struct F : virtual E { virtual int f(); };
struct G : E { virtual int g(); };
struct H : F, G { virtual int h(); };
H h;

// CHECK-DAG: ??_7H@Test21@@6B@
// CHECK-DAG: ??_7H@Test21@@6BC@1@F@1@@
// CHECK-DAG: ??_7H@Test21@@6BD@1@F@1@@
// CHECK-DAG: ??_7H@Test21@@6BC@1@G@1@@
// CHECK-DAG: ??_7H@Test21@@6BD@1@G@1@@
// CHECK-DAG: ??_7G@Test21@@6BC@1@@
// CHECK-DAG: ??_7G@Test21@@6BD@1@@
// CHECK-DAG: ??_7F@Test21@@6B@
// CHECK-DAG: ??_7F@Test21@@6BC@1@@
// CHECK-DAG: ??_7F@Test21@@6BD@1@@
// CHECK-DAG: ??_7E@Test21@@6BC@1@@
// CHECK-DAG: ??_7E@Test21@@6BD@1@@
// CHECK-DAG: ??_7D@Test21@@6B@
// CHECK-DAG: ??_7B@Test21@@6B@
// CHECK-DAG: ??_7C@Test21@@6B@
}
