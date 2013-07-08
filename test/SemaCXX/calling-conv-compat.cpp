// RUN: %clang_cc1 -fsyntax-only -fms-extensions -cxx-abi microsoft -verify -triple i686-pc-win32 %s

// Pointers to free functions
void            free_func_default();
void __cdecl    free_func_cdecl();
void __stdcall  free_func_stdcall();
void __fastcall free_func_fastcall();

typedef void (           *fptr_default)();
typedef void (__cdecl    *fptr_cdecl)();
typedef void (__stdcall  *fptr_stdcall)();
typedef void (__fastcall *fptr_fastcall)();

// expected-note@+4 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'fptr_default' (aka 'void (*)()') for 1st argument}}
// expected-note@+3 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'fptr_default' (aka 'void (*)()') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (*)() __attribute__((stdcall))' to 'fptr_default' (aka 'void (*)()') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (*)() __attribute__((fastcall))' to 'fptr_default' (aka 'void (*)()') for 1st argument}}
void cb_fptr_default(fptr_default ptr);
// expected-note@+4 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'fptr_cdecl' (aka 'void (*)()') for 1st argument}}
// expected-note@+3 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'fptr_cdecl' (aka 'void (*)()') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (*)() __attribute__((stdcall))' to 'fptr_cdecl' (aka 'void (*)()') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (*)() __attribute__((fastcall))' to 'fptr_cdecl' (aka 'void (*)()') for 1st argument}}
void cb_fptr_cdecl(fptr_cdecl ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void ()' to 'fptr_stdcall' (aka 'void (*)() __attribute__((stdcall))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((cdecl))' to 'fptr_stdcall' (aka 'void (*)() __attribute__((stdcall))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'fptr_stdcall' (aka 'void (*)() __attribute__((stdcall))') for 1st argument}}
void cb_fptr_stdcall(fptr_stdcall ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void ()' to 'fptr_fastcall' (aka 'void (*)() __attribute__((fastcall))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((cdecl))' to 'fptr_fastcall' (aka 'void (*)() __attribute__((fastcall))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'fptr_fastcall' (aka 'void (*)() __attribute__((fastcall))') for 1st argument}}
void cb_fptr_fastcall(fptr_fastcall ptr);
// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'const fptr_default' (aka 'void (*const)()') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'const fptr_default' (aka 'void (*const)()') for 1st argument}}
void cb_fptr_const_default(const fptr_default ptr);

void call_free_func() {
  cb_fptr_default(free_func_default);
  cb_fptr_default(free_func_cdecl);
  cb_fptr_default(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fptr_default'}}
  cb_fptr_default(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fptr_default'}}
  cb_fptr_default(&free_func_default);
  cb_fptr_default(&free_func_cdecl);
  cb_fptr_default(&free_func_stdcall); // expected-error {{no matching function for call to 'cb_fptr_default'}}
  cb_fptr_default(&free_func_fastcall); // expected-error {{no matching function for call to 'cb_fptr_default'}}

  cb_fptr_cdecl(free_func_default);
  cb_fptr_cdecl(free_func_cdecl);
  cb_fptr_cdecl(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fptr_cdecl'}}
  cb_fptr_cdecl(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fptr_cdecl'}}
  cb_fptr_cdecl(&free_func_default);
  cb_fptr_cdecl(&free_func_cdecl);
  cb_fptr_cdecl(&free_func_stdcall); // expected-error {{no matching function for call to 'cb_fptr_cdecl'}}
  cb_fptr_cdecl(&free_func_fastcall); // expected-error {{no matching function for call to 'cb_fptr_cdecl'}}

  cb_fptr_stdcall(free_func_default); // expected-error {{no matching function for call to 'cb_fptr_stdcall'}}
  cb_fptr_stdcall(free_func_cdecl); // expected-error {{no matching function for call to 'cb_fptr_stdcall'}}
  cb_fptr_stdcall(free_func_stdcall);
  cb_fptr_stdcall(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fptr_stdcall'}}

  cb_fptr_fastcall(free_func_default); // expected-error {{no matching function for call to 'cb_fptr_fastcall'}}
  cb_fptr_fastcall(free_func_cdecl); // expected-error {{no matching function for call to 'cb_fptr_fastcall'}}
  cb_fptr_fastcall(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fptr_fastcall'}}
  cb_fptr_fastcall(free_func_fastcall);

  cb_fptr_const_default(free_func_default);
  cb_fptr_const_default(free_func_cdecl);
  cb_fptr_const_default(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fptr_const_default'}}
  cb_fptr_const_default(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fptr_const_default'}}

}

// Pointers to variadic functions
// variadic function can't declared stdcall or fastcall
void         free_func_variadic_default(int, ...);
void __cdecl free_func_variadic_cdecl(int, ...);

typedef void (        *fptr_variadic_default)(int, ...);
typedef void (__cdecl *fptr_variadic_cdecl)(int, ...);

void cb_fptr_variadic_default(fptr_variadic_default ptr);
void cb_fptr_variadic_cdecl(fptr_variadic_cdecl ptr);

void call_free_variadic_func() {
  cb_fptr_variadic_default(free_func_variadic_default);
  cb_fptr_variadic_default(free_func_variadic_cdecl);
  cb_fptr_variadic_default(&free_func_variadic_default);
  cb_fptr_variadic_default(&free_func_variadic_cdecl);

  cb_fptr_variadic_cdecl(free_func_variadic_default);
  cb_fptr_variadic_cdecl(free_func_variadic_cdecl);
  cb_fptr_variadic_cdecl(&free_func_variadic_default);
  cb_fptr_variadic_cdecl(&free_func_variadic_cdecl);
}

// References to functions
typedef void (           &fref_default)();
typedef void (__cdecl    &fref_cdecl)();
typedef void (__stdcall  &fref_stdcall)();
typedef void (__fastcall &fref_fastcall)();

// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'fref_default' (aka 'void (&)()') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'fref_default' (aka 'void (&)()') for 1st argument}}
void cb_fref_default(fref_default ptr);
// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'fref_cdecl' (aka 'void (&)()') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'fref_cdecl' (aka 'void (&)()') for 1st argument}}
void cb_fref_cdecl(fref_cdecl ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void ()' to 'fref_stdcall' (aka 'void (&)() __attribute__((stdcall))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((cdecl))' to 'fref_stdcall' (aka 'void (&)() __attribute__((stdcall))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((fastcall))' to 'fref_stdcall' (aka 'void (&)() __attribute__((stdcall))') for 1st argument}}
void cb_fref_stdcall(fref_stdcall ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void ()' to 'fref_fastcall' (aka 'void (&)() __attribute__((fastcall))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void () __attribute__((cdecl))' to 'fref_fastcall' (aka 'void (&)() __attribute__((fastcall))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void () __attribute__((stdcall))' to 'fref_fastcall' (aka 'void (&)() __attribute__((fastcall))') for 1st argument}}
void cb_fref_fastcall(fref_fastcall ptr);

void call_free_func_ref() {
  cb_fref_default(free_func_default);
  cb_fref_default(free_func_cdecl);
  cb_fref_default(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fref_default'}}
  cb_fref_default(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fref_default'}}

  cb_fref_cdecl(free_func_default);
  cb_fref_cdecl(free_func_cdecl);
  cb_fref_cdecl(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fref_cdecl'}}
  cb_fref_cdecl(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fref_cdecl'}}

  cb_fref_stdcall(free_func_default); // expected-error {{no matching function for call to 'cb_fref_stdcall'}}
  cb_fref_stdcall(free_func_cdecl); // expected-error {{no matching function for call to 'cb_fref_stdcall'}}
  cb_fref_stdcall(free_func_stdcall);
  cb_fref_stdcall(free_func_fastcall); // expected-error {{no matching function for call to 'cb_fref_stdcall'}}

  cb_fref_fastcall(free_func_default); // expected-error {{no matching function for call to 'cb_fref_fastcall'}}
  cb_fref_fastcall(free_func_cdecl); // expected-error {{no matching function for call to 'cb_fref_fastcall'}}
  cb_fref_fastcall(free_func_stdcall); // expected-error {{no matching function for call to 'cb_fref_fastcall'}}
  cb_fref_fastcall(free_func_fastcall);
}

// References to variadic functions
// variadic function can't declared stdcall or fastcall
typedef void (        &fref_variadic_default)(int, ...);
typedef void (__cdecl &fref_variadic_cdecl)(int, ...);

void cb_fref_variadic_default(fptr_variadic_default ptr);
void cb_fref_variadic_cdecl(fptr_variadic_cdecl ptr);

void call_free_variadic_func_ref() {
  cb_fref_variadic_default(free_func_variadic_default);
  cb_fref_variadic_default(free_func_variadic_cdecl);

  cb_fref_variadic_cdecl(free_func_variadic_default);
  cb_fref_variadic_cdecl(free_func_variadic_cdecl);
}

// Pointers to members
namespace NonVariadic {

struct A {
  void            member_default();
  void __cdecl    member_cdecl();
  void __thiscall member_thiscall();
};

struct B : public A {
};

struct C {
  void            member_default();
  void __cdecl    member_cdecl();
  void __thiscall member_thiscall();
};

typedef void (           A::*memb_a_default)();
typedef void (__cdecl    A::*memb_a_cdecl)();
typedef void (__thiscall A::*memb_a_thiscall)();
typedef void (           B::*memb_b_default)();
typedef void (__cdecl    B::*memb_b_cdecl)();
typedef void (__thiscall B::*memb_b_thiscall)();
typedef void (           C::*memb_c_default)();
typedef void (__cdecl    C::*memb_c_cdecl)();
typedef void (__thiscall C::*memb_c_thiscall)();

// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_a_default' (aka 'void (NonVariadic::A::*)() __attribute__((thiscall))') for 1st argument}}
void cb_memb_a_default(memb_a_default ptr);
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_a_cdecl' (aka 'void (NonVariadic::A::*)() __attribute__((cdecl))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_a_cdecl' (aka 'void (NonVariadic::A::*)() __attribute__((cdecl))') for 1st argument}}
void cb_memb_a_cdecl(memb_a_cdecl ptr);
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_a_thiscall' (aka 'void (NonVariadic::A::*)() __attribute__((thiscall))') for 1st argument}}
void cb_memb_a_thiscall(memb_a_thiscall ptr);
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_b_default' (aka 'void (NonVariadic::B::*)() __attribute__((thiscall))') for 1st argument}}
void cb_memb_b_default(memb_b_default ptr);
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_b_cdecl' (aka 'void (NonVariadic::B::*)() __attribute__((cdecl))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_b_cdecl' (aka 'void (NonVariadic::B::*)() __attribute__((cdecl))') for 1st argument}}
void cb_memb_b_cdecl(memb_b_cdecl ptr);
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_b_thiscall' (aka 'void (NonVariadic::B::*)() __attribute__((thiscall))') for 1st argument}}
void cb_memb_b_thiscall(memb_b_thiscall ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_c_default' (aka 'void (NonVariadic::C::*)() __attribute__((thiscall))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_c_default' (aka 'void (NonVariadic::C::*)() __attribute__((thiscall))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_c_default' (aka 'void (NonVariadic::C::*)() __attribute__((thiscall))') for 1st argument}}
void cb_memb_c_default(memb_c_default ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_c_cdecl' (aka 'void (NonVariadic::C::*)() __attribute__((cdecl))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_c_cdecl' (aka 'void (NonVariadic::C::*)() __attribute__((cdecl))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_c_cdecl' (aka 'void (NonVariadic::C::*)() __attribute__((cdecl))') for 1st argument}}
void cb_memb_c_cdecl(memb_c_cdecl ptr);
// expected-note@+3 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_c_thiscall' (aka 'void (NonVariadic::C::*)() __attribute__((thiscall))') for 1st argument}}
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((cdecl))' to 'memb_c_thiscall' (aka 'void (NonVariadic::C::*)() __attribute__((thiscall))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (NonVariadic::A::*)() __attribute__((thiscall))' to 'memb_c_thiscall' (aka 'void (NonVariadic::C::*)() __attribute__((thiscall))') for 1st argument}}
void cb_memb_c_thiscall(memb_c_thiscall ptr);

void call_member() {
  cb_memb_a_default(&A::member_default);
  cb_memb_a_default(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_a_default'}}
  cb_memb_a_default(&A::member_thiscall);

  cb_memb_a_cdecl(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_a_cdecl'}}
  cb_memb_a_cdecl(&A::member_cdecl);
  cb_memb_a_cdecl(&A::member_thiscall); // expected-error {{no matching function for call to 'cb_memb_a_cdecl'}}

  cb_memb_a_thiscall(&A::member_default);
  cb_memb_a_thiscall(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_a_thiscall'}}
  cb_memb_a_thiscall(&A::member_thiscall);
}

void call_member_inheritance() {
  cb_memb_b_default(&A::member_default);
  cb_memb_b_default(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_b_default'}}
  cb_memb_b_default(&A::member_thiscall);
  cb_memb_c_default(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_c_default'}}
  cb_memb_c_default(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_c_default'}}
  cb_memb_c_default(&A::member_thiscall); // expected-error {{no matching function for call to 'cb_memb_c_default'}}

  cb_memb_b_cdecl(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_b_cdecl'}}
  cb_memb_b_cdecl(&A::member_cdecl);
  cb_memb_b_cdecl(&A::member_thiscall); // expected-error {{no matching function for call to 'cb_memb_b_cdecl'}}
  cb_memb_c_cdecl(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_c_cdecl'}}
  cb_memb_c_cdecl(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_c_cdecl'}}
  cb_memb_c_cdecl(&A::member_thiscall); // expected-error {{no matching function for call to 'cb_memb_c_cdecl'}}

  cb_memb_b_thiscall(&A::member_default);
  cb_memb_b_thiscall(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_b_thiscall'}}
  cb_memb_b_thiscall(&A::member_thiscall);
  cb_memb_c_thiscall(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_c_thiscall'}}
  cb_memb_c_thiscall(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_c_thiscall'}}
  cb_memb_c_thiscall(&A::member_thiscall); // expected-error {{no matching function for call to 'cb_memb_c_thiscall'}}
}
} // end namespace NonVariadic

namespace Variadic {
struct A {
  void            member_default(int, ...);
  void __cdecl    member_cdecl(int, ...);
  void __thiscall member_thiscall(int, ...);
};

struct B : public A {
};

struct C {
  void            member_default(int, ...);
  void __cdecl    member_cdecl(int, ...);
};

typedef void (           A::*memb_a_default)(int, ...);
typedef void (__cdecl    A::*memb_a_cdecl)(int, ...);
typedef void (           B::*memb_b_default)(int, ...);
typedef void (__cdecl    B::*memb_b_cdecl)(int, ...);
typedef void (           C::*memb_c_default)(int, ...);
typedef void (__cdecl    C::*memb_c_cdecl)(int, ...);

void cb_memb_a_default(memb_a_default ptr);
void cb_memb_a_cdecl(memb_a_cdecl ptr);
void cb_memb_b_default(memb_b_default ptr);
void cb_memb_b_cdecl(memb_b_cdecl ptr);
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (Variadic::A::*)(int, ...)' to 'memb_c_default' (aka 'void (Variadic::C::*)(int, ...)') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (Variadic::A::*)(int, ...) __attribute__((cdecl))' to 'memb_c_default' (aka 'void (Variadic::C::*)(int, ...)') for 1st argument}}
void cb_memb_c_default(memb_c_default ptr);
// expected-note@+2 {{candidate function not viable: no known conversion from 'void (Variadic::A::*)(int, ...)' to 'memb_c_cdecl' (aka 'void (Variadic::C::*)(int, ...) __attribute__((cdecl))') for 1st argument}}
// expected-note@+1 {{candidate function not viable: no known conversion from 'void (Variadic::A::*)(int, ...) __attribute__((cdecl))' to 'memb_c_cdecl' (aka 'void (Variadic::C::*)(int, ...) __attribute__((cdecl))') for 1st argument}}
void cb_memb_c_cdecl(memb_c_cdecl ptr);

void call_member() {
  cb_memb_a_default(&A::member_default);
  cb_memb_a_default(&A::member_cdecl);

  cb_memb_a_cdecl(&A::member_default);
  cb_memb_a_cdecl(&A::member_cdecl);
}

void call_member_inheritance() {
  cb_memb_b_default(&A::member_default);
  cb_memb_b_default(&A::member_cdecl);
  cb_memb_c_default(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_c_default'}}
  cb_memb_c_default(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_c_default'}}

  cb_memb_b_cdecl(&A::member_default);
  cb_memb_b_cdecl(&A::member_cdecl);
  cb_memb_c_cdecl(&A::member_default); // expected-error {{no matching function for call to 'cb_memb_c_cdecl'}}
  cb_memb_c_cdecl(&A::member_cdecl); // expected-error {{no matching function for call to 'cb_memb_c_cdecl'}}
}
} // end namespace Variadic
