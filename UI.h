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

//
// Defines
//

namespace Main
{
    int const sizeX = 1280, sizeY = 720;
    int const width = 1000, height = 500;

    int const  space = 50;
    int const buttonWidth = sizeX - width - 3 * space;
    int const buttonHeight = sizeY - height - 3 * space;

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
        }
    };

    ButtonAdd* buttonAdd;
    JG::Canvas* canvas;

    struct MainWindow : public JG::Window
    {
        MainWindow() : Window(sizeX, sizeY, "ALEXANDR GANDYRBAYEV", Window::Default^ Window::Resize)
        {
            addChild(buttonAdd = new ButtonAdd(this));
            addChild(canvas = new JG::Canvas(this, space, space, Main::width, Main::height));
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