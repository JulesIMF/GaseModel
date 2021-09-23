/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    geometry.h

Abstract:

    3 dim vector module definitions.

Author / Creation date:

    JulesIMF / 07.09.21

Revision History:
    21.09.21        Vector renamed to Vector3 due to collisions with Vector2

--*/

#ifndef VECTOR3_GEOMETRY
#define VECTOR3_GEOMETRY

//
// Includes / usings
//


//
// Defines
//

class Vector3
{
public:
    Vector3(double x = 0, double y = 0, double z = 0);

    void setX(double X);
    void setY(double Y);
    void setZ(double Z);
    double getX() const;
    double getY() const;
    double getZ() const;

    double length() const;
    void homotetia(double coef);
    void stretchTo(double newLength);

    Vector3 projection(Vector3 axis) const;

    Vector3& operator+=(Vector3 const& second);
    Vector3& operator-=(Vector3 const& second);
    Vector3& operator*=(double coef);
    
    Vector3 operator+(Vector3 const& second) const;
    Vector3 operator-(Vector3 const& second) const;
    Vector3 operator-() const;
    Vector3 operator*(double coef) const;
    double operator*(Vector3 const& second) const;
    friend Vector3 operator*(double coef, Vector3 const& Vector3);

    static void print(Vector3 const& Vector3);
    static double cosBeetwen(Vector3 const& first, Vector3 const& second);

protected:
    double x_, y_, z_;
    double length_;
    void recalcLength_();
};

Vector3 operator*(double coef, Vector3 const& Vector3);

#endif // !VECTOR3_GEOMETRY