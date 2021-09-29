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
            KeyPressed,
        };

        EventType type;

        struct EventClose
        {

        };

        struct EventKeyPressed
        {
            enum Key
            {
                Unknown = -1, ///< Unhandled key
                A = 0,        ///< The A key
                B,            ///< The B key
                C,            ///< The C key
                D,            ///< The D key
                E,            ///< The E key
                F,            ///< The F key
                G,            ///< The G key
                H,            ///< The H key
                I,            ///< The I key
                J,            ///< The J key
                K,            ///< The K key
                L,            ///< The L key
                M,            ///< The M key
                N,            ///< The N key
                O,            ///< The O key
                P,            ///< The P key
                Q,            ///< The Q key
                R,            ///< The R key
                S,            ///< The S key
                T,            ///< The T key
                U,            ///< The U key
                V,            ///< The V key
                W,            ///< The W key
                X,            ///< The X key
                Y,            ///< The Y key
                Z,            ///< The Z key
                Num0,         ///< The 0 key
                Num1,         ///< The 1 key
                Num2,         ///< The 2 key
                Num3,         ///< The 3 key
                Num4,         ///< The 4 key
                Num5,         ///< The 5 key
                Num6,         ///< The 6 key
                Num7,         ///< The 7 key
                Num8,         ///< The 8 key
                Num9,         ///< The 9 key
                Escape,       ///< The Escape key
                LControl,     ///< The left Control key
                LShift,       ///< The left Shift key
                LAlt,         ///< The left Alt key
                LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
                RControl,     ///< The right Control key
                RShift,       ///< The right Shift key
                RAlt,         ///< The right Alt key
                RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
                Menu,         ///< The Menu key
                LBracket,     ///< The [ key
                RBracket,     ///< The ] key
                Semicolon,    ///< The ; key
                Comma,        ///< The , key
                Period,       ///< The . key
                Quote,        ///< The ' key
                Slash,        ///< The / key
                Backslash,    ///< The \ key
                Tilde,        ///< The ~ key
                Equal,        ///< The = key
                Hyphen,       ///< The - key (hyphen)
                Space,        ///< The Space key
                Enter,        ///< The Enter/Return keys
                Backspace,    ///< The Backspace key
                Tab,          ///< The Tabulation key
                PageUp,       ///< The Page up key
                PageDown,     ///< The Page down key
                End,          ///< The End key
                Home,         ///< The Home key
                Insert,       ///< The Insert key
                Delete,       ///< The Delete key
                Add,          ///< The + key
                Subtract,     ///< The - key (minus, usually from numpad)
                Multiply,     ///< The * key
                Divide,       ///< The / key
                Left,         ///< Left arrow
                Right,        ///< Right arrow
                Up,           ///< Up arrow
                Down,         ///< Down arrow
                Numpad0,      ///< The numpad 0 key
                Numpad1,      ///< The numpad 1 key
                Numpad2,      ///< The numpad 2 key
                Numpad3,      ///< The numpad 3 key
                Numpad4,      ///< The numpad 4 key
                Numpad5,      ///< The numpad 5 key
                Numpad6,      ///< The numpad 6 key
                Numpad7,      ///< The numpad 7 key
                Numpad8,      ///< The numpad 8 key
                Numpad9,      ///< The numpad 9 key
                F1,           ///< The F1 key
                F2,           ///< The F2 key
                F3,           ///< The F3 key
                F4,           ///< The F4 key
                F5,           ///< The F5 key
                F6,           ///< The F6 key
                F7,           ///< The F7 key
                F8,           ///< The F8 key
                F9,           ///< The F9 key
                F10,          ///< The F10 key
                F11,          ///< The F11 key
                F12,          ///< The F12 key
                F13,          ///< The F13 key
                F14,          ///< The F14 key
                F15,          ///< The F15 key
                Pause,        ///< The Pause key

                KeyCount, ///< Keep last -- the total number of keyboard keys

                // Deprecated values:

                Dash = Hyphen,         ///< \deprecated Use Hyphen instead
                BackSpace = Backspace, ///< \deprecated Use Backspace instead
                BackSlash = Backslash, ///< \deprecated Use Backslash instead
                SemiColon = Semicolon, ///< \deprecated Use Semicolon instead
                Return = Enter         ///< \deprecated Use Enter instead
            };
        
            Key key;

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