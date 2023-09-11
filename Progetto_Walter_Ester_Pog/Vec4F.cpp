#include "Generale.h"

Vec4F::Vec4F() : x(), y(), z(), w() {}

Vec4F::Vec4F(float val, float valW) : x(val), y(val), z(val), w(valW) {}

Vec4F::Vec4F(float val1, float val2, float val3, float valW) : x(val1), y(val2), z(val3), w(valW) {}

Vec4F::Vec4F(const Vec4F& vec4f) : x(vec4f.x), y(vec4f.y), z(vec4f.z), w(vec4f.w) {}

//OPERATORI

Vec4F Vec4F::operator+(const Vec4F& vec4f) const
{
	return Vec4F(x + vec4f.x, y + vec4f.y, z + vec4f.z, w + vec4f.w);
}

Vec4F& Vec4F::operator+=(const Vec4F& vec4f)
{
	x += vec4f.x;
	y += vec4f.y;
	z += vec4f.z;
	w += vec4f.w;

	return *this;
}

Vec4F Vec4F::operator-(const Vec4F& vec4f) const
{
	return Vec4F(x - vec4f.x, y - vec4f.y, z - vec4f.z, w - vec4f.w);
}

Vec4F& Vec4F::operator-=(const Vec4F& vec4f)
{
	x -= vec4f.x;
	y -= vec4f.y;
	z -= vec4f.z;
	z -= vec4f.z;

	return *this;
}

Vec4F Vec4F::operator*(float scal) const
{
	return Vec4F(x * scal, y * scal, z * scal, w * scal);
}

Vec4F& Vec4F::operator*=(float scal)
{
	x *= scal;
	y *= scal;
	z *= scal;
	w *= scal;

	return *this;
}

float Vec4F::operator*(const Vec4F& vec4f) const
{
	return (x * vec4f.x + y * vec4f.y + z * vec4f.z + w * vec4f.w);
}

Vec4F Vec4F::operator%(const Vec4F& vec4f) const
{
	return Vec4F(x * vec4f.x, y * vec4f.y, z * vec4f.z, w * vec4f.w);
}

Vec4F Vec4F::operator/(float scal) const
{
	float div = 1.f / scal;
	return Vec4F(x * div, y * div, z * div, w * div);
}

Vec4F& Vec4F::operator/=(float scal)
{
	float div = 1.f / scal;
	x *= div;
	y *= div;
	z *= div;
	w *= div;

	return *this;
}

Vec4F Vec4F::operator-() const
{
	return Vec4F(-x, -y, -z, -w);
}

bool Vec4F::operator==(const Vec4F& vec4f) const
{
	return (x == vec4f.x && y == vec4f.y && z == vec4f.z && w == vec4f.w);
}

bool Vec4F::operator!=(const Vec4F& vec4f) const
{
	return (x != vec4f.x || y != vec4f.y || z != vec4f.z || w != vec4f.w);
}

bool Vec4F::operator>(const Vec4F& vec4f) const
{
	return (x > vec4f.x && y > vec4f.y && z > vec4f.z && w > vec4f.w);
}

bool Vec4F::operator<(const Vec4F& vec4f) const
{
	return (x < vec4f.x && y < vec4f.y && z < vec4f.z && w < vec4f.w);
}

Vec4F& Vec4F::operator=(const Vec4F& vec4f)
{
	x = vec4f.x;
	y = vec4f.y;
	z = vec4f.z;
	w = vec4f.w;

	return *this;
}

//FUNZIONI

float Vec4F::normaQD() const
{
	return (x * x + y * y + z * z + w * w);
}

float Vec4F::norma() const
{
	return (sqrt(normaQD()));
}

Vec4F Vec4F::toNormalize() const
{
	return ((*this) / norma());
}

Vec4F& Vec4F::normalize()
{
	return ((*this) /= norma());
}

//riflessione
//{
//
//}

Vec3F Vec4F::xyz()const
{
	return Vec3F(x, y, z);
}

Vec4F operator*(float scal, const Vec4F& vec4f)
{
	return (vec4f * scal);
}

ostream& operator<<(ostream& output, const Vec4F& vec4f)
{
	return output << "[" << vec4f.x << ", " << vec4f.y << ", " << vec4f.z << ", " << vec4f.w << "]";
}
