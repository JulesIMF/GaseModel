/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    wrapper.h

Abstract:

    An abstract wrapper for a graphics library.

Author / Creation date:

    JulesIMF / 15.09.21

Revision History:

    15.09.21        SFML

--*/

//
// Includes / usings
//

#define SFML_WRAPPER
#include "includes/Wrapper.h"

//
// Defines
//

using namespace JG;

#ifdef SFML_WRAPPER

Window::Window(int sizeX, int sizeY, char const *title, int style)
{
    window_.create( {sizeX, sizeY}, title, style);
}


void Window::beginDrawing()
{
    window_.clear();
}


void Window::endDrawing()
{
    window_.display();
}

void Window::close()
{
    window_.close();
}

bool Window::pollEvent(Event& event)
{
    sf::Event sfEvent = {};
    bool pulled = window_.pollEvent(sfEvent);
    if (!pulled)
        return false;

    switch (sfEvent.type)
    {
    case sf::Event::Closed:
        event.type = Event::Close;
        break;
    
    default:
        event.type = Event::Other;
        break;
    }

    return true;
}

Circle::Circle(int x, int y, int radius) : circle_(radius)
{
    circle_.setOrigin({-x, -y});
}

void Circle::draw(Window& window) const
{
    window.getSfWindow().draw(circle_);
}

void Circle::move(int newX, int newY)
{
    circle_.setOrigin({-newX, -newY});
}

void Circle::setColor(Color color)
{
    circle_.setFillColor(color);
}

Rectangle::Rectangle(int x, int y, int width, int heigth) : rectangle_({width, heigth})
{
    rectangle_.setOrigin({-x, -y});
}

void Rectangle::draw(Window &window) const
{
    window.getSfWindow().draw(rectangle_);
}

void Rectangle::move(int newX, int newY)
{
    rectangle_.setOrigin({-newX, -newY});
}

void Rectangle::setColor(Color color)
{
    rectangle_.setFillColor(color);
}

#endif //!SFML_WRAPPER