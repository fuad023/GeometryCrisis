
#ifndef OBJECTS_H
#define OBJECTS_H

#include "AFK_Shape.h"

class Window
{
public:

    int display;
    int width, height;
    std::string title;

    Window() {}

    void init(int width, int height, const std::string &title) { this->width = width; this->height = height; this->title = title; }
    void create(bool fullScreen = false)
    {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition( (glutGet(GLUT_SCREEN_WIDTH)-width)/2, (glutGet(GLUT_SCREEN_HEIGHT)-height)/2 );
        glutInitWindowSize(width, height);
        display = glutCreateWindow(title.c_str());
        if(fullScreen) { glutFullScreen(); }

        glClearColor(0.0,0.0,0.0,1.0);
        gluOrtho2D(0, width, height, 0);
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    void clear(const Vec4& colour) { glClearColor(colour.r, colour.g, colour.b, colour.a); }







    void Render(const Point& point)
    {
        glColor4d( point.colour.r, point.colour.g, point.colour.b, point.colour.a );
        glPointSize(point.size);

        glBegin(GL_POINTS);
            glVertex2d( point.draw.x, point.draw.y );
        glEnd();
    }

    void Render(const Line& line)
    {
        glColor4d( line.colour.r, line.colour.g, line.colour.b, line.colour.a );
        glLineWidth(line.size);

        glBegin(GL_LINES);
            glVertex2d( line.draw.x                     , line.draw.y                      );
            glVertex2d( line.draw.x + line.angleOffset.x, line.draw.y + line.angleOffset.y );
        glEnd();
        glColor4ub(255,255,255,255);
    }

    void Render(const Circle& circle)
    {
        glLineWidth(1);
        glColor4ub( circle.getFillColour().r, circle.getFillColour().g, circle.getFillColour().b, circle.getFillColour().a);
        glBegin(GL_POLYGON);
        for (int i = 0; i < circle.getVertex(); ++i) {
            double theta = 2.0 * M_PI * double(i) / double(circle.getVertex()); // Angle in radians
            double x = circle.getRadius() * cos(-theta); // Calculate x coordinate
            double y = circle.getRadius() * sin(-theta); // Calculate y coordinate
            glVertex2d(x + circle.draw.x, y + circle.draw.y); // Output vertex
        }
        glEnd();


        glLineWidth(circle.getOutlineThickness());
        glColor4ub( circle.getOutlineColour().r, circle.getOutlineColour().g, circle.getOutlineColour().b, circle.getOutlineColour().a);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < circle.getVertex(); ++i) {
            double theta = 2.0 * M_PI * double(i) / double(circle.getVertex()); // Angle in radians
            double x = circle.getRadius() * cos(-theta); // Calculate x coordinate
            double y = circle.getRadius() * sin(-theta); // Calculate y coordinate
            glVertex2d(x + circle.draw.x, y + circle.draw.y); // Output vertex
        }
        glEnd();

        //glColor4ub(255,255,255,255);
    }

    void Render(const Quad& quad)
    {
        glColor4ub( quad.getOutlineColour().r, quad.getOutlineColour().g, quad.getOutlineColour().b, quad.getOutlineColour().a );
        glLineWidth(quad.getOutlineThickness());
        double halfOT = quad.halfOutlineThickness;

        // outline code
        glBegin(GL_LINES); //anti-clock wise rotation
            //left
            glVertex2d( quad.draw.x                    - halfOT   , quad.draw.y                    - halfOT*2 );
            glVertex2d( quad.draw.x                    - halfOT   , quad.draw.y + quad.getSize().y            );
            //bottom
            glVertex2d( quad.draw.x                    - halfOT*2 , quad.draw.y + quad.getSize().y + halfOT   );
            glVertex2d( quad.draw.x + quad.getSize().x            , quad.draw.y + quad.getSize().y + halfOT   );
            //right
            glVertex2d( quad.draw.x + quad.getSize().x + halfOT   , quad.draw.y + quad.getSize().y + halfOT*2 );
            glVertex2d( quad.draw.x + quad.getSize().x + halfOT   , quad.draw.y                               );
            //top
            glVertex2d( quad.draw.x + quad.getSize().x + halfOT*2 , quad.draw.y                    - halfOT   );
            glVertex2d( quad.draw.x                               , quad.draw.y                    - halfOT   );
        glEnd();

        // fill code
        glColor4ub( quad.getFillColour().r, quad.getFillColour().g, quad.getFillColour().b, quad.getFillColour().a );
        glBegin(GL_POLYGON);
            glVertex2d( quad.draw.x                    , quad.draw.y               );
            glVertex2d( quad.draw.x                    , quad.draw.y + quad.getSize().y );
            glVertex2d( quad.draw.x + quad.getSize().x , quad.draw.y + quad.getSize().y );
            glVertex2d( quad.draw.x + quad.getSize().x , quad.draw.y               );

        glEnd();
    }

    void Render(const Sprite& tex)
    {
        for (int y=0; y<tex.texSize.y; ++y)
        {
            for (int x=0; x < tex.texSize.x ; ++x)
            {
                int xo = x;
                int pixel = (y * tex.texSize.x + xo) * 3;
                int r = tex.texture[pixel+0]; int g = tex.texture[pixel+1]; int b = tex.texture[pixel+2];
                int a = 255; if (tex.offColour == Vec4(r,g,b,a)) { a=0; }

                glPointSize(tex.size); glColor4ub(r,g,b,a);
                glBegin(GL_POINTS);
                    if (tex.getScale().x == SPRITE_RIGHT)
                    {
                        glVertex2i( (xo*  tex.size + tex.size/2.0) + tex.draw.x ,
                                    (y  * tex.size + tex.size/2.0) + tex.draw.y );
                    }
                    if (tex.getScale().x == SPRITE_LEFT)
                    {
                        glVertex2i( tex.texSize.x*tex.size - (xo*tex.size + tex.size/2.0) + tex.draw.x ,
                                                             (y *tex.size + tex.size/2.0) + tex.draw.y );
                    }
                glEnd();
            }
        }
        glColor4ub(255,255,255,255);
    }

    void Render(Image& tex)
    {
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, tex.texture);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glBegin(GL_QUADS);
            if (tex.getScale().x == SPRITE_RIGHT) {
                glTexCoord2f(0, 0);
                glVertex2f(tex.draw.x, tex.draw.y + tex.texSize.y);

                glTexCoord2f(1, 0);
                glVertex2f(tex.draw.x + tex.texSize.x, tex.draw.y+tex.texSize.y);

                glTexCoord2f(1, -1);
                glVertex2f(tex.draw.x + tex.texSize.x, tex.draw.y);

                glTexCoord2f(0, -1);
                glVertex2f(tex.draw.x, tex.draw.y);
            }
            else {
                glTexCoord2f(0, 0);
                glVertex2f(tex.draw.x + tex.texSize.x, tex.draw.y+tex.texSize.y);

                glTexCoord2f(1, 0);
                glVertex2f(tex.draw.x, tex.draw.y + tex.texSize.y);

                glTexCoord2f(1, -1);
                glVertex2f(tex.draw.x, tex.draw.y);

                glTexCoord2f(0, -1);
                glVertex2f(tex.draw.x + tex.texSize.x, tex.draw.y);
            }
            
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void Render(const Text& text)
    {
        const char *c;
        glColor4ub(text.getColour().r,text.getColour().g,text.getColour().b,text.getColour().a);
        glRasterPos2f(text.getPos().x, text.getPos().y);
        for (c = text.getText().c_str(); *c != '\0'; c++) {
            glutBitmapCharacter(text.getFont(), *c);
        }
    }


};

#endif // OBJECTS_H
