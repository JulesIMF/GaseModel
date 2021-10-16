/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Molecules.cpp

Abstract:

    Molecule struct definitions.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:
	12.10.21  14:46		Drawing in window changed to canvas
	21.09.21  04:55		Molecule and Ball described

--*/


//
// Includes / usings
//

#include <Molecule.h>

//
// Defines
//


// +++++
// Molecule
// -----

void Molecule::move(double dt)
{
    position += (dt / mass) * momentum;
}


void Molecule::addMomentum(Vector2 dp_i)
{
    dp += dp_i;
}

void Molecule::compareParents(Molecule *first, Molecule *second)
{
    bool areSiblings = (first->parent == second->parent) && first->parent;
    first->foundSiblingNear  |= areSiblings;
    second->foundSiblingNear |= areSiblings;
}

bool Molecule::commonParents(Molecule *first, Molecule *second)
{
    bool areSiblings = (first->parent == second->parent) && first->parent;
    return areSiblings;
}

void Molecule::apply()
{
    momentum += dp;
    if (!foundSiblingNear)
        parent = nullptr;
}

void Molecule::reset()
{
    dp = {0, 0};
    foundSiblingNear = false;
}

// +++++
// Ball
// -----

void Ball::display(JG::Canvas& canvas)
{
    JG::Color ballColor = {217, 11, 172}; // Лиловый
    JG::Circle circle(position.getX() - radius, 
                      position.getY() - radius,
                      radius);

    circle.setColor(ballColor);
    circle.draw(canvas);
}


void Square::display(JG::Canvas& canvas)
{
    JG::Color squareColor = {2, 161, 196}; // Голубой
    JG::Rectangle rectangle(position.getX() - radius,
                            position.getY() - radius,
                            2 * radius,
                            2 * radius);

    rectangle.setColor(squareColor);
    rectangle.draw(canvas);
}

void Square::apply()
{
    Molecule::apply();
    mass += dm;


    // Площадь квадрата = 4 * radius, поэтому 
    // нужно делить ds на 4
    radius = sqrt(radius * radius + ds / 4);
}

void Square::reset()
{
    Molecule::reset();
    dm = 0;
    ds = 0;
}
