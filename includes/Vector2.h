/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Vector2.h

Abstract:

    2 dim vector module definitions.

Author / Creation date:

    JulesIMF / 07.09.21

Revision History:
	27.09.21  04:02		Functions now inline
	22.09.21  02:39		Added operator/
    21.09.21            Vector renamed to Vector2 due to collisions with Vector3

--*/

#ifndef VECTOR2_GEOMETRY
#define VECTOR2_GEOMETRY

//
// Includes / usings
//

#include <cmath>
#include <cstdio>

//
// Defines
//

class Vector2
{
public:
    Vector2(double x = 0, double y = 0)
    {
        x_ = x;
        y_ = y;
        recalcAngle_();
        recalcLength_();
    }

    void setX(double x)
    {
        x_ = x;
        recalcAngle_();
        recalcLength_();
    }

    void setY(double y)
    {
        y_ = y;
        recalcAngle_();
        recalcLength_();
    }

    double getX() const
    {
        return x_;
    }

    double getY() const
    {
        return y_;
    }

    double length() const
    {
        return length_;
    }

    double angle() const
    {
        return angle_;
    }

    void homotetia(double coef)
    {
        x_ *= coef;
        y_ *= coef;
        recalcLength_();
    }

    void rotate(double phi)
    {
        auto x = x_ * cos(phi) - y_ * sin(phi);
        auto y = x_ * sin(phi) + y_ * cos(phi);
        x_ = x;
        y_ = y;
        recalcAngle_();
    }

    void stretchTo(double newLength)
    {
        homotetia(newLength / length_);
    }

    void rotateTo(double newAngle)
    {
        rotate(newAngle - angle_);
    }

    Vector2 normal() const
    {
        return {-y_, x_};
    }

    Vector2 unit() const
    {
        Vector2 unit = *this;
        unit.stretchTo(1);
        return unit;
    }

    Vector2 projectionTo(Vector2 const& line) const
    {
        Vector2 lineUnit = line.unit();
        return lineUnit * (lineUnit * (*this));
    }

    Vector2& operator+=(Vector2 const& second)
    {
        x_ += second.x_;
        y_ += second.y_;
        recalcAngle_();
        recalcLength_();
        return *this;
    }

    Vector2& operator-=(Vector2 const& second)
    {
        x_ -= second.x_;
        y_ -= second.y_;
        recalcAngle_();
        recalcLength_();
        return *this;
    }

    Vector2 &operator*=(double coef)
    {
        x_ *= coef;
        y_ *= coef;
        recalcAngle_();
        recalcLength_();
        return *this;
    }

    Vector2 operator+(Vector2 const& second) const
    {
        Vector2 first = *this;
        first += second;
        return first;
    }

    Vector2 operator-(Vector2 const& second) const
    {
        Vector2 first = *this;
        first -= second;
        return first;
    }

    Vector2 operator*(double coef) const
    {
        Vector2 first = *this;
        first *= coef;
        return first;
    }

    Vector2 operator/(double coef) const
    {
        return *this * (1.0 / coef);
    }

    double operator*(Vector2 const& second) const
    {
        return x_ * second.x_ + y_ * second.y_;
    }

    friend Vector2 operator*(double coef, Vector2 const &Vector2);

    static void print(Vector2 const& Vector2)
    {
        static int time = 0;
        printf("%d: (%lf, %lf), abs = %lf, arg = %lf\n",
               time++,
               Vector2.x_, Vector2.y_,
               Vector2.length_, Vector2.angle_);
    }

protected:
    double x_, y_;
    double length_, angle_;
    void recalcLength_()
    {
        length_ = sqrt(x_ * x_ + y_ * y_);
    }

    void recalcAngle_()
    {
        angle_ = 0;
        if (y_ < 0 || (y_ == 0 && x_ < 0))
            angle_ += M_PI;

        angle_ += atan(y_ / x_);
    }

};

Vector2 operator*(double coef, Vector2 const& Vector2);

#endif // !VECTOR2_GEOMETRY