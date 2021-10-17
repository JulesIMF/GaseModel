/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    main.cpp

Abstract:

    Entry point module.

Author / Creation date:

    JulesIMF / 12.09.21

Revision History:
	29.09.21  23:35		Now total energy (H), mass (M) and area (S) are printed
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
#include "UI.h"

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

    goto SettingEnergies; // Don`t want to wrap the loop into an if

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

SettingEnergies:

    double stdEnergy = mass * maxSpeed * maxSpeed / 10.0;

    manager.setMinEnergy(MoleculeType::BALL, MoleculeType::BALL, onlyBalls ? INFINITY : stdEnergy);
    manager.setMinEnergy(MoleculeType::BALL, MoleculeType::SQUARE, stdEnergy * (8/5));
    manager.setMinEnergy(MoleculeType::SQUARE, MoleculeType::SQUARE, stdEnergy);
}

void printState(Manager const& manager, int time)
{
    printf("%ds: K = %lf, H = %lf, S = %lf, M = %lf\n", 
           time, 
           manager.energy(),
           manager.totalEnergy(),
           manager.totalArea(),
           manager.totalMass());
}


int main(int nArgs, char const** vArgs)
{
    int const boundX = 1280, boundY = 720;
    Main::MainWindow window;
    Event event = {};

    high_resolution_clock clock;
    auto previousTimeStepped    = clock.now();
    auto previousTimeDisplayed  = clock.now();
    auto previousTimeState      = clock.now();

    double fps = 60.0,     // frames ps
           cps = fps * 5,  // calculates ps
           sps = 1;        // states ps

    Manager manager((double)Main::width, (double)Main::height);
    Main::manager = &manager;

    Main::step      = 100;
    Main::maxSpeed  = 200;
    Main::radius    = 10;
    Main::mass      = 5;
    Main::onlyBalls = nArgs > 1;

    fillManager(manager, 
                Main::step, 
                Main::width, 
                Main::height, 
                Main::maxSpeed, 
                Main::radius, 
                Main::mass,
                Main::onlyBalls);

    printState(manager, 0);

    int startTime = time(NULL);

    while(true)
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Close:
                window.close();
                return 0;
            
            default:
                break;
            }
        }

        double timeSinceLastDisplay = 0;
        if (timeHasCome(clock, previousTimeDisplayed, fps, timeSinceLastDisplay))
        {
           manager.display(*Main::canvas);
           auto statistics = manager.getStatistics();
           printf("Enter\n");
           Main::plot->moveSetPen(Main::ballsPen,  timeSinceLastDisplay, statistics.nBalls);
           Main::plot->moveSetPen(Main::squarePen, timeSinceLastDisplay, statistics.nSquares);
           printf("Exit\n");
        }

        double timeSinceLastStep = 0;
        if (timeHasCome(clock, previousTimeStepped, cps, timeSinceLastStep))
        {
            manager.nextStep(timeSinceLastStep);
        }

        if (timeHasCome(clock, previousTimeState, sps))
        {
            static int time = 0;
            printState(manager, ++time);
        }
    }
}
