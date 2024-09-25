#pragma once

#include <iostream>
#include <cmath>
#define M_PI 3.14159265358979323846

class Vec2
{
public:

    double x = 0.0;
    double y = 0.0;

    Vec2();
    Vec2(double n);
    Vec2(double x, double y);
    Vec2(const Vec2& rhs); // copy constuctor
    
    Vec2 operator = (const Vec2& rhs); // same as the copy constructor above
    Vec2 operator + (const Vec2& rhs) const; // Example: c = a = b, here a and b will remain intact, only c will be manipulated
    Vec2 operator - (const Vec2& rhs) const;
    Vec2 operator * (const Vec2& rhs) const;
    Vec2 operator / (const Vec2& rhs) const;
    Vec2 operator * (double val) const;
    Vec2 operator / (double val) const;

    bool operator == (const Vec2& rhs) const;
    bool operator != (const Vec2& rhs) const;

    void operator += (const Vec2& rhs);
    void operator -= (const Vec2& rhs);
    void operator *= (const Vec2& rhs);
    void operator /= (const Vec2& rhs);
    void operator *= (double val);
    void operator /= (double val);
    void print       ()                const;

    Vec2&  add        (double val);
    Vec2&  scale      (double val);  // somewhat like the above * operator
    double dist       (const Vec2& rhs)  const;
    double distSq     (const Vec2& rhs)  const;
    double length     ()                 const;
    Vec2&  normalize  ();
    Vec2&  assign     (const Vec2& rhs);
    
    Vec2&  abs        ();
    Vec2&  unit       (double degree);      // uses sin() cos() functions which are slow
    Vec2&  unit       (Vec2 velDir);        // makes the Vec2(1, 1) keeping the (+) (-) signs [ekhane parameter Vec2 reference hobe na]
    Vec2&  diagonalFix();                   // for same apmlitude in both axes

};

