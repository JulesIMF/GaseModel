/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Manager.cpp

Abstract:

    Definition of manager, handling molecules behavior.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:
	29.09.21  11:38		Physics functions taken out to physics.cpp
	27.09.21  11:07		Chemistry of square-square
	27.09.21  08:59		Chemistry of ball-ball and ball-molecule added
	22.09.21  04:48		Balls reflections done, energy is being normalized
	21.09.21  23:52		Walls reflections defined

--*/


//
// Includes / usings
//

#include <cassert>
#include <cmath>
#include <Manager.h>

// 
// Defines
//


// --------------------------------------------------------------


Manager::Manager(double boundX, double boundY) : boundX(boundX),
                                                 boundY(boundY)
{
    initEnergy = 0;

    vTable[0][0] = &Manager::interactBallBall;          vTable[0][1] = &Manager::interactBallSquare;
    vTable[1][0] = &Manager::interactSquareBall;        vTable[1][1] = &Manager::interactSquareSquare;

    for (int i = 0; i != nTypesMolecules; i++)
        for (int j = 0; j != nTypesMolecules; j++)
            minEnergy[i][j] = INFINITY;
}


Manager::~Manager()
{
    for(auto molecule : gas)
    {
        delete molecule;
    }
}


size_t Manager::insert(Molecule* molecule)
{
    gas.push_back(molecule);
    molecule->state = MoleculeState::OFF;
    recalcInit();
    return gas.size() - 1;
}


size_t Manager::size() const
{
    return gas.size();
}


Molecule* Manager::remove(size_t index)
{   
    auto molecule = gas.at(index);
    gas.erase(gas.begin() + index);
    recalcInit();
    return molecule;
}


void Manager::destroy(size_t index)
{
    delete remove(index);
}


void Manager::display(JG::Canvas& canvas) const
{

    for (auto& molecule : gas)
        molecule->display(canvas);
    
    canvas.flush();
}


void Manager::updateStates()
{
    int currentSize = 0;

    for (int i = 0; i < (currentSize = gas.size()); )
    {
        if (gas[i]->state == MoleculeState::OUT)
        {
            Molecule* deletedMolecule = gas[i];
            std::swap(gas[i], gas[currentSize - 1]);
            gas.erase(gas.end() - 1);
            delete deletedMolecule;
        }

        else
        {
            gas[i]->state = MoleculeState::ON;
            i++;
        }
    }
}


double Manager::totalArea() const
{
    double area = 0;
    for (auto molecule : gas)
    {
        if (molecule->type == (int)MoleculeType::BALL)
            area += M_PI * molecule->getRadius() * molecule->getRadius();
        else
            area += 4 * molecule->getRadius() * molecule->getRadius();
    }

    return area;
}


double Manager::totalMass() const
{
    double mass = 0;
    for (auto molecule : gas)
        mass += molecule->getMass();

    return mass;
}


void Manager::callVirtual(Molecule *first, Molecule *second)
{
    InteractionFunction fncPtr = vTable[((int)first->Molecule::type)][((int)second->Molecule::type)];
    (this->*fncPtr)(first, second);
}


void Manager::setMinEnergy(MoleculeType first, MoleculeType second, double newMinEnergy)
{
    minEnergy[(int)first][(int)second] = 
    minEnergy[(int)second][(int)first] = newMinEnergy;
}


void Manager::nextStep(double dt)
{
    size_t gaseSize = size();
    countBalls();

    // Сначала необходимо обработать взаимодействия друг с другом
    for (size_t i = 0; i < gaseSize; i++)
        for (size_t j = i + 1; j < gaseSize; j++)
            if (gas[i]->state == MoleculeState::ON &&
                gas[j]->state == MoleculeState::ON)
            {    
                callVirtual(gas[i], gas[j]);
            }

    // Потом со стенами
    for (auto molecule : gas)
        reflectFromWalls(molecule);

    // Возможно, какие то специфичные вещи...

    // И наконец применить импульс и подвинуть шарики
    for (auto molecule : gas)
    {
        molecule->apply();
        molecule->reset();
        molecule->move(dt);
    }

    // Уберем ненужные молекулы и добавим нужные
    updateStates();

    // Нормализуем
    normalize();
}

void Manager::recalcInit()
{
    initEnergy = totalEnergy();
    initMass   = totalMass();
    initArea   = totalArea();
}

void Manager::normalize()
{
    // С энергией фигня, поэтому мы вручню все поправим
    double currentEnergy = totalEnergy();
    assert(currentEnergy > 0);

    double currentArea   = totalArea();
    double currentMass   = totalMass();

    double energyCoef    = initEnergy / currentEnergy;
    double momentumCoef  = sqrt(energyCoef);
    double radiusCoef    = sqrt(initArea / currentArea);
    double massCoef      = initMass / currentMass;

    for (auto molecule : gas)
    {
        molecule->momentum *= momentumCoef;
        if (molecule->type == (int)MoleculeType::SQUARE)
            reinterpret_cast<Square *>(molecule)->chemicalEnergy *= energyCoef;

        molecule->normalizeMass(massCoef);
        molecule->normalizeRadius(radiusCoef);
    }
}