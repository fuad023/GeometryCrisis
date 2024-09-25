#include "Vec2.h"



Vec2::Vec2() {}
Vec2::Vec2(double n)           : x(n),       y(n) {}
Vec2::Vec2(double x, double y) : x(x),       y(y) {}
Vec2::Vec2(const Vec2& rhs)    : x(rhs.x), y(rhs.y) {}
 

Vec2 Vec2::operator = (const Vec2& rhs) {
    return Vec2(x = rhs.x, y = rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const {
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const Vec2& rhs) const {
    return Vec2(x * rhs.x, y * rhs.y);
}
Vec2 Vec2::operator / (const Vec2& rhs) const {
    return Vec2(x / rhs.x, y / rhs.y);
}

Vec2 Vec2::operator * (double val) const {
    return Vec2(x*val, y*val);
}

Vec2 Vec2::operator / (double val) const {
    return Vec2(x/val, y/val);
}

bool Vec2::operator == (const Vec2& rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const {
    return !(*this == rhs);
}

void Vec2::operator += (const Vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
}
void Vec2::operator -= (const Vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const Vec2& rhs) {
    x *= rhs.x;
    y *= rhs.y;
}

void Vec2::operator /= (const Vec2& rhs) {
    x /= rhs.x;
    y /= rhs.y;
}


void Vec2::operator *= (double val) {
    x *= val;
    y *= val;
}

void Vec2::operator /= (double val) {
    x /= val;
    y /= val;
}

void Vec2::print() const {
    std::cout << "x: " << x << " y: " << y << std::endl;
}

Vec2& Vec2::add(double val) {
    x += val;
    y += val;
    return *this;
}

Vec2& Vec2::scale(double val) {
    x *= val;
    y *= val;
    return *this;
}

double Vec2::dist(const Vec2& rhs) const {
    return sqrt(distSq(rhs));
}

double Vec2::distSq(const Vec2& rhs) const {
    return (x - rhs.x)*(x - rhs.x) + (y - rhs.y)*(y - rhs.y);
}

double Vec2::length() const {
    return sqrt( x*x + y*y );
}

Vec2& Vec2::normalize() {
    double len = Vec2::length();
    x /= len;
    y /= len;
    return *this;
}

Vec2& Vec2::assign (const Vec2& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vec2& Vec2::abs() {
    if (x < 0) x *= -1;
    if (y < 0) y *= -1;
    return *this;
}

Vec2& Vec2::unit(double degree) {
    double radian = degree * (M_PI/180);
    x = cos(radian);
    y = sin(radian);
    return *this;
}

Vec2& Vec2::unit(Vec2 rhs)
{
    rhs.abs();
    *this /= rhs;
    return *this;
}

Vec2& Vec2::diagonalFix() {
    *this /= sqrt(2);
    return *this;    
}
