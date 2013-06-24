// RUN: %clang_cc1 -fsyntax-only -verify -Wno-objc-root-class -Wshadow %s

@interface FieldShadow
-(void) my_method: (int)field;
@end

@implementation FieldShadow {
  int field; // expected-note {{previous declaration is here}}
}
- (void) my_method: (int)field { // expected-warning {{declaration shadows a field of 'FieldShadow'}}
}
@end
