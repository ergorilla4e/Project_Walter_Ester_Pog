#include "Generale.h"

Vec3F::Vec3F() : x(), y(), z() {}

Vec3F::Vec3F(float val) : x(val), y(val), z(val) {}

Vec3F::Vec3F(float val1, float val2, float val3) : x(val1), y(val2), z(val3) {}

Vec3F::Vec3F(const Vec3F& vec3f) : x(vec3f.x), y(vec3f.y), z(vec3f.z) {}

//OPERATORI

Vec3F Vec3F::operator+(const Vec3F& vec3f) const
{
	return Vec3F(x + vec3f.x, y + vec3f.y, z + vec3f.z);
}

Vec3F& Vec3F::operator+=(const Vec3F& vec3f)
{
	x += vec3f.x;
	y += vec3f.y;
	z += vec3f.z;

	return *this;
}

Vec3F Vec3F::operator-(const Vec3F& vec3f) const
{
	return Vec3F(x - vec3f.x, y - vec3f.y, z - vec3f.z);
}

Vec3F& Vec3F::operator-=(const Vec3F& vec3f)
{
	x -= vec3f.x;
	y -= vec3f.y;
	z -= vec3f.z;

	return *this;
}

Vec3F Vec3F::operator*(float scal) const
{
	return Vec3F(x * scal, y * scal, z * scal);
}

Vec3F& Vec3F::operator*=(float scal)
{
	x *= scal;
	y *= scal;
	z *= scal;

	return *this;
}

float Vec3F::operator*(const Vec3F& vec3f) const
{
	return (x * vec3f.x + y * vec3f.y + z * vec3f.z);
}

Vec3F Vec3F::operator^(const Vec3F& vec3f) const
{
	return Vec3F(y * vec3f.z - z * vec3f.y, z * vec3f.x - x * vec3f.z, x * vec3f.y - y * vec3f.x);
}

Vec3F Vec3F::operator%(const Vec3F& vec3f) const
{
	return Vec3F(x * vec3f.x, y * vec3f.y, z * vec3f.z);
}

Vec3F Vec3F::operator/(float scal) const
{
	float div = 1.f / scal;
	return Vec3F(x * div, y * div, z * div);
}

Vec3F& Vec3F::operator/=(float scal)
{
	float div = 1.f / scal;
	x *= div;
	y *= div;
	z *= div;

	return *this;
}

Vec3F Vec3F::operator-() const
{
	return Vec3F(-x, -y, -z);
}

bool Vec3F::operator==(const Vec3F& vec3f) const
{
	return (x == vec3f.x && y == vec3f.y && z == vec3f.z);
}

bool Vec3F::operator!=(const Vec3F& vec3f) const
{
	return (x != vec3f.x || y != vec3f.y || z != vec3f.z);
}

bool Vec3F::operator>(const Vec3F& vec3f) const
{
	return (x > vec3f.x && y > vec3f.y && z > vec3f.z);
}

bool Vec3F::operator<(const Vec3F& vec3f) const
{
	return (x < vec3f.x && y < vec3f.y && z < vec3f.z);
}

Vec3F& Vec3F::operator=(const Vec3F& vec3f)
{
	x = vec3f.x;
	y = vec3f.y;
	z = vec3f.z;

	return *this;
}

//FUNZIONI

float Vec3F::normaQD() const
{
	return (x * x + y * y + z * z);
}

float Vec3F::norma() const
{
	return (sqrt(normaQD()));
}

Vec3F Vec3F::toNormalize() const
{
	return ((*this) / norma());
}

Vec3F& Vec3F::normalize()
{
	return ((*this) /= norma());
}

//riflessione
//{
//
//}

Vec3F operator*(float scal, const Vec3F& vec3f)
{
	return (vec3f * scal);
}

ostream& operator<<(ostream& output, const Vec3F& vec3f)
{
	return output << "[" << vec3f.x << ", " << vec3f.y << ", " << vec3f.z << "]";
}

