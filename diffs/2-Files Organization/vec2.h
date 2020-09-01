//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Matrix classes
// ------------------
//

#if !defined __LOL_MATRIX_H__
#define __LOL_MATRIX_H__

#include <cmath>

#include "vec2fwd.h"

namespace lol {

#define VECTOR_OP(elems, op)                                                   \
  template <typename U>                                                        \
  inline Vec##elems<T> operator op(Vec##elems<U> const &val) const {           \
    Vec##elems<T> ret;                                                         \
    for (int n = 0; n < elems; n++)                                            \
      ret[n] = (*this)[n] op val[n];                                           \
    return ret;                                                                \
  }                                                                            \
                                                                               \
  template <typename U>                                                        \
  inline Vec##elems<T> operator op##=(Vec##elems<U> const &val) {              \
    return *this = (*this)op val;                                              \
  }

#define BOOL_OP(elems, op, op2, ret)                                           \
  inline bool operator op(Vec##elems<T> const &val) const {                    \
    for (int n = 0; n < elems; n++)                                            \
      if (!((*this)[n] op2 val[n]))                                            \
        return !ret;                                                           \
    return ret;                                                                \
  }

#define SCALAR_OP(elems, op)                                                   \
  inline Vec##elems<T> operator op(T const &val) const {                       \
    Vec##elems<T> ret;                                                         \
    for (int n = 0; n < elems; n++)                                            \
      ret[n] = (*this)[n] op val;                                              \
    return ret;                                                                \
  }                                                                            \
                                                                               \
  inline Vec##elems<T> operator op##=(T const &val) {                          \
    return *this = (*this)op val;                                              \
  }

#define OPERATORS(elems)                                                       \
  inline T &operator[](int n) { return *(&x + n); }                            \
  inline T const &operator[](int n) const { return *(&x + n); }                \
                                                                               \
  VECTOR_OP(elems, -)                                                          \
  VECTOR_OP(elems, +)                                                          \
  VECTOR_OP(elems, *)                                                          \
  VECTOR_OP(elems, /)                                                          \
                                                                               \
  BOOL_OP(elems, ==, ==, true)                                                 \
  BOOL_OP(elems, !=, ==, false)                                                \
  BOOL_OP(elems, <=, <=, true)                                                 \
  BOOL_OP(elems, >=, >=, true)                                                 \
  BOOL_OP(elems, <, <, true)                                                   \
  BOOL_OP(elems, >, >, true)                                                   \
                                                                               \
  SCALAR_OP(elems, -)                                                          \
  SCALAR_OP(elems, +)                                                          \
  SCALAR_OP(elems, *)                                                          \
  SCALAR_OP(elems, /)                                                          \
                                                                               \
  inline T sqlen() const {                                                     \
    T acc = 0;                                                                 \
    for (int n = 0; n < elems; n++)                                            \
      acc += (*this)[n] * (*this)[n];                                          \
    return acc;                                                                \
  }                                                                            \
                                                                               \
  inline float len() const {                                                   \
    using namespace std;                                                       \
    return sqrtf((float)sqlen());                                              \
  }

template <typename T> struct Vec2 {
  inline Vec2() {}
  inline Vec2(T val) { x = y = val; }
  inline Vec2(T _x, T _y) {
    x = _x;
    y = _y;
  }

  OPERATORS(2)

  T x;
  T y;
};

#define SCALAR_GLOBAL(elems, op, U)                                            \
  template <typename T>                                                        \
  static inline Vec##elems<U> operator op(U const &val,                        \
                                          Vec##elems<T> const &that) {         \
    Vec##elems<U> ret;                                                         \
    for (int n = 0; n < elems; n++)                                            \
      ret[n] = val op that[n];                                                 \
    return ret;                                                                \
  }

#define SCALAR_GLOBAL2(elems, op)                                              \
  SCALAR_GLOBAL(elems, op, int)                                                \
  SCALAR_GLOBAL(elems, op, float)

#define GLOBALS(elems)                                                         \
  SCALAR_GLOBAL2(elems, -)                                                     \
  SCALAR_GLOBAL2(elems, +)                                                     \
  SCALAR_GLOBAL2(elems, *)                                                     \
  SCALAR_GLOBAL2(elems, /)

GLOBALS(2)

} /* namespace lol */

#endif // __LOL_MATRIX_H__
