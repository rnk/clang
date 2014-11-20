// RUN: %clang_cc1 %s -triple x86_64-pc-win32 -fexceptions -fms-extensions -emit-llvm -o - | FileCheck %s

// FIXME: Perform this outlining automatically CodeGen.
void try_body(int numerator, int denominator, int *myres) {
  *myres = numerator / denominator;
}
// CHECK-LABEL: define void @try_body(i32 %numerator, i32 %denominator, i32* %myres)
// CHECK: sdiv i32
// CHECK: store i32 %{{.*}}, i32*
// CHECK: ret void

int SafeDiv(int numerator, int denominator, int *res) {
  int myres = 0;
  int success = 1;
  __try {
    try_body(numerator, denominator, &myres);
  } __except (1) {
    success = 0;
  }
  *res = myres;
  return success;
}
// CHECK-LABEL: define i32 @SafeDiv(i32 %numerator, i32 %denominator, i32* %res)
// CHECK: invoke void @try_body(i32 %{{.*}}, i32 %{{.*}}, i32* %{{.*}})
// CHECK:       to label %[[cont:[^ ]*]] unwind label %[[lpad:[^ ]*]]
// CHECK: [[lpad]]
// CHECK: landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__C_specific_handler to i8*)
// CHECK:       catch i32 (i8*, i8*)* @"\01?filt$0@0@SafeDiv@@"
// CHECK: %[[sel:[^ ]*]] = load i32*
// CHECK: %[[typeid:[^ ]*]] = call i32 @llvm.eh.typeid.for(i8* bitcast (i32 (i8*, i8*)* @"\01?filt$0@0@SafeDiv@@" to i8*))
// CHECK: icmp eq i32 %[[sel]], %[[typeid]]
// CHECK: [[cont]]
// CHECK: %[[myres:[^ ]*]] = load i32*
// CHECK: store i32 %[[myres]], i32*

void j(void);

int nested_try() {
  int r = 42;
  __try {
    __try {
      j();
      r = 0;
    } __except(_exception_code() == 123) {
      r = 123;
    }
  } __except(_exception_code() == 456) {
    r = 456;
  }
  return r;
}
// CHECK-LABEL: define i32 @nested_try()
// CHECK: store i32 42, i32* %[[r:[^ ,]*]]
// CHECK: invoke void @j()
// CHECK:       to label %[[cont:[^ ]*]] unwind label %[[lpad:[^ ]*]]
//
// CHECK: [[cont]]
// CHECK: store i32 0, i32* %[[r]]
// CHECK: br label %[[inner_try_cont:[^ ]*]]
//
// CHECK: [[lpad]]
// CHECK: landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__C_specific_handler to i8*)
// CHECK:       catch i32 (i8*, i8*)* @"\01?filt$0@0@nested_try@@"
// CHECK:       catch i32 (i8*, i8*)* @"\01?filt$1@0@nested_try@@"
//
// CHECK: %[[sel:[^ ]*]] = load i32*
// CHECK: %[[typeid:[^ ]*]] = call i32 @llvm.eh.typeid.for(i8* bitcast (i32 (i8*, i8*)* @"\01?filt$0@0@nested_try@@" to i8*))
// CHECK: icmp eq i32 %[[sel]], %[[typeid]]
// CHECK: br i1
//
// CHECK: %[[sel:[^ ]*]] = load i32*
// CHECK: %[[typeid:[^ ]*]] = call i32 @llvm.eh.typeid.for(i8* bitcast (i32 (i8*, i8*)* @"\01?filt$1@0@nested_try@@" to i8*))
// CHECK: icmp eq i32 %[[sel]], %[[typeid]]
// CHECK: br i1
//
// CHECK: store i32 456, i32* %[[r]]
// CHECK: br label %[[outer_try_cont:[^ ]*]]
//
// CHECK: [[outer_try_cont]]
// CHECK: %[[r_load:[^ ]*]] = load i32* %[[r]]
// CHECK: ret i32 %[[r_load]]
//
// CHECK: store i32 123, i32* %[[r]]
// CHECK: br label %[[inner_try_cont]]
//
// CHECK: [[inner_try_cont]]
// CHECK: br label %[[outer_try_cont]]

// FIXME: This lowering of __finally can't actually work, it will have to
// change.
static unsigned g = 0;
void basic_finally() {
  ++g;
  __try {
    j();
  } __finally {
    --g;
  }
}
// CHECK-LABEL: define void @basic_finally()
// CHECK: load i32* @g
// CHECK: add i32 %{{.*}}, 1
// CHECK: store i32 %{{.*}}, i32* @g
//
// CHECK: invoke void @j()
// CHECK:       to label %[[cont:[^ ]*]] unwind label %[[lpad:[^ ]*]]
//
// CHECK: [[cont]]
// CHECK: load i32* @g
// CHECK: add i32 %{{.*}}, -1
// CHECK: store i32 %{{.*}}, i32* @g
// CHECK: ret void
//
// CHECK: [[lpad]]
// CHECK: landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__C_specific_handler to i8*)
// CHECK-NEXT: cleanup
// CHECK: load i32* @g
// CHECK: add i32 %{{.*}}, -1
// CHECK: store i32 %{{.*}}, i32* @g
// CHECK: resume
