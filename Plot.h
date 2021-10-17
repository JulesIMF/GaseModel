/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Plot.h

Abstract:

    

Author / Creation date:

    JulesIMF / 17.10.21

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
    struct Plot : public JG::Canvas
    {
        Plot(JG::Window* window,
             unsigned beginX, unsigned beginY,
             unsigned width,  unsigned height) : JG::Canvas(window, 
                                                            beginX, beginY,
                                                            width,  height) 
        { 
            buffer = new JG::Color* [width];
            for (int i = 0; i != width; i++)
                buffer[i] = new JG::Color[height];
            
            clear();
        }


        struct Pen
        {
            double x = 0, y = 0;
            JG::Color color = JG::Color::Red;
        };

        int newPen(JG::Color color = JG::Color::Red)
        {
            pens.push_back(Pen());
            pens.back().color = color;
            return (int)pens.size() - 1;
        }

        double unitX = 5.0, unitY = 2.0;

        Pen& getPen(int penIndex) { return pens.at(penIndex); }
        
        void movePen(int penIndex, double dx, double dy)
        {
            auto& pen = getPen(penIndex);
            if (dx < 0)
            {
                pen.x += dx;
                pen.y += dy;
                return;
            }

            double derivative = dy / dx;

            int pixelX      = (int)(pen.x * unitX) % width,
                finalPixelX = (int)((pen.x + dx) * unitX) % width;

            float unitsPerPixelX = 1.0 / unitX;

            int pixelY      = (int)(pen.y * unitY),
                finalPixelY = (int)((pen.y + dy) * unitY);
            
            int yPixelShift = ( finalPixelY < pixelY ) ? -1 : 1;

            for (int steps = 0; pixelX != finalPixelX; pixelX = (pixelX + 1) % width, steps++)
            {
                double nextPixelY = (int)( (pen.y + derivative * (steps * unitsPerPixelX) ) * unitY);

                for (; pixelY <= nextPixelY; pixelY += yPixelShift)
                {
                    if ( !(0 <= pixelY && pixelY < height) )
                        continue;

                    buffer[pixelX][height - pixelY - 1] = pen.color;
                }             

                if (pixelX + 1 == width)
                    clear();
            }

            pen.x += dx;
            pen.y += dy;

            transferFromBuffer(buffer);
            flush();
            window->sendEvent(JG::Event::PaintEvent());
        }

        void setPenPosition(int penIndex, double newX, double newY)
        {
            auto pen = getPen(penIndex);
            movePen(penIndex, newX - pen.x, newY - pen.y);
        }

        void moveSetPen(int penIndex, double dx, double newY)
        {
            auto pen = getPen(penIndex);
            movePen(penIndex, dx, newY - pen.y);
        }

        void clear(JG::Color color = {0, 0, 0})
        {
            for (unsigned x = 0; x != width; x++)
                for (unsigned y = 0; y != height; y++)
                    buffer[x][y] = color;
        }

        protected:
            std::vector<Pen> pens;
            JG::Color** buffer;
    };
}
