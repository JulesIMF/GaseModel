/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Button.cpp

Abstract:

    Button class defenition

Author / Creation date:

    JulesIMF / 11.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <Shape.h>
#include <Widget.h>
#include <Window.h>
using namespace JG;

//
// Defines
//

void Button::setCaption(char const* caption)
{
    this->caption = caption;
    renderMyself();
}

char const* Button::getCaption()
{
    return caption;
}

void Button::setRectangle(Rectangle rectangle)
{
    defaultRectangle = currentRectangle = rectangle;
    validate();
    renderMyself();
}

Button::Button(Window* window,
               unsigned beginX, unsigned beginY,
               unsigned width,  unsigned height,
               Color color) : RenderableWidget(window),
                              defaultRectangle(Rectangle(beginX, beginY, width, height)),
                              currentRectangle(defaultRectangle)
{
    defaultRectangle = Rectangle(beginX, beginY, width, height);
    defaultRectangle.setColor(color);
    this->beginX = beginX;
    this->beginY = beginY;
    this->width  = width;
    this->height = height;
    setCurrentRectangle();
}

Rectangle Button::getRectangle()
{
    return defaultRectangle;
}

void Button::validate()
{
    
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

Widget::HandlerResponce Button::onMouseEntered(Event event)
{
    setCurrentRectangle();
    renderMyself();
    Event paint = {}; paint.type = Event::Paint;
    window->sendEvent(paint);
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onMouseLeft(Event event)
{
    setCurrentRectangle();
    renderMyself();
    Event paint = {}; paint.type = Event::Paint;
    window->sendEvent(paint);
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onMouseButtonPressed(Event event)
{
    setCurrentRectangle();
    renderMyself();
    Event paint = {}; paint.type = Event::Paint;
    window->sendEvent(paint);
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onMouseButtonReleased(Event event)
{
    setCurrentRectangle();
    renderMyself();
    if (event.mouseButton.button == Mouse::Button::Left)
        onClick(event);

    if (event.mouseButton.button == Mouse::Button::Right)
        onRightClick(event);

    Event paint = {}; paint.type = Event::Paint;
    window->sendEvent(paint);
    return Widget::HandlerResponce::Success;
}

void Button::setCurrentRectangle()
{
    currentRectangle = defaultRectangle;
    auto currentColor = defaultRectangle.getColor();

    double hc = 1.3;

    if (mouseOn)
        currentColor.r = int(currentColor.r * hc),
        currentColor.g = int(currentColor.g * hc),
        currentColor.b = int(currentColor.b * hc);

    if (mousePressed)
        currentColor.r = int(currentColor.r * hc),
        currentColor.g = int(currentColor.g * hc),
        currentColor.b = int(currentColor.b * hc);
    
    currentRectangle.setColor(currentColor);
}

void Button::renderMyself()
{
    currentRectangle.draw(*window);
    sf::Font font;
    static char const* fontFileName = "JG/resources/fonts/CourierNew.ttf";
    if (!font.loadFromFile(fontFileName))
    {
        printf("Can`t load %s\n", fontFileName);
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(caption);
    text.setCharacterSize(24);
    text.setFillColor(Color::White);
    int shiftY = (height - 30) / 2;
    int shiftX = (width - strlen(caption) * 17) / 2;
    text.move({ (float)(beginX + shiftX), (float)(beginY + shiftY) });
    window->getSfWindow().draw(text);
}

Widget::HandlerResponce Button::onClick(Event event)
{
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onRightClick(Event event)
{
    return Widget::HandlerResponce::Success;
}
