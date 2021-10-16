/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Widget.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 10.10.21

Revision History:
	11.10.21  13:13		Widget and RenderableWidget released

--*/


//
// Includes / usings
//

#include <cassert>
#include <Widget.h>
using namespace JG;

//
// Defines
//

#define PROCESS_CHILD_RESPONCE(action)                      \
switch (action)                                             \
{                                                           \
            case HandlerResponce::SuccessCapture:           \
                return HandlerResponce::SuccessCapture;     \
                                                            \
                                                            \
            case HandlerResponce::SuccessYield:             \
                responce = HandlerResponce::SuccessYield;   \
                                                            \
                break;                                      \
                                                            \
            case HandlerResponce::Failure:                  \
                break;                                      \
                                                            \
            default:                                        \
                assert(!"Unprocessed responce");            \
}

Widget::HandlerResponce Widget::onClose(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child : children)
    {
        PROCESS_CHILD_RESPONCE(child->onClose(event))
    }

    return responce;
}

Widget::~Widget()
{
    for (auto child : children) delete child;
}

void Widget::addChild(Widget* child)
{
    children.push_back(child);
}

Widget::HandlerResponce Widget::onKeyPressed(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child : children)
    {
        PROCESS_CHILD_RESPONCE(child->onKeyPressed(event))
    }

    return responce;
}

Widget::HandlerResponce Widget::onKeyReleased(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child : children)
    {
        PROCESS_CHILD_RESPONCE(child->onKeyReleased(event))
    }

    return responce;
}

Widget::HandlerResponce RenderableWidget::onMouseButtonPressed(Event event)
{
    mousePressed = true;
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        if (!isRenderableWidget(*child))
            continue; 
        RenderableWidget* renderable = static_cast<RenderableWidget*>(*child);

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height &&
            !renderable->mousePressed)
        {
            Event virtualEvent = event;
            virtualEvent.mouseButton = {.button = event.mouseButton.button,
                                        .x      = virtualX,
                                        .y      = virtualY};
            renderable->mousePressed= true;
            PROCESS_CHILD_RESPONCE(renderable->onMouseButtonPressed(virtualEvent))
        }
        
    }

    return responce;
}

Widget::HandlerResponce RenderableWidget::onMouseButtonReleased(Event event)
{
    mousePressed = false;
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        if (!isRenderableWidget(*child))
            continue;
        RenderableWidget* renderable = static_cast<RenderableWidget*>(*child);

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            renderable->mousePressed)
        {
            Event virtualEvent = event;
            virtualEvent.mouseButton = {.button = event.mouseButton.button,
                                        .x      = virtualX,
                                        .y      = virtualY};

            renderable->mousePressed = false;
            PROCESS_CHILD_RESPONCE(renderable->onMouseButtonReleased(virtualEvent))
        }

    }

    return responce;
}

Widget::HandlerResponce RenderableWidget::onMouseMoved(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        if (!isRenderableWidget(*child))
            continue;

        RenderableWidget* renderable = static_cast<RenderableWidget*>(*child);

        int virtualX = event.mouseMove.x - renderable->beginX,
            virtualY = event.mouseMove.y - renderable->beginY;

        /* MouseMoved send signals to hovered rectangles about mouse movement.
         * If it does not cover now renderable, but did, it signals about mouseLeft.
         * And vice versa.
         */

        Event virtualEvent = event;
        virtualEvent.mouseMove = { .x = virtualX,
                                   .y = virtualY };
        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height)
        {
            renderable->onMouseMoved(event);
            if (renderable->mouseOn)
                continue;
                
            renderable->mouseOn = true; // Need to do this if overrided handler does not set mouseOn
            PROCESS_CHILD_RESPONCE(renderable->onMouseEntered(virtualEvent))
        }

        else
        {
            if (!renderable->mouseOn)
                continue;
            
            renderable->mouseOn = false; // Need to do this if overrided handler does not set mouseOn
            PROCESS_CHILD_RESPONCE(renderable->onMouseLeft(virtualEvent))
        }
    }

    return responce;
}

Widget::HandlerResponce RenderableWidget::onMouseEntered(Event event)
{
    mouseOn = true;
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        if (!isRenderableWidget(*child))
            continue;

        RenderableWidget* renderable = static_cast<RenderableWidget*>(*child);

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height &&
            !renderable->mouseOn)
        {
            Event virtualEvent = event;
            virtualEvent.mouseMove = {.x = virtualX,
                                      .y = virtualY};
            renderable->mouseOn = true; // Need to do this if overrided handler does not set mouseOn
            PROCESS_CHILD_RESPONCE(renderable->onMouseEntered(virtualEvent))
        }
    }

    return responce;
}

Widget::HandlerResponce RenderableWidget::onMouseLeft(Event event)
{
    mouseOn = false;
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        if (!isRenderableWidget(*child))
            continue;

        RenderableWidget* renderable = static_cast<RenderableWidget*>(*child);

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height &&
            !renderable->mouseOn)
        {
            Event virtualEvent = event;
            virtualEvent.mouseMove = {.x = virtualX,
                                      .y = virtualY};

            renderable->mouseOn = false; // Need to do this if overrided handler does not set mouseOn
            PROCESS_CHILD_RESPONCE(renderable->onMouseLeft(virtualEvent))
        }
    }

    return responce;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void RenderableWidget::renderMyself()
{
    // pass by default: we have to do this and to ignore GCC
}

void RenderableWidget::render()
{
    renderMyself();

    for (auto child : children)
    {
        if (!isRenderableWidget(child))
            continue;

        RenderableWidget* renderable = static_cast<RenderableWidget*>(child);
        renderable->render();
    }
}

