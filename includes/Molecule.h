/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Molecule.h

Abstract:

    Molecule struct description.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:

--*/

#ifndef MOLECULES_MOLEULES
#define MOLECULES_MOLEULES

//
// Includes / usings
//

#include <Vector2.h>
#include <Wrapper.h>

//
// Defines
//

struct Molecule
{
    Vector2 position,
            momentum;

    double getRadius()               { return radius; }
    double getMass()                 { return mass; }
    void resetDeltaMomentum()         { dp = {0, 0}; }

    inline void setMechanicalState(float radius,
                                   float mass,
                                   Vector2 position,
                                   Vector2 momentum)
    {
        this->radius   = radius;
        this->mass     = mass;
        this->position = position;
        this->momentum = momentum;
    }

    virtual void move(double dt);
    virtual void addMomentum(Vector2 dp_i);
    virtual void applyMomentum();
    virtual void display(JG::Window& window) = 0;
    virtual ~Molecule() = default;

protected:
    double radius, mass;
    Vector2 dp;
};

struct Ball : public Molecule
{
    Ball(float radius,
         float mass,
         Vector2 position,
         Vector2 momentum)
    {
        setMechanicalState(radius, mass, position, momentum);
    }

    virtual void display(JG::Window &window) override;
};



#endif // !MOLECULES_MOLEULES