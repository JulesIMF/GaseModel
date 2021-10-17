/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    UI.h

Abstract:

    A module, describing specific UI for main.cpp

Author / Creation date:

    JulesIMF / 12.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <cmath>
#include <Manager.h>
#include <JG.h>
#include "Plot.h"

//
// Defines
//

namespace Main
{
    int const sizeX = 1280, sizeY = 720;
    int const width = 1000, height = 300;

    int const  space = 50;
    int const buttonWidth = sizeX - width - 3 * space;
    int const buttonHeight = buttonWidth / 2;

    Manager* manager;
    int step;
    int boundX;
    int boundY;
    int maxSpeed;
    double radius;
    double mass;
    bool onlyBalls = false;

    struct ButtonAdd : public JG::Button
    {
        ButtonAdd(JG::Window* window) : JG::Button(window,
                                               sizeX - buttonWidth - space, sizeY - buttonHeight - space,
                                               buttonWidth, buttonHeight)
        {
            caption = "Add";
        }

        virtual ~ButtonAdd() {}

        virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
        {
            int sgn1 = (rand() & 1) ? -1 : 1,
                sgn2 = (rand() & 1) ? -1 : 1;

            if ((rand() & 1) || onlyBalls)
                manager->insert(new Ball(
                    radius,
                    mass,
                    { rand() % (Main::width - 2 * (int)radius), rand() % (Main::height - 2 * (int)radius) },
                    mass * Vector2{ sgn1 * rand() % (maxSpeed), sgn2 * rand() % (maxSpeed) }));

            else
                manager->insert(new Square(
                    radius,
                    mass,
                    { rand() % (Main::width - 2 * (int)radius), rand() % (Main::height - 2 * (int)radius) },
                    mass * Vector2{ sgn1 * rand() % (maxSpeed), sgn2 * rand() % (maxSpeed) }));

            return HandlerResponce::Success;
        }

        virtual JG::Widget::HandlerResponce onKeyPressed(JG::Event event)
        {
            if (event.key.code == JG::Keyboard::Enter)
                onClick(event);
            
            return HandlerResponce::Success;
        }
    };

    ButtonAdd* buttonAdd;
    JG::Canvas* canvas;
    Plot* plot;
    int ballsPen = 0, squarePen = 0;

    struct MainWindow : public JG::Window
    {
        MainWindow() : Window(sizeX, sizeY, "ALEXANDR GANDYRBAYEV", Window::Default ^ Window::Resize)
        {
            addChild(buttonAdd = new ButtonAdd(this));
            addChild(canvas = new JG::Canvas(this, space, space, Main::width, Main::height));
            addChild(plot = new Plot(this, space, space * 2 + Main::height, Main::width, Main::sizeY - Main::height - 3 * space));

            ballsPen  = plot->newPen({217, 11, 172}); // Лиловый
            squarePen = plot->newPen({2, 161, 196}); // Голубой
        }

        virtual void renderMyself()
        {
            sf::Font font;
            static char const* fontFileName = "JG/resources/fonts/CourierNew.ttf";
            if (!font.loadFromFile(fontFileName))
            {
                printf("Cant load %s\n", fontFileName);
                return;
            }
        }
    };
}