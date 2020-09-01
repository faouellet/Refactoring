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

#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <type_traits>

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
  VECTOR_OP(elems, -)                                                          \
  VECTOR_OP(elems, +)                                                          \
  VECTOR_OP(elems, *)                                                          \
  VECTOR_OP(elems, /)                                                          \
                                                                               \
  SCALAR_OP(elems, -)                                                          \
  SCALAR_OP(elems, +)                                                          \
  SCALAR_OP(elems, *)                                                          \
  SCALAR_OP(elems, /)

namespace details
{
    template <typename TVec, typename TFunc>
    inline bool compareVector(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs, TFunc&& func)
    { 
        for(int iElem = 0; iElem < 2; ++iElem)
        {
            if(!func(lhs[iElem], rhs[iElem]))
            {
                return false;
            }
        }
        return true;
    }
}

template <typename TVec> class Vec2 {
    static_assert(std::is_same_v<TVec, int> || std::is_same_v<TVec, float>);

    using TData = std::array<TVec, 2>;

public:
    // Ctor
    explicit Vec2(TVec val = TVec{}) : m_data{val, val} { }
    Vec2(TVec _x, TVec _y) : m_data{_x, _y} { }

    // Indexing
    constexpr const TVec& operator[](int n) const 
    { 
        assert(0 <= n && n < 2);
        return m_data[n]; 
    }

    // Accessors
    constexpr TVec X() const { return m_data[0]; }
    constexpr TVec Y() const { return m_data[1]; }

    constexpr TVec& X() { return m_data[0]; }
    constexpr TVec& Y() { return m_data[1]; }

    // Iterators
    constexpr typename TData::iterator begin() noexcept              { return m_data.begin(); }
    constexpr typename TData::iterator end() noexcept                { return m_data.end(); }
    constexpr typename TData::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
    constexpr typename TData::const_iterator cend() const noexcept   { return m_data.cend(); }

    // Length
    TVec sqlen() const
    { 
        return std::accumulate(m_data.cbegin(), m_data.cend(), TVec{});
    } 
    
    float len() const
    { 
        return std::sqrt(static_cast<float>(sqlen()));
    }

  OPERATORS(2)

private:
    TData m_data{};
};

// Equality operators
inline bool operator==(const Vec2<int>& lhs, const Vec2<int>& rhs)
{ 
    return details::compareVector(lhs, rhs, std::equal_to<int>{});
}
inline bool operator==(const Vec2<float>& lhs, const Vec2<float>& rhs)
{ 
    return details::compareVector(lhs, rhs, [](float lhs, float rhs){ return std::fabs(lhs - rhs) > std::numeric_limits<float>::epsilon(); } );
}
template<typename TVec> inline bool operator!=(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs) { return !(lhs == rhs); }

// Comparison operators
template<typename TVec> inline bool operator<(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs)  
{
    return details::compareVector(lhs, rhs, [](TVec lhs, TVec rhs){ return lhs < rhs; });
}
template<typename TVec> inline bool operator<=(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs) { return !(lhs > rhs); }
template<typename TVec> inline bool operator>=(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs) { return !(lhs < rhs); }
template<typename TVec> inline bool operator>(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs)  { return rhs < lhs; }


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
