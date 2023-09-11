#include "Generale.h"

Quaternion::Quaternion(float w = 0.f, float x = 0.f, float y = 0.f, float z = 0.f) : m_w(w), m_x(x), m_y(y), m_z(z) {}

float Quaternion::norm() const
{
	return std::sqrt(m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z);
}

Quaternion Quaternion::normalized() const
{
	float n = norm();
	if (n == 0.f) return Quaternion(1.f, 0.f, 0.f, 0.f);
	return Quaternion(m_w / n, m_x / n, m_y / n, m_z / n);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(m_w, -m_x, -m_y, -m_z);
}

Quaternion Quaternion::inverse() const
{
	float n2 = m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
	if (n2 == 0.f) return Quaternion(1.f, 0.f, 0.f, 0.f);
	return conjugate() / n2;
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(m_w + q.m_w, m_x + q.m_x, m_y + q.m_y, m_z + q.m_z);
}

Quaternion Quaternion::operator-()const
{
	return Quaternion(-m_w, -m_x, -m_y, -m_z);
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
	return Quaternion(m_w - q.m_w, m_x - q.m_x, m_y - q.m_y, m_z - q.m_z);
}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
	m_w -= q.m_w;
	m_x -= q.m_x;
	m_y -= q.m_y;
	m_z -= q.m_z;

	return *this;
}

Quaternion& Quaternion::operator*=(const float k)
{
	m_w *= k;
	m_x *= k;
	m_y *= k;
	m_z *= k;

	return *this;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return Quaternion(m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z,
		m_w * q.m_x + m_x * q.m_w + m_y * q.m_z - m_z * q.m_y,
		m_w * q.m_y - m_x * q.m_z + m_y * q.m_w + m_z * q.m_x,
		m_w * q.m_z + m_x * q.m_y - m_y * q.m_x + m_z * q.m_w);
}

Quaternion Quaternion::operator*(const float k) const
{
	return Quaternion(k * m_w, k * m_x, k * m_y, k * m_z);
}

Quaternion Quaternion::operator/(const float s) const
{
	return Quaternion(m_w / s, m_x / s, m_y / s, m_z / s);
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
	os << "(" << q.m_w << ", " << q.m_x << ", " << q.m_y << ", " << q.m_z << ")";
	return os;
}

Quaternion Quaternion::axisAngle(const Quaternion& q)
{
	float angle = 2.f * std::acos(q.m_w);
	if (angle == 0.f) return Quaternion(1.f, 0.f, 0.f, 0.f);
	float s = std::sqrt(1.f - q.m_w * q.m_w);
	if (s == 0.f) return Quaternion(1.f, 0.f, 0.f, 0.f);
	return Quaternion(std::cos(angle / 2.f), q.m_x / s, q.m_y / s, q.m_z / s);
}

Quaternion Quaternion::rotate(const Quaternion& q, const Quaternion& r)
{
	return r * q * r.conjugate();
}

Quaternion Quaternion::rotateX(float angle)
{
	return Quaternion(std::cos(angle / 2.f), std::sin(angle / 2.f), 0.f, 0.f);
}

Quaternion Quaternion::rotateY(float angle)
{
	return Quaternion(std::cos(angle / 2.f), 0.f, std::sin(angle / 2.f), 0.f);
}

Quaternion Quaternion::rotateZ(float angle)
{
	return Quaternion(std::cos(angle / 2.f), 0.f, 0.f, std::sin(angle / 2.f));
}

