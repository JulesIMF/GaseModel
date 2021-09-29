/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    physics.cpp

Abstract:

    Physics functions of manager

Author / Creation date:

    JulesIMF / 29.09.21

Revision History:
	29.09.21  11:36		Fixed problems with area calculations

--*/


//
// Includes / usings
//

#include <cmath>
#include <cassert>
#include <Manager.h>


//
// Defines
//

//-----------------------------------------------------//
//---------------Вспомогательные функции---------------//
//-----------------------------------------------------//

inline Vector2 velocity(Molecule *molecule)
{
    return molecule->momentum / molecule->getMass();
}

inline Vector2 massCenter(Molecule *first, Molecule *second)
{
    return (first->position * first->getMass() + second->position * second->getMass()) /
           (first->getMass() + second->getMass());
}

inline Vector2 velocityCM(Molecule *first, Molecule *second)
{
    return (first->momentum + second->momentum) /
           (first->getMass() + second->getMass());
}

inline double kineticEnergy(Molecule *molecule)
{
    return molecule->momentum * molecule->momentum / (2 * molecule->getMass());
}

inline double kineticEnergyInCM(Molecule *first, Molecule *second)
{
    Vector2 v_cm = velocityCM(first, second);
    double v1 = (velocity(first) - v_cm).length();
    double v2 = (velocity(second) - v_cm).length();

    return v1 * v1 * first->getMass() / 2 +
           v2 * v2 * second->getMass() / 2;
}

inline bool farFromEachOther(Molecule *first, Molecule *second)
{
    return (first->position - second->position).length() >
           first->getRadius() + second->getRadius();
}

void reflectMolecules(Molecule *first, Molecule *second);

//-----------------------------------------------------//
//---------------Функции взаимодействия----------------//
//-----------------------------------------------------//

bool Manager::underMinEnergy(Molecule *first, Molecule *second)
{
    return kineticEnergyInCM(first, second) < minEnergy[first->type][second->type];
}

void Manager::interactBallBall(Molecule *first, Molecule *second)
{
    if (farFromEachOther(first, second))
    {
        // если они разъехались далеко, то они могут
        // уже забыть наконец друг про друга
        return;
    }

    Molecule::compareParents(first, second);

    if (Molecule::commonParents(first, second))
        return;

    if (underMinEnergy(first, second))
    {
        reflectMolecules(first, second);
        return;
    }

    double newRadius = sqrt((first->getRadius() * first->getRadius() +
                            second->getRadius() * second->getRadius()) *
                            (M_PI / 4)); // Хотим сохранить площадь

    auto newSquare = new Square(newRadius,
                                first->getMass() + second->getMass(),
                                massCenter(first, second),
                                first->momentum + second->momentum);

    newSquare->nBalls = 2;
    newSquare->chemicalEnergy += kineticEnergyInCM(first, second);

    newSquare->state = MoleculeState::OFF;
    first->state = second->state = MoleculeState::OUT;
    gas.push_back(newSquare);

    // Т к на каждом шаге кин энергия нормализуется,
    // надо делать вот так, потому что тут кин энергия пропадает
    // initEnergy += kineticEnergy(newSquare);
    // initEnergy -= kineticEnergy(first);
    // initEnergy -= kineticEnergy(second);
}

void Manager::interactSquareSquare(Molecule *first, Molecule *second)
{
    if (farFromEachOther(first, second))
        return;

    if (underMinEnergy(first, second))
    {
        reflectMolecules(first, second);
        return;
    }

    auto center = massCenter(first, second);
    auto momentum = first->momentum + second->momentum;

    auto firstSquare = reinterpret_cast<Square *>(first);
    auto secondSquare = reinterpret_cast<Square *>(second);

    auto totalNBalls = firstSquare->nBalls +
                       secondSquare->nBalls;

    auto totalMass = first->getMass() + second->getMass();

    auto ballMass = totalMass / totalNBalls;

    auto freedKineticEnergy = firstSquare->chemicalEnergy +
                              secondSquare->chemicalEnergy;

    auto ballEnergy = (kineticEnergyInCM(first, second) + freedKineticEnergy) / totalNBalls;

    auto ballArea = 4 * (first->getRadius() * first->getRadius() + second->getRadius() * second->getRadius()) / totalNBalls;

    auto ballRadius = sqrt(ballArea / M_PI);

    // K = p^2 / 2m => p = sqrt(2Km)
    Vector2 momentumInCM = Vector2{1, 0} * sqrt(2 * ballEnergy * ballMass);
    Vector2 translationMomentum = momentum / totalNBalls;
    double angle = 2 * M_PI / totalNBalls;

    for (int i = 0; i != totalNBalls; i++)
    {
        Ball *newBall = new Ball(ballRadius,
                                 ballMass,
                                 center,
                                 momentumInCM + translationMomentum);

        newBall->state = MoleculeState::OFF;
        newBall->setParent(first); // подойдет любой из родителей

        gas.push_back(newBall);
        momentumInCM.rotate(angle);
    }

    first->state = second->state = MoleculeState::OUT;

    // initEnergy += freedKineticEnergy;
    // Вроде по т Кенига ничего не должно меняться в плане кин энергии
}

void Manager::interactBallSquare(Molecule *ball, Molecule *square)
{
    if (farFromEachOther(ball, square))
        return;

    if (underMinEnergy(ball, square))
    {
        reflectMolecules(ball, square);
        return;
    }

    double oldSquareKineticEnergy = kineticEnergy(square);
    double ballArea = M_PI * ball->getRadius() * ball->getRadius();

    Square *castedSquare = reinterpret_cast<Square *>(square); /// TODO: reinterpret_cast -> reinterpret_cast
    if (castedSquare == nullptr)
        throw std::bad_cast(); // вообще то этого не должно быть

    castedSquare->chemicalEnergy += kineticEnergyInCM(ball, square);
    castedSquare->addMass(ball->getMass());
    castedSquare->addArea(ballArea);
    castedSquare->nBalls++;
    castedSquare->momentum += ball->momentum;

    ball->state = MoleculeState::OUT;

    // Т к на каждом шаге кин энергия нормализуется,
    // надо делать вот так, потому что тут кин энергия пропадает
    // initEnergy += kineticEnergy(square);
    // initEnergy -= kineticEnergy(ball);
    // initEnergy -= oldSquareKineticEnergy;
}

void Manager::interactSquareBall(Molecule *first, Molecule *second)
{
    interactBallSquare(second, first);
}

double Manager::energy() const
{
    double energy = 0;
    for (auto const& molecule : gas)
        energy += (molecule->momentum * molecule->momentum) / (2.0 * molecule->getMass());

    return energy;
}


double Manager::totalEnergy() const
{
    double energy = 0;
    for (auto const &molecule : gas)
        if (molecule->type == (int)MoleculeType::SQUARE)
        {
            assert(reinterpret_cast<Square*>(molecule)->chemicalEnergy >= 0);
            energy += reinterpret_cast<Square*>(molecule)->chemicalEnergy;
        }

    return energy + this->energy();
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


void reflectMolecules(Molecule* first, Molecule* second)
{
    // Смотреть выкладки! (которые пока на бумаге....)

    auto p1 = first->momentum,    p2 = second->momentum;
    auto r1 = first->position,    r2 = second->position;
    auto m1 = first->getMass(),   m2 = second->getMass();
    auto R1 = first->getRadius(), R2 = second->getRadius();

    auto d = r2 - r1; // по сути отрезок между центрами

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
         collisionCompensation_2 =      p_rel_projection * (timeSinceCollision / m2);

    first->position  += collisionCompensation_1; // Наконец, мы учли эту компенсацию.
    second->position += collisionCompensation_2; // Пора менять импульсы!

    first->addMomentum(dp1);
    second->addMomentum(dp2);
}
