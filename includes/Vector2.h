/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Vector2.h

Abstract:

    2 dim vector module definitions.

Author / Creation date:

    JulesIMF / 07.09.21

Revision History:
	22.09.21  02:39		Added operator/
    21.09.21            Vector renamed to Vector2 due to collisions with Vector3

--*/

#ifndef VECTOR2_GEOMETRY
#define VECTOR2_GEOMETRY

//
// Includes / usings
//


//
// Defines
//

class Vector2
{
public:
    Vector2(double x = 0, double y = 0);

    void setX(double X);
    void setY(double Y);
    double getX() const;
    double getY() const;

    double length() const;
    double angle() const;
    void homotetia(double coef);
    void rotate(double phi);
    void stretchTo(double newLength);
    void rotateTo(double newAngle);
    Vector2 normal() const;
    Vector2 unit() const;
    Vector2 projectionTo(Vector2 const& line) const;

    Vector2& operator+=(Vector2 const& second);
    Vector2& operator-=(Vector2 const& second);
    Vector2& operator*=(double coef);
    
    Vector2 operator+(Vector2 const& second) const;
    Vector2 operator-(Vector2 const& second) const;
    Vector2 operator*(double coef) const;
    Vector2 operator/(double coef) const;
    double operator*(Vector2 const& second) const;
    friend Vector2 operator*(double coef, Vector2 const &Vector2);

    static void print(Vector2 const& Vector2);

protected:
    double x_, y_;
    double length_, angle_;
    void recalcLength_();
    void recalcAngle_();
};

Vector2 operator*(double coef, Vector2 const& Vector2);

#endif // !VECTOR2_GEOMETRY