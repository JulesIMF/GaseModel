/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Manager.cpp

Abstract:

    Definition of manager, handling molecules behavior.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:
	22.09.21  04:48		Balls reflections done, energy is being normalized
	21.09.21  23:52		Walls reflections defined

--*/


//
// Includes / usings
//

#include <cmath>
#include <Manager.h>

//
// Defines
//

Manager::Manager(double boundX, double boundY) : boundX(boundX),
                                                 boundY(boundY)
{
    initEnergy = 0;
}

Manager::~Manager()
{
    for(auto molecule : gase)
    {
        delete molecule;
    }
}

size_t Manager::insert(Molecule* molecule)
{
    gase.push_back(molecule);
    initEnergy = energy();
    return gase.size() - 1;
}


size_t Manager::size() const
{
    return gase.size();
}


Molecule* Manager::remove(size_t index)
{   
    auto molecule = gase.at(index);
    gase.erase(gase.begin() + index);
}


void Manager::destroy(size_t index)
{
    delete remove(index);
}


void Manager::display(JG::Window& window) const
{   
    window.beginDrawing();

    for (auto& molecule : gase)
        molecule->display(window);
    
    window.endDrawing();
}


double Manager::energy() const
{
    double energy = 0;
    for (auto const& molecule : gase)
        energy += (molecule->momentum * molecule->momentum) / (2.0 * molecule->getMass());

    return energy;
}


void Manager::nextStep(double dt)
{
    size_t gaseSize = size();

    // Сначала необходимо обработать взаимодействия друг с другом
    for (size_t i = 0; i < gaseSize; i++)
        for(size_t j = i + 1; j < gaseSize; j++)
            reflectMolecules(gase[i], gase[j]);

    // Потом со стенами
    for (auto molecule : gase)
        reflectFromWalls(molecule);

    // Возможно, какие то специфичные вещи...

    // И наконец применить импульс и подвинуть шарики
    for (auto molecule : gase)
    {
        molecule->applyMomentum();
        molecule->resetDeltaMomentum();
        molecule->move(dt);
    }

    // С энергией фигня, поэтому мы вручню все поправим
    if (bool compensate = true)
    {
        double currentEnergy = energy();
        double coef = sqrt(initEnergy / currentEnergy);
        for (auto molecule : gase)
            molecule->momentum *= coef;
    }
}

void Manager::reflectFromWalls(Molecule* molecule)
{
    double  toWest  = molecule->position.getX() - molecule->getRadius(),
            toEast  = boundX - molecule->position.getX() - molecule->getRadius(),
            toNorth = molecule->position.getY() - molecule->getRadius(),
            toSouth = boundY - molecule->position.getY() - molecule->getRadius();

    if (toWest < 0)
    {
        molecule->position += Vector2{ -2 * toWest, 0 };
        molecule->addMomentum( {-2 * molecule->momentum.getX(), 0} );
    }
    
    if (toEast < 0)
    {
        molecule->position += Vector2{ 2 * toEast, 0 };
        molecule->addMomentum( {-2 * molecule->momentum.getX(), 0} );
    }

    if (toNorth < 0)
    {
        molecule->position += Vector2{ 0, -2 * toNorth };
        molecule->addMomentum( { 0, -2 * molecule->momentum.getY()} );
    }

    if (toSouth < 0)
    {
        molecule->position += Vector2{ 0, 2 * toSouth };
        molecule->addMomentum( { 0, -2 * molecule->momentum.getY()} );
    }
}


template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


void Manager::reflectMolecules(Molecule* first, Molecule* second)
{
    // Смотреть выкладки! (которые пока на бумаге....)

    auto p1 = first->momentum,    p2 = second->momentum;
    auto r1 = first->position,    r2 = second->position;
    auto m1 = first->getMass(),   m2 = second->getMass();
    auto R1 = first->getRadius(), R2 = second->getRadius();

    auto d = r2 - r1; // по сути отрезок между центрами
    if (d.length() > R1 + R2)
        return;

    auto v1 = p1 / m1, v2 = p2 / m2; // абс скорости
    auto v_c = (p1 + p2) / (m1 + m2); // скорость цм
    auto u1 = v1 - v_c, u2 = v2 - v_c; // скорости в исо цм

    auto R = d.unit() * (R1 + R2) * sgn(d * u1); // Получили d в момент пересечения

    auto Delta = R - d;                         // Компенсировали заезжание, но не учли,
    first->position  -= Delta * m2 / (m1 + m2); // что они должны были уже разъехаться!
    second->position += Delta * m1 / (m1 + m2); // Для этого сначала пересчитаем импульсы

    auto p_rel_projection = (u1 * m1).projectionTo(d); // проекция p1 в со цм на d
    auto reducedMass = m1 * m2 / (m1 + m2);
    auto timeSinceCollision = Delta.length() * reducedMass / p_rel_projection.length();
    
    auto dp1 = -2 * p_rel_projection,
         dp2 =  2 * p_rel_projection;

    auto collisionCompensation_1 = -1 * p_rel_projection * (timeSinceCollision / m1),
         collisionCompensation_2 =  p_rel_projection * (timeSinceCollision / m2);

    first->position  += collisionCompensation_1; // Наконец, мы учли эту компенсацию.
    second->position += collisionCompensation_2; // Пора менять импульсы!

    first->addMomentum(dp1);
    second->addMomentum(dp2);
}
