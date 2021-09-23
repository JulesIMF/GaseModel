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

#ifndef WRAPPER_JULESIMF
#define WRAPPER_JULESIMF

//
// To turn SFML realization on, define the next macros:
#define SFML_WRAPPER
//

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#endif

namespace JG
{
    struct Event
    {
        enum EventType
        {
            Other,
            Close,
        };

        EventType type;

        struct EventClose
        {

        };

        union
        {
            EventClose close;
        };

    protected:
    #ifdef SFML_WRAPPER
        sf::Event sfEvent;
    #endif
    };

    struct Window
    {
    enum Style
    {
    #ifdef SFML_WRAPPER
        None        = sf::Style::None,
        Titlebar    = sf::Style::Titlebar,
        Resize      = sf::Style::Resize,
        Close       = sf::Style::Close,
        Fullscreen  = sf::Style::Fullscreen,
        Default     = sf::Style::Default,
    #endif
    };
        Window(int sizeX, int sizeY, char const* title, int style);
        void beginDrawing();
        void endDrawing();
        bool pollEvent(Event& event);
        void close();
    protected:

    #ifdef SFML_WRAPPER
        sf::RenderWindow window_;

    public:
        sf::RenderWindow& getSfWindow() { return window_; }   
    #endif

    };

#ifdef SFML_WRAPPER
    using Color = sf::Color;
#else
    struct Color
    {
        
    };
#endif

    //
    // Shapes
    //

    struct Shape
    {
    public:
        virtual void draw(Window& window) const = 0;
        virtual void move(int newX, int newY) = 0;
        virtual void setColor(Color color) = 0;
    };

    struct Circle : public Shape
    {
    public:
        Circle(int x, int y, int radius);
        virtual void draw(Window &window) const override;
        virtual void move(int newX, int newY) override;
        virtual void setColor(Color color) override;

    protected:
#ifdef SFML_WRAPPER
    sf::CircleShape circle_;
#endif
    };
    
    struct Rectangle : public Shape
    {
    public:
        Rectangle(int x, int y, int width, int heigth);
        virtual void draw(Window &window) const override;
        virtual void move(int newX, int newY) override;
        virtual void setColor(Color color) override;

    protected:
#ifdef SFML_WRAPPER
        sf::RectangleShape rectangle_;
#endif
    };
}

#endif // !WRAPPER_JULESIMF