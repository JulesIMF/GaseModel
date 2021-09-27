/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    main.cpp

Abstract:

    Entry point module.

Author / Creation date:

    JulesIMF / 12.09.21

Revision History:
	27.09.21  11:07		fillManager now has onlyBalls parameter

--*/


//
// Includes / usings
//

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <Manager.h>

//
// Defines
//

using namespace JG;
using namespace std::chrono;

bool timeHasCome(high_resolution_clock& clock, 
                 _V2::system_clock::time_point& previousTime,
                 double perSeconds,
                 double& timeSinceLast)
{
    timeSinceLast =
        (double)duration_cast<milliseconds>(clock.now() - previousTime).count() / 1000;

    if (timeSinceLast >= 1.0 / perSeconds)
    {
        previousTime = clock.now();
        return true;
    }

    return false;
}


bool timeHasCome(high_resolution_clock& clock, 
                 _V2::system_clock::time_point& previousTime,
                 double perSeconds)
{
    double timeSinceLast = 0;
    return timeHasCome(clock, previousTime, perSeconds, timeSinceLast);
}

void fillManager(Manager& manager, 
                 int step, 
                 int boundX, 
                 int boundY, 
                 int maxSpeed, 
                 double radius, 
                 double mass,
                 bool onlyBalls = false)
{
    srand(time(0));

    for (int x = 0; x < boundX; x += step)
    {
        for (int y = 0; y < boundY; y += step)
        {            
            int sgn1 = (rand() & 1) ? -1 : 1,
                sgn2 = (rand() & 1) ? -1 : 1;

            if ( ((x / step + y / step) & 1) || // шахматная расстановка
                 onlyBalls ) 
                manager.insert(new Ball(
                    radius,
                    mass,
                    {x, y},
                    mass * Vector2{sgn1 * rand() % (maxSpeed), sgn2 * rand() % (maxSpeed)}));
            
            else
                manager.insert(new Square(
                    radius,
                    mass,
                    {x, y},
                    mass * Vector2{sgn1 * rand() % (maxSpeed), sgn2 * rand() % (maxSpeed)}));
        }
    }

    manager.insert(new Square(
                    radius,
                    mass,
                    {radius * 2, boundY / 2},
                    mass * Vector2{10 * mass, 0}));

    manager.insert(new Square(
                    radius,
                    mass,
                    {boundX - radius * 2, boundY / 2},
                    mass * Vector2{-10 * mass, 0}));

    manager.setMinEnergy(MoleculeType::BALL, MoleculeType::BALL, onlyBalls ? INFINITY : 10);
    manager.setMinEnergy(MoleculeType::BALL, MoleculeType::SQUARE, 0);
    manager.setMinEnergy(MoleculeType::SQUARE, MoleculeType::SQUARE, 0);
}

void printEnergy(Manager const& manager, int time)
{
    printf("%ds: K = %lf, H = %lf\n", time, manager.energy(), manager.totalEnergy());
}

int main(int nArgs, char const** vArgs)
{
    int const boundX = 1280, boundY = 720;
    Window window(boundX, boundY, "ALEXANDR GANDYRBAYEV", Window::Default ^ Window::Resize);
    Event event;

    high_resolution_clock clock;
    auto previousTimeStepped    = clock.now();
    auto previousTimeDisplayed  = clock.now();
    auto previousTimeEnergy     = clock.now();

    double fps = 60.0,      // frames ps
           cps = fps * 5,   // calculates ps
           eps = 1;         // energies ps

    Manager manager((double)boundX, (double)boundY);


    int step        = 30;
    int maxSpeed    = 100;
    double radius   = 10;
    double mass     = 20;
    bool onlyBalls  = nArgs > 1;
    fillManager(manager, 
                step, 
                boundX, 
                boundY, 
                maxSpeed, 
                radius, 
                mass,
                onlyBalls);

    printEnergy(manager, 0);

    while(true)
    {
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case Event::Close:
                window.close();
                return 0;

            default:
                break;
            }
        }

        if (timeHasCome(clock, previousTimeDisplayed, fps))
        {
            manager.display(window);
        }

        double timeSinceLastStep = 0;
        if (timeHasCome(clock, previousTimeStepped, cps, timeSinceLastStep))
        {
            manager.nextStep(timeSinceLastStep);
        }

        if (timeHasCome(clock, previousTimeEnergy, eps))
        {
            static int time = 0;
            printEnergy(manager, ++time);
        }
    }
}
