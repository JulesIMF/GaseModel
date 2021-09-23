/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    main.cpp

Abstract:

    Entry point module.

Author / Creation date:

    JulesIMF / 12.09.21

Revision History:

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

void fillManager(Manager& manager, int step, int boundX, int boundY, int maxSpeed, double radius)
{
    srand(time(0));

    for (int x = 0; x < boundX; x += step)
    {
        for (int y = 0; y < boundY; y += step)
        {
            manager.insert(new Ball(
                //rand() % 5 + 5,
                radius,

                //double(rand() % 1000 + 1) / 1000.0,
                0.1,

                {x, y},

                {rand() % (maxSpeed), rand() % (maxSpeed)}));
        }
    }
}

int main()
{
    int const boundX = 1280, boundY = 720;
    Window window(boundX, boundY, "ALEXANDR GANDYRBAYEV", Window::Default ^ Window::Resize);
    Event event;

    high_resolution_clock clock;
    auto previousTimeStepped    = clock.now();
    auto previousTimeDisplayed  = clock.now();
    auto previousTimeEnergy     = clock.now();

    double fps = 60.0,     // frames ps
           cps = fps * 5,   // calculates ps
           eps = 1;         // energies ps

    Manager manager((double)boundX, (double)boundY);
    
    int const nMolecules = 2500;
    fillManager(manager, 300, boundX, boundY, 200, 40);
    printf("%-2d s: K = %lf\n", 0, manager.energy());

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
            printf("%-2d s: K = %lf\n", ++time, manager.energy() );
        }
    }
}
