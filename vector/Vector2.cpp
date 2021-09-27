/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Vector2.cpp

Abstract:

    2 dim vector module realizations.

Author / Creation date:

    JulesIMF / 07.09.21

Revision History:
	27.09.21  04:01	    Functions now inline
	22.09.21  02:39		Added operator/
    21.09.21            Vector renamed to Vector2 due to collisions with Vector3

--*/

//
// Includes / usings
//

#include <cstdio>
#include <cmath>
#include <Vector2.h>

//
// Defines
//

// Vector2::Vector2(double x, double y)
// {
//     x_ = x;
//     y_ = y;
//     recalcAngle_();
//     recalcLength_();
// }


// void Vector2::setX(double x)
// {
//     x_ = x;
//     recalcAngle_();
//     recalcLength_();
// }


// void Vector2::setY(double y)
// {
//     y_ = y;
//     recalcAngle_();
//     recalcLength_();
// }


// double Vector2::getX() const
// {
//     return x_;
// }


// double Vector2::getY() const
// {
//     return y_;
// }


// double Vector2::length() const
// {
//     return length_;
// }


// double Vector2::angle() const
// {
//     return angle_;
// }


// void Vector2::homotetia(double coef)
// {
//     x_ *= coef;
//     y_ *= coef;
//     recalcLength_();
// }


// void Vector2::rotate(double phi)
// {
//     auto x = x_ * cos(phi) - y_ * sin(phi);
//     auto y = x_ * sin(phi) + y_ * cos(phi);
//     x_ = x;
//     y_ = y;
//     recalcAngle_();
// }


// void Vector2::stretchTo(double newLength)
// {
//     homotetia(newLength / length_);
// }


// void Vector2::rotateTo(double newAngle)
// {
//     rotate(newAngle - angle_);
// }


// Vector2 Vector2::unit() const
// {
//     Vector2 unit = *this;
//     unit.stretchTo(1);
//     return unit;
// }


// Vector2 Vector2::normal() const
// {
//     return {-y_, x_};
// }


// Vector2 Vector2::projectionTo(Vector2 const& line) const
// {
//     Vector2 lineUnit = line.unit();
//     return lineUnit * (lineUnit * (*this));
// }


// void Vector2::recalcLength_()
// {
//     length_ = sqrt(x_ * x_ + y_ * y_);
// }


// void Vector2::recalcAngle_()
// {
//     angle_ = 0;
//     if(y_ < 0 || (y_ == 0 && x_ < 0))
//         angle_ += M_PI;

//     angle_ += atan(y_ / x_);
// }


// void Vector2::print(Vector2 const& Vector2)
// {
//     static int time = 0;
//     printf("%d: (%lf, %lf), abs = %lf, arg = %lf\n",
//            time++,
//            Vector2.x_, Vector2.y_,
//            Vector2.length_, Vector2.angle_);
// }


// Vector2& Vector2::operator+=(Vector2 const& second)
// {
//     x_ += second.x_;
//     y_ += second.y_;
//     recalcAngle_();
//     recalcLength_();
//     return *this;
// }


// Vector2& Vector2::operator-=(Vector2 const& second)
// {
//     x_ -= second.x_;
//     y_ -= second.y_;
//     recalcAngle_();
//     recalcLength_();
//     return *this;
// }

// Vector2& Vector2::operator*=(double coef)
// {
//     x_ *= coef;
//     y_ *= coef;
//     recalcAngle_();
//     recalcLength_();
//     return *this;
// }


// Vector2 Vector2::operator+(Vector2 const& second) const
// {
//     Vector2 first = *this;
//     first += second;
//     return first;
// }


// Vector2 Vector2::operator-(Vector2 const& second) const
// {
//     Vector2 first = *this;
//     first -= second;
//     return first;
// }

// Vector2 Vector2::operator*(double coef) const
// {
//     Vector2 first = *this;
//     first *= coef;
//     return first;
// }

// Vector2 Vector2::operator/(double coef) const
// {
//     return *this * (1.0 / coef);
// }

// double Vector2::operator*(Vector2 const& second) const
// {
//     return x_ * second.x_ + y_ * second.y_;
// }

Vector2 operator*(double coef, Vector2 const& Vector2)
{
    return Vector2 * coef;
}
