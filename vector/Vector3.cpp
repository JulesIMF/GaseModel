/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Vector3.cpp

Abstract:

    3 dim vector module realizations.

Author / Creation date:

    JulesIMF / 07.09.21

Revision History:
    21.09.21        Vector renamed to Vector3 due to collisions with Vector2
    
--*/

//
// Includes / usings
//

#include <cstdio>
#include <cmath>
#include <Vector3.h>

//
// Defines
//

Vector3::Vector3(double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
    recalcLength_();
}


void Vector3::setX(double x)
{
    x_ = x;
    recalcLength_();
}


void Vector3::setY(double y)
{
    y_ = y;
    recalcLength_();
}


void Vector3::setZ(double z)
{
    z_ = z;
    recalcLength_();
}


double Vector3::getX() const
{
    return x_;
}


double Vector3::getY() const
{
    return y_;
}


double Vector3::getZ() const
{
    return z_;
}


double Vector3::length() const
{
    return length_;
}


void Vector3::homotetia(double coef)
{
    x_ *= coef;
    y_ *= coef;
    z_ *= coef;
    recalcLength_();
}


void Vector3::stretchTo(double newLength)
{
    homotetia(newLength / length_);
}


Vector3 Vector3::projection(Vector3 axis) const
{
    axis.stretchTo(1);
    return axis * (axis * *this);
}


void Vector3::recalcLength_()
{
    length_ = sqrt(x_ * x_ + y_ * y_ + z_ * z_ );
}


void Vector3::print(Vector3 const& Vector3)
{
    static int time = 0;
    printf("%d: (%lf, %lf, %lf), abs = %lf\n",
           time++,
           Vector3.x_, Vector3.y_, Vector3.z_,
           Vector3.length_);
}


Vector3& Vector3::operator+=(Vector3 const& second)
{
    x_ += second.x_;
    y_ += second.y_;
    z_ += second.z_;
    recalcLength_();
    return *this;
}


Vector3& Vector3::operator-=(Vector3 const& second)
{
    x_ -= second.x_;
    y_ -= second.y_;
    z_ -= second.z_;
    recalcLength_();
    return *this;
}

Vector3& Vector3::operator*=(double coef)
{
    x_ *= coef;
    y_ *= coef;
    z_ *= coef;
    recalcLength_();
    return *this;
}


Vector3 Vector3::operator-() const
{
    return {-x_, -y_, -z_};
}


Vector3 Vector3::operator+(Vector3 const& second) const
{
    Vector3 first = *this;
    first += second;
    return first;
}


Vector3 Vector3::operator-(Vector3 const& second) const
{
    Vector3 first = *this;
    first -= second;
    return first;
}

Vector3 Vector3::operator*(double coef) const
{
    Vector3 first = *this;
    first *= coef;
    return first;
}

double Vector3::operator*(Vector3 const& second) const
{
    return x_ * second.x_ + y_ * second.y_ + z_ * second.z_;
}

Vector3 operator*(double coef, Vector3 const& Vector3)
{
    return Vector3 * coef;
}

double Vector3::cosBeetwen(Vector3 const& first, Vector3 const& second)
{
    return first * second / ( first.length() * second.length() );
}
