#include "Generale.h"

Vec2F::Vec2F() : x(), y() {}

Vec2F::Vec2F(float val) : x(val), y(val) {}

Vec2F::Vec2F(float val1, float val2) : x(val1), y(val2) {}

Vec2F::Vec2F(const Vec2F& vec2f) : x(vec2f.x), y(vec2f.y) {}

// OPERATORS

Vec2F Vec2F::operator+(const Vec2F& vec2f) const
{
    return Vec2F(x + vec2f.x, y + vec2f.y);
}

Vec2F& Vec2F::operator+=(const Vec2F& vec2f)
{
    x += vec2f.x;
    y += vec2f.y;
    return *this;
}

Vec2F Vec2F::operator-(const Vec2F& vec2f) const
{
    return Vec2F(x - vec2f.x, y - vec2f.y);
}

Vec2F& Vec2F::operator-=(const Vec2F& vec2f)
{
    x -= vec2f.x;
    y -= vec2f.y;
    return *this;
}

Vec2F Vec2F::operator*(float scal) const
{
    return Vec2F(x * scal, y * scal);
}

Vec2F& Vec2F::operator*=(float scal)
{
    x *= scal;
    y *= scal;
    return *this;
}

float Vec2F::operator*(const Vec2F& vec2f) const
{
    return (x * vec2f.x + y * vec2f.y);
}

Vec2F Vec2F::operator%(const Vec2F& vec2f) const
{
    return Vec2F(x * vec2f.x, y * vec2f.y);
}

Vec2F Vec2F::operator/(float scal) const
{
    float div = 1.f / scal;
    return Vec2F(x * div, y * div);
}

Vec2F& Vec2F::operator/=(float scal)
{
    float div = 1.f / scal;
    x *= div;
    y *= div;
    return *this;
}

Vec2F Vec2F::operator-() const
{
    return Vec2F(-x, -y);
}

bool Vec2F::operator==(const Vec2F& vec2f) const
{
    return (x == vec2f.x && y == vec2f.y);
}

bool Vec2F::operator!=(const Vec2F& vec2f) const
{
    return (x != vec2f.x || y != vec2f.y);
}

bool Vec2F::operator>(const Vec2F& vec2f) const
{
    return (x > vec2f.x && y > vec2f.y);
}

bool Vec2F::operator<(const Vec2F& vec2f) const
{
    return (x < vec2f.x && y < vec2f.y);
}

Vec2F& Vec2F::operator=(const Vec2F& vec2f)
{
    x = vec2f.x;
    y = vec2f.y;
    return *this;
}

// FUNCTIONS

float Vec2F::normaQD() const
{
    return (x * x + y * y);
}

float Vec2F::norma() const
{
    return (sqrt(normaQD()));
}

Vec2F Vec2F::toNormalize() const
{
    return ((*this) / norma());
}

Vec2F& Vec2F::normalize()
{
    return ((*this) /= norma());
}

// Remove the undefined reflection code

Vec2F operator*(float scal, const Vec2F& vec2f)
{
    return (vec2f * scal);
}

ostream& operator<<(ostream& output, const Vec2F& vec2f)
{
    return output << "[" << vec2f.x << ", " << vec2f.y << "]";
}
