#pragma once

#include "Assets.h"

class CTransform
{
public:
    Vec2   pos     = { 0.0, 0.0 };
    int    speed   = 0;
    Vec2   velMag  = { 0.0, 0.0 };
    Vec2   velAxis = { 0.0, 0.0 }; // (+) x-axis = (+) y-axis = 1 // (-) x-axis = (-) y-axis = -1
    Vec2   scale;
    double angle   = 0.0;
	double theta   = 0.0;
	double x = 0; int dir = 1;

    CTransform() {}
    CTransform(const Vec2& position, const Vec2& velMagnitude, const Vec2& velDirection, double angle)
    : pos(position), velMag(velMagnitude), velAxis(velDirection), angle(angle) {}
};


class CShape
{
public:
    Circle circle;
    Vec4 FILL, OUTLINE;
    
    CShape() {}
    CShape(double radius, std::size_t points, const Vec4& fill, const Vec4& outline, double thickness)
        : FILL(fill), OUTLINE(outline)
    {
        circle.setRadius(radius);
        circle.setVertex(points);
        circle.setFillColour(fill);
        circle.setOutlineColour(outline);
        circle.setOutlineThickness(thickness);
    }

    void setAlpha(const int alpha) { FILL.a = alpha; OUTLINE.a = alpha; }
};


class CCollision
{
public:
    double r = 0.0;
    CCollision(double radius) : r(radius) {}
};

class CInput
{
public:

    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;

    bool shoot = false;
    bool specialWeapon = false;

    CInput() {}
};

class CScore
{
public:
    int score = 0;
    CScore(int s) : score(s) {}
};

class CLifeSpan
{
public:
    int remaining  = 0; // amount of lifespan remaining on the entity
    int total      = 0; // the total initial amount of lifespan
    CLifeSpan(int total) : remaining(total), total(total) {}
};