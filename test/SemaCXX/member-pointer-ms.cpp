// RUN: %clang_cc1 -std=c++11 -cxx-abi microsoft -fms-compatibility -fsyntax-only -verify %s

// Test that we don't allow reinterpret_casts from pointers of one size to
// pointers of a different size.
struct A {};
struct B {};
struct C: A, B {};

void (A::*paf)();
void (C::*pcf)() = reinterpret_cast<void (C::*)()>(paf); //expected-error{{cannot reinterpret_cast from member pointer type}}

class __single_inheritance D;
class __multiple_inheritance D; // expected-warning {{ignored since inheritance model was already declared as 'single'}}

class __virtual_inheritance E;
class __virtual_inheritance E;  // no warning expected since same attribute

// http://llvm.org/PR12070
struct Foo {
  typedef int Foo::*FooInt;
  int f;
};

// Test the size of all possible member pointer combinations:
// - complete and incomplete
// - single, multiple, and virtual inheritance (and unspecified for incomplete)
// - data and function pointers
// - templated with declared specializations with annotations
// - template that can be instantiated

// incomplete types
class __single_inheritance IncSingle;
class __multiple_inheritance IncMultiple;
class __virtual_inheritance IncVirtual;

// incomplete data
static_assert(sizeof(int IncSingle::*)   == 1 * sizeof(void *), "");
static_assert(sizeof(int IncMultiple::*) == 1 * sizeof(void *), "");
static_assert(sizeof(int IncVirtual::*)  == 2 * sizeof(void *), "");

// incomplete function
static_assert(sizeof(void (IncSingle::*)())   == 1 * sizeof(void *), "");
static_assert(sizeof(void (IncMultiple::*)()) == 2 * sizeof(void *), "");
static_assert(sizeof(void (IncVirtual::*)())  == 3 * sizeof(void *), "");

// An incomplete type with an unspecified inheritance model seems to take one
// more slot than virtual.  It's not clear what it's used for yet.
class IncUnspecified;
static_assert(sizeof(int IncUnspecified::*) == 3 * sizeof(void *), "");
static_assert(sizeof(void (IncUnspecified::*)()) == 4 * sizeof(void *), "");

// complete types
struct B1 { };
struct B2 { };
struct Single { };
struct Multiple : B1, B2 { };
struct Virtual : virtual B1 { };

// complete data
static_assert(sizeof(int Single::*)   == 1 * sizeof(void *), "");
static_assert(sizeof(int Multiple::*) == 1 * sizeof(void *), "");
static_assert(sizeof(int Virtual::*)  == 2 * sizeof(void *), "");

// complete function
static_assert(sizeof(void (Single::*)())   == 1 * sizeof(void *), "");
static_assert(sizeof(void (Multiple::*)()) == 2 * sizeof(void *), "");
static_assert(sizeof(void (Virtual::*)())  == 3 * sizeof(void *), "");

// Test both declared and defined templates.
template <typename T> class X;
template <> class __single_inheritance   X<IncSingle>;
template <> class __multiple_inheritance X<IncMultiple>;
template <> class __virtual_inheritance  X<IncVirtual>;
// Don't declare X<IncUnspecified>.
static_assert(sizeof(int X<IncSingle>::*)           == 1 * sizeof(void *), "");
static_assert(sizeof(int X<IncMultiple>::*)         == 1 * sizeof(void *), "");
static_assert(sizeof(int X<IncVirtual>::*)          == 2 * sizeof(void *), "");
static_assert(sizeof(int X<IncUnspecified>::*)      == 3 * sizeof(void *), "");
static_assert(sizeof(void (X<IncSingle>::*)())      == 1 * sizeof(void *), "");
static_assert(sizeof(void (X<IncMultiple>::*)())    == 2 * sizeof(void *), "");
static_assert(sizeof(void (X<IncVirtual>::*)())     == 3 * sizeof(void *), "");
static_assert(sizeof(void (X<IncUnspecified>::*)()) == 4 * sizeof(void *), "");

template <typename T>
struct Y : T { };
static_assert(sizeof(int Y<Single>::*)        == 1 * sizeof(void *), "");
static_assert(sizeof(int Y<Multiple>::*)      == 1 * sizeof(void *), "");
static_assert(sizeof(int Y<Virtual>::*)       == 2 * sizeof(void *), "");
static_assert(sizeof(void (Y<Single>::*)())   == 1 * sizeof(void *), "");
static_assert(sizeof(void (Y<Multiple>::*)()) == 2 * sizeof(void *), "");
static_assert(sizeof(void (Y<Virtual>::*)())  == 3 * sizeof(void *), "");
