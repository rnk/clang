// RUN: %clang_cc1 -fsyntax-only -verify %s

void f() { // expected-note@+1 {{declared here}}
  void *p = malloc(sizeof(int) * 10); // expected-error{{use of undeclared identifier 'malloc'}}
}

int malloc(double);
