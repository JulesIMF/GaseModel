/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Events.h

Abstract:

    

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:

--*/


//
// Includes / usings
//

#ifndef EVENTS_JULESIMF
#define EVENTS_JULESIMF
#include <__JGDebug.h>
#include <cstring>

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#endif

#include <Device.h>

//
// Defines
//

namespace JG
{
    struct Window;

    struct Event
    {
        enum EventType
        {
            Other,
            Close,
            KeyPressed,
            KeyReleased,
            MouseButtonPressed,     
            MouseButtonReleased,    
            MouseMoved,             
            MouseEntered,           
            MouseLeft,
            Paint,
        };

        EventType type;

        struct EventClose
        {

        };

        struct EventKey
        {
            Keyboard::Key code;

            bool          alt;
            bool          control;
            bool          shift;
            bool          system;
        };

        struct EventMouseButton
        {
            enum Action
            {
                Press,
                Release,
            };

            Mouse::Button button; ///< Code of the button that has been pressed
            int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
            int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct EventMouseMove
        {
            int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
            int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
        };

        union
        {
            EventClose              close;
            EventKey                key;
            EventMouseButton        mouseButton;
            EventMouseMove          mouseMove;
        };

        Window* window;

    protected:
#ifdef SFML_WRAPPER
        sf::Event sfEvent;
#endif
    };
}

#endif // !EVENTS_JULESIMF