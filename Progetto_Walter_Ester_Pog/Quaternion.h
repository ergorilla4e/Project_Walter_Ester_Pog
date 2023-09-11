#pragma once
#include "Generale.h"

class Quaternion
{
public:

    float m_w, m_x, m_y, m_z;

    Quaternion(float, float, float, float);

    Quaternion operator-()const;
    Quaternion operator+(const Quaternion&)const;
    Quaternion operator-(const Quaternion&)const;
    Quaternion& operator-=(const Quaternion&);
    Quaternion operator*(const Quaternion&)const;
    Quaternion operator*(const float) const;
    Quaternion& operator*=(const float);
    Quaternion operator/(const float) const;

    float norm()const;
    Quaternion normalized()const;
    Quaternion conjugate()const;
    Quaternion inverse()const;
    Quaternion axisAngle(const Quaternion&);
    Quaternion rotate(const Quaternion&, const Quaternion&);
    Quaternion rotateX(float);
    Quaternion rotateY(float);
    Quaternion rotateZ(float);
};

std::ostream& operator<<(std::ostream&, const Quaternion&);