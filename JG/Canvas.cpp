/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Canvas.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 11.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <algorithm>
#include <Widget.h>
#include <Window.h>
using namespace JG;

//
// Defines
//

Canvas::~Canvas()
{
    for (int x = 0; x != (int)width; x++)
        delete[] bltCanvas[x];

    for (int x = 0; x != (int)width; x++)
        delete[] bltRender[x];
}

Canvas::Canvas(Window* window,
               unsigned beginX, unsigned beginY,
               unsigned width,  unsigned height) : RenderableWidget(window)
{
    this->beginX = beginX;
    this->beginY = beginY;
    this->width  = width;
    this->height = height;

    bltCanvas = new sf::Vertex* [width];
    bltRender = new sf::Vertex* [width];

    for (int x = 0; x != (int)width; x++)
    {
        bltCanvas[x] = new sf::Vertex[height];
        for (int y = 0; y != (int)height; y++)
            bltCanvas[x][y] = { { float(beginX + x), float(beginY + y) }, { 0, 0, 0 } };
    }


    for (int x = 0; x != (int)width; x++)
    {
        bltRender[x] = new sf::Vertex[height];
        for (int y = 0; y != (int)height; y++)
            bltRender[x][y] = { { float(beginX + x), float(beginY + y) }, { 0, 0, 0 } };
    }
}

void Canvas::flush()
{
    std::swap(bltRender, bltCanvas);
    for (int x = 0; x != (int)width; x++)
        for (int y = 0; y != (int)height; y++)
            bltCanvas[x][y] = { { float(beginX + x), float(beginY + y) }, { 0, 0, 0 } };
    
    renderMyself();
    Event paint; paint.type = Event::Paint;
    window->sendEvent(paint);
}

void Canvas::renderMyself()
{
    for (int x = 0; x != (int)width; x++)
        window->getSfWindow().draw(bltRender[x], height, sf::Points);
}