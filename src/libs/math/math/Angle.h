#pragma once

#include "Constants.h"

#include <string>


namespace ad {
namespace math {


struct Radian_tag {};
struct Degree_tag {};


// Forward declaration to define the usings, so they are available in Angle definition
template <class T_representation, class T_unitTag = Radian_tag>
class Angle;


//
// Actual units for clients use
//
template <class T_representation>
using Radian = Angle<T_representation, Radian_tag>;

template <class T_representation>
using Degree = Angle<T_representation, Degree_tag>;


//
// Angle definition
//
template <class T_representation, class T_unitTag>
class Angle
{
    template <class> struct type{};

public:
    explicit Angle(T_representation aValue) :
        mValue{aValue}
    {}

    /*implicit*/ operator Angle<T_representation, Radian_tag> () const;

    template <template <class> class TT_angle>
    TT_angle<T_representation> as() const;

    T_representation value() const
    {
        return mValue;
    }

    Angle & operator+=(const Angle aRhs);
    Angle & operator-=(const Angle aRhs);
    template <class T_factor>
    Angle & operator*=(const T_factor aFactor);
    template <class T_factor>
    Angle & operator/=(const T_factor aFactor);

private:
    T_representation mValue;
};


//
// Operations
//
#define ANGLE Angle<T_representation, T_unitTag>

template <class T_representation, class T_unitTag>
bool operator==(const ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs.value() == aRhs.value();
}

template <class T_representation, class T_unitTag>
bool operator!=(const ANGLE aLhs, const ANGLE aRhs)
{
    return !(aLhs == aRhs);
}

template <class T_representation, class T_unitTag>
bool operator<(const ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs.value() < aRhs.value();
}

template <class T_representation, class T_unitTag>
bool operator>(const ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs.value() > aRhs.value();
}

template <class T_representation, class T_unitTag>
bool operator<=(const ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs.value() <= aRhs.value();
}

template <class T_representation, class T_unitTag>
bool operator>=(const ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs.value() >= aRhs.value();
}

template <class T_representation, class T_unitTag>
ANGLE operator-(const ANGLE aAngle)
{
    return ANGLE{-aAngle.value()};
}

template <class T_representation, class T_unitTag>
ANGLE & ANGLE::operator+=(const Angle aRhs)
{
    mValue += aRhs.mValue;
    return *this;
}

template <class T_representation, class T_unitTag>
ANGLE operator+(ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs += aRhs;
}

template <class T_representation, class T_unitTag>
ANGLE & ANGLE::operator-=(const Angle aRhs)
{
    mValue -= aRhs.mValue;
    return *this;
}

template <class T_representation, class T_unitTag>
ANGLE operator-(ANGLE aLhs, const ANGLE aRhs)
{
    return aLhs -= aRhs;
}

template <class T_representation, class T_unitTag>
template <class T_factor>
ANGLE & ANGLE::operator*=(const T_factor aFactor)
{
    mValue *= aFactor;
    return *this;
}

template <class T_representation, class T_unitTag, class T_factor>
ANGLE operator*(ANGLE aLhs, const T_factor aFactor)
{
    return aLhs *= aFactor;
}

template <class T_representation, class T_unitTag, class T_factor>
ANGLE operator*(const T_factor aFactor, ANGLE aRhs)
{
    return aRhs *= aFactor;
}

template <class T_representation, class T_unitTag>
template <class T_factor>
ANGLE & ANGLE::operator/=(const T_factor aFactor)
{
    mValue /= aFactor;
    return *this;
}

template <class T_representation, class T_unitTag, class T_factor>
ANGLE operator/(ANGLE aLhs, const T_factor aFactor)
{
    return aLhs /= aFactor;
}

#undef ANGLE



//
// User-defined literals
//
Radian<double> operator"" _rad(long double aValue)
{ return Radian<double>(aValue); }

Radian<float> operator"" _radf(long double aValue)
{ return Radian<float>(static_cast<float>(aValue)); }
Radian<float> operator"" _radF(long double aValue)
{ return operator"" _radf(aValue); }

Radian<long double> operator"" _radl(long double aValue)
{ return Radian<long double>(aValue); }
Radian<long double> operator"" _radL(long double aValue)
{ return operator"" _radl(aValue); }

Radian<int> operator"" _rad(unsigned long long aValue)
{ return Radian<int>(static_cast<int>(aValue)); }


Degree<double> operator"" _deg(long double aValue)
{ return Degree<double>(aValue); }

Degree<float> operator"" _degf(long double aValue)
{ return Degree<float>(static_cast<float>(aValue)); }
Degree<float> operator"" _degF(long double aValue)
{ return operator"" _degf(aValue); }

Degree<long double> operator"" _degl(long double aValue)
{ return Degree<long double>(aValue); }
Degree<long double> operator"" _degL(long double aValue)
{ return operator"" _degl(aValue); }

Degree<int> operator"" _deg(unsigned long long aValue)
{ return Degree<int>(static_cast<int>(aValue)); }


//
// Traits
//
template <class T_angle>
class Angle_trait;

template <class T_representation>
class Angle_trait<Radian<T_representation>>
{
public:
    static constexpr const char * const suffix = "rad";
    static constexpr double radian_factor = 1.0;
};

template <class T_representation>
class Angle_trait<Degree<T_representation>>
{
public:
    static constexpr const char * const suffix = "deg";
    // Would be nice to use T_representation here to match the floating point
    // yet that would break for integral types
    static constexpr double radian_factor = 2*pi<double>/360.0;
};


//
// IO
//
template <class T_representation, class T_unitTag>
std::ostream & operator<<(std::ostream & aOs, Angle<T_representation, T_unitTag> aAngle)
{
    return aOs << aAngle.value()
               << ' '
               << Angle_trait<Angle<T_representation, T_unitTag>>::suffix;
}


//
// Implementation
//
template <class T_representation, class T_unitTag>
Angle<T_representation, T_unitTag>::operator Angle<T_representation, Radian_tag> () const
{
    return this->as<Radian>();
}

template <class T_representation, class T_unitTag>
template <template <class> class TT_angle>
TT_angle<T_representation> Angle<T_representation, T_unitTag>::as() const
{
    // Note: allows narrowing
    return TT_angle<T_representation>(static_cast<T_representation>(
        value()
        * Angle_trait<Angle>::radian_factor
        / Angle_trait<TT_angle<T_representation>>::radian_factor
    ));
}

} // namespace math
} // namespace ad