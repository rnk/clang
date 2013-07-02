// RUN: %clang_cc1 -triple i686-pc-win32 -cxx-abi microsoft -fms-extensions -verify %s

// Pointers to free functions
void            free_func_default();
void __cdecl    free_func_cdecl();
void __stdcall  free_func_stdcall(); // expected-note {{previous declaration is here}}
void __fastcall free_func_fastcall(); // expected-note 2 {{previous declaration is here}}

void __cdecl    free_func_default(); // expected-note 2 {{previous declaration is here}}
void __stdcall  free_func_default(); // expected-error {{function declared 'stdcall' here was previously declared without calling convention}}
void __fastcall free_func_default(); // expected-error {{function declared 'fastcall' here was previously declared without calling convention}}

void            free_func_cdecl(); // expected-note 2 {{previous declaration is here}}
void __stdcall  free_func_cdecl(); // expected-error {{function declared 'stdcall' here was previously declared 'cdecl'}}
void __fastcall free_func_cdecl(); // expected-error {{function declared 'fastcall' here was previously declared 'cdecl'}}

void __cdecl    free_func_stdcall(); // expected-error {{function declared 'cdecl' here was previously declared 'stdcall'}}
void            free_func_stdcall(); // expected-note {{previous declaration is here}}
void __fastcall free_func_stdcall(); // expected-error {{function declared 'fastcall' here was previously declared 'stdcall'}}

void __cdecl    free_func_fastcall(); // expected-error {{function declared 'cdecl' here was previously declared 'fastcall'}}
void __stdcall  free_func_fastcall(); // expected-error {{function declared 'stdcall' here was previously declared 'fastcall'}}
void            free_func_fastcall();

// Overloaded functions may have different calling conventions
void __fastcall free_func_default(int);
void __cdecl    free_func_default(int *);

void __stdcall  free_func_cdecl(char *);
void __cdecl    free_func_cdecl(double);


// Pointers to member functions
struct S {
  void            member_default1(); // expected-note {{previous declaration is here}}
  void            member_default2();
  void __cdecl    member_cdecl1();
  void __cdecl    member_cdecl2(); // expected-note {{previous declaration is here}}
  void __thiscall member_thiscall1();
  void __thiscall member_thiscall2(); // expected-note {{previous declaration is here}}
  
  // Static member functions can't be __thiscall
  static void            static_member_default1();
  static void            static_member_default2(); // expected-note {{previous declaration is here}}
  static void __cdecl    static_member_cdecl1();
  static void __cdecl    static_member_cdecl2(); // expected-note {{previous declaration is here}}
  static void __stdcall  static_member_stdcall1();
  static void __stdcall  static_member_stdcall2();

  // Variadic functions can't be other than default or __cdecl
  void            member_variadic_default(int x, ...);
  void __cdecl    member_variadic_cdecl(int x, ...);

  static void            static_member_variadic_default(int x, ...);
  static void __cdecl    static_member_variadic_cdecl(int x, ...);
};

void __cdecl    S::member_default1() {} // expected-error {{function declared 'cdecl' here was previously declared without calling convention}}
void __thiscall S::member_default2() {}

void            S::member_cdecl1() {}
void __thiscall S::member_cdecl2() {} // expected-error {{function declared 'thiscall' here was previously declared 'cdecl'}}

void            S::member_thiscall1() {}
void __cdecl    S::member_thiscall2() {} // expected-error {{function declared 'cdecl' here was previously declared 'thiscall'}}

void __cdecl    S::static_member_default1() {}
void __stdcall  S::static_member_default2() {} // expected-error {{function declared 'stdcall' here was previously declared without calling convention}}

void            S::static_member_cdecl1() {}
void __stdcall  S::static_member_cdecl2() {} // expected-error {{function declared 'stdcall' here was previously declared 'cdecl'}}

void __cdecl    S::member_variadic_default(int x, ...) {
  (void)x;
}
void            S::member_variadic_cdecl(int x, ...) {
  (void)x;
}

void __cdecl    S::static_member_variadic_default(int x, ...) {
  (void)x;
}
void            S::static_member_variadic_cdecl(int x, ...) {
  (void)x;
}

// Illegal calling conventions in free functions.
void            free_f_default();
void __cdecl    free_f_cdecl();
void __stdcall  free_f_stdcall();
void __thiscall free_f_thiscall(); // expected-error {{non-member function cannot use thiscall calling convention}}
void __fastcall free_f_fastcall();

void            variadic_f_default(int, ...);
void __cdecl    variadic_f_cdecl(int, ...);
void __stdcall  variadic_f_stdcall(int, ...); // expected-error {{variadic non-member function cannot use stdcall calling convention}}
void __thiscall variadic_f_thiscall(int, ...); // expected-error {{variadic non-member function cannot use thiscall calling convention}}
void __fastcall variadic_f_fastcall(int, ...); // expected-error {{variadic function cannot use fastcall calling convention}}

// Illegal calling conventions in member functions.
class C {
public:
  void            member_default();
  void __cdecl    member_cdecl();
  void __stdcall  member_stdcall();
  void __thiscall member_thiscall();
  void __fastcall member_fastcall();

  void            member_variadic_default(int, ...);
  void __cdecl    member_variadic_cdecl(int, ...);
  void __stdcall  member_variadic_stdcall(int, ...);// expected-error {{variadic member function cannot use stdcall calling convention}}
  void __thiscall member_variadic_thiscall(int, ...); // expected-error {{variadic member function cannot use thiscall calling convention}}
  void __fastcall member_variadic_fastcall(int, ...); // expected-error {{variadic function cannot use fastcall calling convention}}

  static void            static_default();
  static void __cdecl    static_cdecl();
  static void __stdcall  static_stdcall();
  static void __thiscall static_thiscall(); // expected-error {{static member function cannot use thiscall calling convention}}
  static void __fastcall static_fastcall();

  static void            static_variadic_default(int, ...);
  static void __cdecl    static_variadic_cdecl(int, ...);
  static void __stdcall  static_variadic_stdcall(int, ...);// expected-error {{variadic static member function cannot use stdcall calling convention}}
  static void __thiscall static_variadic_thiscall(int, ...); // expected-error {{variadic static member function cannot use thiscall calling convention}}
  static void __fastcall static_variadic_fastcall(int, ...); // expected-error {{variadic function cannot use fastcall calling convention}}
};

// Illegal calling conventions in pointer-to-function declarations.
void (__cdecl    *cdecl_ptr)();
void (__stdcall  *stdcall_ptr)();
void (__thiscall *thiscall_ptr)(); // expected-error {{non-member function cannot use thiscall calling convention}}
void (__fastcall *fastcall_ptr)();

void (__cdecl    *cdecl_variadic_ptr)(int, ...);
void (__stdcall  *stdcall_variadic_ptr)(int, ...); // expected-error {{variadic non-member function cannot use stdcall calling convention}}
void (__thiscall *thiscall_variadic_ptr)(int, ...); // expected-error {{variadic non-member function cannot use thiscall calling convention}}
void (__fastcall *fastcall_variadic_ptr)(int, ...); // expected-error {{variadic function cannot use fastcall calling convention}}

// Illegal calling convention in reference-to-function declarations.
void (__cdecl    &cdecl_ref)() = free_f_cdecl;
void (__stdcall  &stdcall_ref)() = free_f_stdcall;
void (__thiscall &thiscall_ref)() = free_f_thiscall; // expected-error {{non-member function cannot use thiscall calling convention}}
void (__fastcall &fastcall_ref)() = free_f_fastcall;

void (__cdecl    &cdecl_variadic_ref)(int, ...) = variadic_f_cdecl;
void (__stdcall  &stdcall_variadic_ref)(int, ...) = variadic_f_stdcall; // expected-error {{variadic non-member function cannot use stdcall calling convention}}
void (__thiscall &thiscall_variadic_ref)(int, ...) = variadic_f_thiscall; // expected-error {{variadic non-member function cannot use thiscall calling convention}}
void (__fastcall &fastcall_variadic_ref)(int, ...) = variadic_f_fastcall; // expected-error {{variadic function cannot use fastcall calling convention}}

// Illegal calling convention in pointer-to-member-function declarations.
void (__cdecl    C::*cdecl_mem_ptr)();
void (__stdcall  C::*stdcall_mem_ptr)();
void (__thiscall C::*thiscall_mem_ptr)();
void (__fastcall C::*fastcall_mem_ptr)();

void (__cdecl    C::*cdecl_variadic_mem_ptr)(int, ...);
void (__stdcall  C::*stdcall_variadic_mem_ptr)(int, ...); // expected-error {{variadic member function cannot use stdcall calling convention}}
void (__thiscall C::*thiscall_variadic_mem_ptr)(int, ...); // expected-error {{variadic member function cannot use thiscall calling convention}}
void (__fastcall C::*fastcall_variadic_mem_ptr)(int, ...); // expected-error {{variadic function cannot use fastcall calling convention}}

// Illegal calling convention in function arguments.
void cb1( void (__stdcall  *)() );
void cb2( void (__thiscall *)() ); // expected-error {{non-member function cannot use thiscall calling convention}}
void cb3( void (__stdcall  *)(int, ...) ); // expected-error {{variadic non-member function cannot use stdcall calling convention}}
void cb4( void (__fastcall C::*)() );
void cb5( void (__fastcall C::*)(int, ...) ); // expected-error {{variadic function cannot use fastcall calling convention}}
void cb6( void (__cdecl    &)() );
void cb7( void (__thiscall &)() ); // expected-error {{non-member function cannot use thiscall calling convention}}

// Illegal calling convention in typedefs.
typedef void (__stdcall  *stdcall_t)();
typedef void (__thiscall *thiscall_t)(); // expected-error {{non-member function cannot use thiscall calling convention}}
typedef void (__thiscall C::*thiscall_mem_t)();
typedef void (__stdcall  *stdcall_variadic_t)(int, ...); // expected-error {{variadic non-member function cannot use stdcall calling convention}}
typedef void (__thiscall C::*thiscall_variadic_mem_t)(int, ...); // expected-error {{variadic member function cannot use thiscall calling convention}}

// Illegal calling convention in templates.
template <typename T>
void __thiscall temp_thiscall(T t) {} // expected-error {{non-member function cannot use thiscall calling convention}}

template <typename T>
void __stdcall temp_variadic_stdcall(T t, ...) {} // expected-error {{variadic non-member function cannot use stdcall calling convention}}

template <typename T>
struct CT {
  void __stdcall temp_memb_variadic_stdcall(T t, ...); // expected-error {{variadic member function cannot use stdcall calling convention}}
  static void __thiscall temp_static_memb_thiscall(T t); // expected-error {{static member function cannot use thiscall calling convention}}

  template <typename U>
  void __stdcall temp_temp_memb_variadic_stdcall(U u, T t, ...); // expected-error {{variadic member function cannot use stdcall calling convention}}
};

// Pointers to function within other declarations.
// expected-error@+2 {{non-member function cannot use thiscall calling convention}}
// expected-error@+1 {{variadic member function cannot use thiscall calling convention}}
typedef void (__thiscall *(__thiscall C::*ret_method_ret_func1())(int, ...))(int);

// expected-error@+1 {{variadic member function cannot use thiscall calling convention}}
typedef void (*(__thiscall C::*ret_method_ret_func2())(int, ...))(int);

// expected-error@+1 {{variadic member function cannot use thiscall calling convention}}
void accept(void (*)(), void (__thiscall C::*(__thiscall C::*)())(int, ...), void (*)());

// Redeclaration for function declared in namespaces.
namespace N {
  void foo(int, ...); // expected-note {{previous declaration is here}}
}

void __thiscall N::foo(int, ...) {} // expected-error {{function declared 'thiscall' here was previously declared without calling convention}}

// Correctly process class defintion.
struct TheFriend {
  void __thiscall foo();
  void bar(int, ...); // expected-note {{previous declaration is here}}
};

void theFriend();

struct Friendly {
  // Fields and typedefs that are pointers to funciton or method.
  void (__thiscall *fptr)(); // expected-error {{non-member function cannot use thiscall calling convention}}
  void (__thiscall TheFriend::*mptr)();
  typedef void (__stdcall *td)(int, ...); // expected-error {{variadic non-member function cannot use stdcall calling convention}}

  // Friend declarations.
  friend void __thiscall TheFriend::foo();
  friend void __thiscall TheFriend::bar(int, ...); // expected-error {{function declared 'thiscall' here was previously declared without calling convention}}
  friend void __thiscall theFriend(); // expected-error {{non-member function cannot use thiscall calling convention}}
};

// Don't fail on nested parentheses.
typedef void ((((__stdcall (*((ptr)))))()));
typedef void ((((__thiscall (*((fail)))))())); // expected-error {{non-member function cannot use thiscall calling convention}}

void cc_overloads(void (__cdecl   *fn)()) { }
void cc_overloads(void (__stdcall *fn)()) { }
// no __fastcall
void use_overload() {
  cc_overloads(free_f_default);
  cc_overloads(free_f_stdcall);
  cc_overloads(free_f_fastcall); // expected-error {{asdf}}
}
