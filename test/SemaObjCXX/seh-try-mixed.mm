// RUN: clang -cc1 -triple i686-pc-windows-msvc -verify %s -fms-extensions -fexceptions -fcxx-exceptions -fobjc-exceptions

void bar();

int seh_cxx() {
  __try { // expected-error {{SEH __try statements cannot appear in the same function as C++ try statements}}
    bar();
  } __except (1) {
  }
  try { // expected-note {{conflicting C++ try statement here}}
    bar();
  } catch (...) {
  }
}

int seh_objc() {
  __try { // expected-error {{SEH __try statements cannot appear in the same function as Obj-C @try statements}}
    bar();
  } __except(1) {
  }
  @try { // expected-note {{conflicting Obj-C @try statement here}}
    bar();
  } @catch (id) {
  }
}

int cxx_seh() {
  try { // expected-note {{conflicting C++ try statement here}}
    bar();
  } catch (...) {
  }
  __try { // expected-error {{SEH __try statements cannot appear in the same function as C++ try statements}}
    bar();
  } __except (1) {
  }
}

int objc_seh() {
  @try { // expected-note {{conflicting Obj-C @try statement here}}
    bar();
  } @catch (id) {
  }
  __try { // expected-error {{SEH __try statements cannot appear in the same function as Obj-C @try statements}}
    bar();
  } __except(1) {
  }
}
