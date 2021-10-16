/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Widget.h

Abstract:

    Widget class defenition

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:

--*/


//
// Includes / usings
//

#ifndef VIEW_JULESIMF
#define VIEW_JULESIMF

#include <vector>
#include <__JGDebug.h>  
#include <Events.h>
#include <Shape.h>

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#endif

//
// Defines
//

namespace JG
{
    class Window;

    enum WidgetType
    {
        WTypeRenderable         = 1ll << 0,
        WTypeWindow             = 1ll << 1,
    };

    class Widget
    {
    public:
        long long type = 0;

        enum class HandlerResponce
        {
            SuccessCapture,             // Handler processed the event and does not let other widgets process it as well
            Success = SuccessCapture,   // Default success responce -- capturing the evemt
            SuccessYield,               // Handler processed the event but lets other widgets process it as well
            Failure,                    // Handler can`t process the event
        };

        /**
         * \brief Обработчик события "нажатие"
         *
         * \param event
         */
        virtual HandlerResponce onKeyPressed(Event event);

        /**
         * \brief Обработчик события "нажатие"
         *
         * \param event
         */
        virtual HandlerResponce onKeyReleased(Event event);

        /**
         * \brief Обработчик уведомления о закрытии
         *
         * \param event
         */
        virtual HandlerResponce onClose(Event event);

        virtual ~Widget();
        virtual void addChild(Widget* widget);

    protected:
        std::vector<Widget*> children;
    
    };

    class RenderableWidget : public Widget
    {
    public:
        unsigned beginX, beginY,
                 width,  height;
        
        RenderableWidget(Window* window) : Widget(), window(window)
        {
            type |= WTypeRenderable;
        }

        /**
         * \brief Handler of "mouse clicked"
         *
         * \param event
         */
        virtual HandlerResponce onMouseButtonPressed(Event event);

        /**
         * \brief Handler of "mouse clicked"
         *
         * \param event
         */
        virtual HandlerResponce onMouseButtonReleased(Event event);

        virtual HandlerResponce onMouseMoved(Event event);

        /**
         * \brief Handler of "mouse entered"
         *
         * \param event
         */
        virtual HandlerResponce onMouseEntered(Event event);

        /**
        *  \brief Handler of "mouse left"
        *
        *  \param event
        */
        virtual HandlerResponce onMouseLeft(Event event);

        virtual void renderMyself();
        void render();
        void setWindow(Window* window) { this->window = window; }

    protected:
        bool mouseOn = false;
        bool mousePressed = false;
        Window* window;
        static bool inline isRenderableWidget(Widget const* widget) { return widget->type | WTypeRenderable; }
    };

    struct Button : public RenderableWidget
    {
    public:
        virtual HandlerResponce onMouseButtonPressed(Event event) override final;
        virtual HandlerResponce onMouseButtonReleased(Event event) override final;
        virtual HandlerResponce onMouseEntered(Event event) override final;
        virtual HandlerResponce onMouseLeft(Event event) override final;
        virtual void renderMyself() override;

        virtual HandlerResponce onClick(Event event);
        virtual HandlerResponce onRightClick(Event event);
    

        Button(Window* window, 
               unsigned beginX, unsigned beginY,
               unsigned width,  unsigned height,
               Color color = { 114, 111, 143 });

        void setCaption(char const* caption);
        char const* getCaption();
        void setRectangle(Rectangle rectangle);
        Rectangle getRectangle();

    protected:
        char const* caption = nullptr;
        Rectangle defaultRectangle; // Rectangle that is rendered if not clicked and not hovered
        Rectangle currentRectangle; // Rectangle that is rendered right now
        void setCurrentRectangle();
        void validate();
    };

    struct Canvas : public RenderableWidget
    {
        Canvas(Window* window,
               unsigned beginX, unsigned beginY,
               unsigned width,  unsigned height);

        virtual ~Canvas();
        virtual void renderMyself() override;
        
        void flush();
        void setPixel(int x, int y, Color const& color) 
        {
            if (x < 0 || x >= (int)width ||
                y < 0 || y >= (int)height) 
            {
                return;
            }

            bltCanvas[x][y].color = color;
        }
        Color getPixel(int x, int y) { return bltCanvas[x][y].color; }
    #ifdef SFML_WRAPPER
    protected:
        sf::Vertex** bltRender,      // this blt should be displayed now
                  ** bltCanvas;      // in this blt we draw now
    #endif
    };
}

#endif // !VIEW_JULESIMF