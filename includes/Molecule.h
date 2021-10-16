/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Molecule.h

Abstract:

    Molecule struct description.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:
	27.09.21  09:00		Square and molecule states added

--*/

#ifndef MOLECULES_MOLEULES
#define MOLECULES_MOLEULES

//
// Includes / usings
//

#include <Vector2.h>
#include <JG.h>

//
// Defines
//

struct Molecule
{
    Vector2 position,
            momentum;

    int const type;

    double getRadius() { return radius; }
    double getMass()   { return mass; }

    void normalizeMass(double coef)   { mass *= coef; }
    void normalizeRadius(double coef) { radius *= coef; }


    void setParent(Molecule* newParent)
    {
        parent = newParent;
        foundSiblingNear = true;
    }

    inline void setMechanicalState(float radius,
                                   float mass,
                                   Vector2 position,
                                   Vector2 momentum)
    {
        this->radius = radius;
        this->mass = mass;
        this->position = position;
        this->momentum = momentum;
    }

    enum class MoleculeState
    {
        ON,
        OFF,
        OUT,
    };

    MoleculeState state;

    static void compareParents(Molecule* first, Molecule* second);
    static bool commonParents(Molecule* first, Molecule* second);

    virtual void reset();
    virtual void move(double dt);
    virtual void addMomentum(Vector2 dp_i);
    virtual void apply();
    virtual void display(JG::Canvas& canvas) = 0;
    virtual ~Molecule() = default;
    Molecule(int type) : type(type) {}
    Molecule* parent;

protected:
    double radius, mass;
    Vector2 dp; // delta radius
    bool foundSiblingNear;
};

using MoleculeState = Molecule::MoleculeState;

enum class MoleculeType
{
    BALL,
    SQUARE
};

int constexpr nTypesMolecules = 2;

struct Ball : public Molecule
{
    Ball(float radius,
         float mass,
         Vector2 position,
         Vector2 momentum) : Molecule((int)MoleculeType::BALL)
    {
        setMechanicalState(radius, mass, position, momentum);
    }

    virtual void display(JG::Canvas& canvas) override;
};

struct Square : public Molecule
{
    Square(float radius,
           float mass,
           Vector2 position,
           Vector2 momentum) : Molecule((int)MoleculeType::SQUARE)
    {
        setMechanicalState(radius, mass, position, momentum);
        dm = 0;
        ds = 0;
        nBalls = 1; // Надо же чтобы они были изначально, если растолкнутся
        chemicalEnergy = 0;
    }

    virtual void display(JG::Canvas& canvas) override;
    virtual void apply() override;
    virtual void reset() override;

    void addMass(double dm_i)
    {
        dm += dm_i;
    }

    void addArea(double ds_i)
    {
        ds += ds_i;
    }

    int nBalls;
    double chemicalEnergy;

protected:
    double dm; // delta mass
    double ds; // delta radius
};

#endif // !MOLECULES_MOLEULES