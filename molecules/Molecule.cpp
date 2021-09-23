/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Molecules.cpp

Abstract:

    Molecule struct definitions.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:
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


void Molecule::applyMomentum()
{
    momentum += dp;
}

// +++++
// Ball
// -----

void Ball::display(JG::Window& window)
{
    JG::Color ballColor = {217, 11, 172}; // Лиловый
    JG::Circle circle(position.getX() - radius, 
                      position.getY() - radius,
                      radius);

    circle.setColor(ballColor);
    circle.draw(window);
}
