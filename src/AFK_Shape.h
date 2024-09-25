#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"
#include <GL\glut.h>

#define SPRITE_LEFT -1
#define SPRITE_RIGHT 1

// Function to convert degrees to radians
static double degreeToRadian(double degrees) { return degrees * (M_PI / 180.0); }
static double sinDegree(double degrees) { return std::sin(degreeToRadian(-degrees)); }
static double cosDegree(double degrees) { return std::cos(degreeToRadian(-degrees)); }
static double tanDegree(double degrees) { return std::tan(degreeToRadian(-degrees)); }

class Vec4
{
public:
    double r = 0.0, g = 0.0, b = 0.0, a = 0.0;
    
    Vec4() {}    
    Vec4(double x, double alpha = 255)                                            : r(x), g(x), b(x), a(alpha) {}
    Vec4(double red, double green, double blue, double alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
    Vec4(const Vec4& rhs)                                     : r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a) {}

    void assign(const Vec4& rhs) { r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a; }
    bool operator == (const Vec4& rhs) const {
        return ( (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a) );
    }
};



class Quad
{
    Vec2 pos = Vec2(0.0); 
    double OutlineThickness = 0.0;
    Vec2 size = Vec2(0.0);
    Vec4 outlineColour = Vec4(255.0), fillColour = Vec4(0.0); 

public:
    Vec2 draw = Vec2(0.0);
    double halfOutlineThickness = 0.0;
    
    Quad() {}
    Quad(const Vec2& pos, const Vec2& size, double OutlineThickness = 1.0)
    : pos(pos), size(size), OutlineThickness(OutlineThickness)
    {
        draw = pos - (size/2); halfOutlineThickness = OutlineThickness/2;
        
        GLfloat lineWidthRange[2];
        glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);
        if (OutlineThickness < lineWidthRange[0] || OutlineThickness > lineWidthRange[1]) {
            std::cout << "Supported line width range: [" << lineWidthRange[0] << ", " << lineWidthRange[1] << "]" << std::endl;
        }
    }

    void setOutlineThickness(double size) { this->OutlineThickness = size; this->halfOutlineThickness = size/2.0; }
    double getOutlineThickness() const { return OutlineThickness; }

    void setPos(const Vec2& rhs) { this->pos.assign(rhs); this->draw = this->pos - (size/2); }
    Vec2& getPos() { return pos; } 

    void setSize(const Vec2& size) { this->size = size; }
    const Vec2& getSize() const { return size; }

    void setFillColour(const Vec4& colour) { this->fillColour = colour; }
    const Vec4& getFillColour() const { return fillColour; }
    
    void setOutlineColour(const Vec4& colour) { this->outlineColour = colour; }
    const Vec4& getOutlineColour() const { return outlineColour; }
};

class Circle
{
    Vec2 pos = Vec2(0.0);
    Vec4 outlineColour = Vec4(1.0);
    Vec4 fillColour = Vec4(0.0);
    double OutlineThickness = 0.0;
    double radius = 0.0; int vertices = 0;

public:
    Vec2 draw = Vec2(0.0);
    
    Circle() { vertices = 20; OutlineThickness = 1.0; }
    Circle(const Vec2& pos, double radius, int vertices = 20, double size = 1.0)
    : pos(pos), radius(radius), vertices(vertices), OutlineThickness(OutlineThickness) 
    {
        draw = pos;
    }

    void setPos(const Vec2& rhs) { pos = rhs; draw = pos; }
    const Vec2& getPos() const { return pos; }

    void setOutlineColour(const Vec4& rhs) { outlineColour = rhs; }
    const Vec4& getOutlineColour() const { return outlineColour; }
    void setFillColour(const Vec4& rhs) { fillColour = rhs; }
    const Vec4& getFillColour() const { return fillColour; }

    void setOutlineThickness(double OutlineThickness) { this->OutlineThickness = OutlineThickness; }
    double getOutlineThickness() const { return OutlineThickness; }

    void setRadius(double Radius) { this->radius = Radius; }
    double getRadius() const { return radius; }

    void setVertex(int Vertices) { this->vertices = Vertices; }
    int getVertex() const { return vertices; }


};

class Line
{
public:
    Vec2 pos = Vec2(0.0); Vec2 draw = Vec2(0.0);
    Vec4 colour = Vec4(1.0);
    double lenght = 0.0, angle = 0.0;
    double size = 0.0;
    Vec2 angleOffset = Vec2(0.0);
    
    Line() {}
    Line(const Vec2& pos, double lenght, double angle, double size = 1.0) : pos(pos), lenght(lenght), angle(angle), size(size) 
    {
        angleOffset.x = cosDegree(angle)*lenght;
        angleOffset.y = sinDegree(angle)*lenght;
        draw = pos;
    }
}; 

class Point
{
public:
    Vec2 pos = Vec2(0.0); Vec2 draw = Vec2(0.0);
    Vec4 colour = Vec4(1.0);
    double size = 0.0;

    Point() {}
    Point(const Vec2& pos, double size = 1.0) : pos(pos), size(size)
    {
        draw = pos;
    }
};

class Sprite
{
    Vec2 pos = Vec2(0.0);
    Vec2 scale = Vec2(0.0);

public:
    int* texture;
    Vec2 draw = Vec2(0.0);
    Vec2 texSize = Vec2(0.0);
    double size = 1.0; Vec4 offColour = Vec4(0.0);
    int offset = 0;

    Sprite() { scale = Vec2(1); offColour = Vec4(0,128,0,255); }
    Sprite(const Vec2& pos, const Vec2& sizeTex, int* ref, const Vec2& scale = Vec2(1), double size = 1.0, Vec4 offColour = Vec4(0,128,0,255))
    : pos(pos), texSize(sizeTex), texture(ref), scale(scale), size(size), offColour(offColour)
    {
        draw = pos - (texSize/2.0);
    }
    void setPos(const Vec2& rhs) { pos = rhs; draw = pos - (texSize/2.0); }
    const Vec2& getPos() { return pos; }

    void setScale(const Vec2& scale) { this->scale = scale; }
    const Vec2& getScale() const { return scale; }

};

class Image
{
    Vec2 pos = Vec2(0.0);
    Vec2 scale = Vec2(0.0);

public:
    unsigned int texture;
    Vec2 draw = Vec2(0.0);
    Vec2 texSize = Vec2(0.0);


    Image() { scale = Vec2(1); }
    Image(const Vec2& sizeTex, std::string ref, const Vec2& scale = Vec2(1))
    : texSize(sizeTex), scale(scale) { texture = iLoadImage(ref.c_str()); }
    
    void setPos(const Vec2& rhs) { pos = rhs; draw = pos - (texSize/2.0); }
    const Vec2& getPos() { return pos; }

    void setScale(const Vec2& scale) { this->scale = scale; }
    const Vec2& getScale() const { return scale; }

    unsigned int iLoadImage(const char filename[]);
};

class Text
{
    Vec2 pos = Vec2(0.0);
    std::string text;
    Vec4 colour = Vec4(0.0);
    void* font = GLUT_BITMAP_TIMES_ROMAN_24 ;
public:

    Text() {}

    void setPos(const Vec2& pos) { this->pos = pos; }
    const Vec2& getPos() const { return pos; }

    void setFont(void* Font) { font = Font; }
    void* getFont() const { return font; }

    void setText(const std::string& text) { this->text = text; }
    const std::string& getText() const { return text; }

    void setColour(const Vec4& ref) { this->colour = ref; }
    const Vec4& getColour() const { return colour; }
};

#endif // SHAPE_H