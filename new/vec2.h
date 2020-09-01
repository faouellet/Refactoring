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

#ifndef __LOL_VEC2_H__
#define __LOL_VEC2_H__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <type_traits>

namespace lol
{

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

template <typename TVec> 
class Vec2
{
    // Pourrait accepter plus de types numériques, mais cela ne faisait pas partie de l'implémentaion initiale
    static_assert(std::is_same<TVec, int>::value || std::is_same<TVec, float>::value);

    using TData = std::array<TVec, 2>;

public:
    // Ctor
    explicit Vec2(TVec val = TVec{}) : m_data{val, val} { }
    Vec2(TVec _x, TVec _y) : m_data{_x, _y} { }

    // Opérations vectorielles
    Vec2<TVec>& operator+=(const Vec2<TVec>& val) { *this = vectorOpImpl(val, std::plus<TVec>{});       return *this; }
    Vec2<TVec>& operator-=(const Vec2<TVec>& val) { *this = vectorOpImpl(val, std::minus<TVec>{});      return *this; }
    Vec2<TVec>& operator*=(const Vec2<TVec>& val) { *this = vectorOpImpl(val, std::multiplies<TVec>{}); return *this; }
    Vec2<TVec>& operator/=(const Vec2<TVec>& val) { *this = vectorOpImpl(val, std::divides<TVec>{});    return *this; }

    // Opérations scalaires
    Vec2<TVec>& operator+=(const TVec& val) { *this = scalarOpImpl(val, std::plus<TVec>{});       return *this; }
    Vec2<TVec>& operator-=(const TVec& val) { *this = scalarOpImpl(val, std::minus<TVec>{});      return *this; }
    Vec2<TVec>& operator*=(const TVec& val) { *this = scalarOpImpl(val, std::multiplies<TVec>{}); return *this; }
    Vec2<TVec>& operator/=(const TVec& val) { *this = scalarOpImpl(val, std::divides<TVec>{});    return *this; }
    
    // Indexation
    const TVec& operator[](int n) const 
    { 
        assert(0 <= n && n < 2);
        return m_data[n]; 
    }
    
    // Accesseurs
    TVec X() const { return m_data[0]; }
    TVec Y() const { return m_data[1]; }

    TVec& X() { return m_data[0]; }
    TVec& Y() { return m_data[1]; }

    // Itérateurs
    constexpr typename TData::iterator begin() noexcept              { return m_data.begin(); }
    constexpr typename TData::iterator end() noexcept                { return m_data.end(); }
    constexpr typename TData::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
    constexpr typename TData::const_iterator cend() const noexcept   { return m_data.cend(); }

    // Longueur
    TVec sqlen() const
    { 
        return std::accumulate(m_data.cbegin(), m_data.cend(), TVec{});
    } 
    
    float len() const
    { 
        return std::sqrt(static_cast<float>(sqlen()));
    }
    
private:
    template<typename TFunc>
    Vec2<TVec>& vectorOpImpl(const Vec2<TVec>& val, TFunc&& func)
    {
        std::transform(val.cbegin(), val.cend(), m_data.begin(), m_data.begin(), func);
        return *this;
    }

    template<typename TFunc>
    Vec2<TVec>& scalarOpImpl(const TVec& val, TFunc&& func)
    {
        std::transform(m_data.begin(), m_data.end(), m_data.begin(), [&func, &val](TVec elem){ return func(elem, val); });
        return *this;
    }

private:
    TData m_data{};
};

// Opérations vectorielles
template<typename TVec> inline Vec2<TVec> operator+(Vec2<TVec> lhs, const Vec2<TVec>& rhs) { lhs += rhs; return lhs; }
template<typename TVec> inline Vec2<TVec> operator-(Vec2<TVec> lhs, const Vec2<TVec>& rhs) { lhs -= rhs; return lhs; }
template<typename TVec> inline Vec2<TVec> operator*(Vec2<TVec> lhs, const Vec2<TVec>& rhs) { lhs *= rhs; return lhs; }
template<typename TVec> inline Vec2<TVec> operator/(Vec2<TVec> lhs, const Vec2<TVec>& rhs) { lhs /= rhs; return lhs; }

// Opérations scalaires
template<typename TVec> inline Vec2<TVec> operator+(Vec2<TVec> lhs, const TVec& rhs) { lhs += rhs; return lhs; }
template<typename TVec> inline Vec2<TVec> operator-(Vec2<TVec> lhs, const TVec& rhs) { lhs -= rhs; return lhs; }
template<typename TVec> inline Vec2<TVec> operator*(Vec2<TVec> lhs, const TVec& rhs) { lhs *= rhs; return lhs; }
template<typename TVec> inline Vec2<TVec> operator/(Vec2<TVec> lhs, const TVec& rhs) { lhs /= rhs; return lhs; }

// Opérations booléennes (égalité)
inline bool operator==(const Vec2<int>& lhs, const Vec2<int>& rhs)
{ 
    return details::compareVector(lhs, rhs, std::equal_to<int>{});
}
inline bool operator==(const Vec2<float>& lhs, const Vec2<float>& rhs)
{ 
    return details::compareVector(lhs, rhs, [](float lhs, float rhs){ return std::fabs(lhs - rhs) > std::numeric_limits<float>::epsilon(); } );
}
template<typename TVec> inline bool operator!=(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs) { return !(lhs == rhs); }

// Opérations booléennes (comparaison)
template<typename TVec> inline bool operator<(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs)  
{
    return details::compareVector(lhs, rhs, [](TVec lhs, TVec rhs){ return lhs < rhs; });
}
template<typename TVec> inline bool operator<=(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs) { return !(lhs > rhs); }
template<typename TVec> inline bool operator>=(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs) { return !(lhs < rhs); }
template<typename TVec> inline bool operator>(const Vec2<TVec>& lhs, const Vec2<TVec>& rhs)  { return rhs < lhs; }

} /* namespace lol */

#endif // __LOL_VEC2_H__

