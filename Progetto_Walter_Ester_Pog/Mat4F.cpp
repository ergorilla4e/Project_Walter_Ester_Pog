#include "Generale.h"

Mat4F::Mat4F()
{
	memset(mat4f, 0.f, sizeof(float) * 16);
	mat4f[0] = mat4f[5] = mat4f[10] = mat4f[15] = 1.f;
}

Mat4F::Mat4F(float val)
{
	memset(mat4f, 0.f, sizeof(float) * 16);
	mat4f[0] = mat4f[5] = mat4f[10] = mat4f[15] = val;
}

Mat4F::Mat4F(float v[16]) : mat4f(v) {}

Mat4F::Mat4F(float val0, float val1, float val2, float val3,
	float val4, float val5, float val6, float val7,
	float val8, float val9, float val10, float val11,
	float val12, float val13, float val14, float val15)
{
	mat4f[0] = val0;	mat4f[1] = val4;	mat4f[2] = val8;	mat4f[3] = val12;
	mat4f[4] = val1;	mat4f[5] = val5;	mat4f[6] = val9;	mat4f[7] = val13;
	mat4f[8] = val2;	mat4f[9] = val6;	mat4f[10] = val10;	mat4f[11] = val14;
	mat4f[12] = val3;	mat4f[13] = val7;	mat4f[14] = val11;	mat4f[15] = val15;
}

Mat4F::Mat4F(const Vec3F& u, const Vec3F& v, const Vec3F& h, const Vec3F& k)
{
	mat4f[0] = u.x;		mat4f[1] = u.y;		mat4f[2] = u.z;		mat4f[3] = 0.f;
	mat4f[4] = v.x;		mat4f[5] = v.y;		mat4f[6] = v.z;		mat4f[7] = 0.f;
	mat4f[8] = h.x;		mat4f[9] = h.y;		mat4f[10] = h.z;	mat4f[11] = 0.f;
	mat4f[12] = k.x;	mat4f[13] = k.y;	mat4f[14] = k.z;	mat4f[15] = 1.f;
}

Mat4F::Mat4F(const Vec4F& u, const Vec4F& v, const Vec4F& h, const Vec4F& k)
{
	mat4f[0] = u.x;		mat4f[1] = u.y;		mat4f[2] = u.z;		mat4f[3] = u.w;
	mat4f[4] = v.x;		mat4f[5] = v.y;		mat4f[6] = v.z;		mat4f[7] = v.w;
	mat4f[8] = h.x;		mat4f[9] = h.y;		mat4f[10] = h.z;	mat4f[11] = h.w;
	mat4f[12] = k.x;	mat4f[13] = k.y;	mat4f[14] = k.z;	mat4f[15] = k.w;
}

Mat4F::Mat4F(const Mat4F& otherMat4f)
{
	memcpy(mat4f, otherMat4f.mat4f, sizeof(float) * 16);
}

Mat4F::Mat4F(Mat4F&& otherMat4f)noexcept
{
	delete[] mat4f;

	mat4f = otherMat4f.mat4f;
	otherMat4f.mat4f = nullptr;
}

Mat4F::~Mat4F()
{
	delete[] mat4f;
}
//Passa da gradi a radianti (Metterla su un file.h per operazioni matematiche)
float toRad(float deg)
{
	const float pi = 2.f * acos(-1.0);

	return deg * pi / 180.0f;
}

Mat4F Mat4F::operator*(const Mat4F& otherMat4f)const
{
	Mat4F tmp;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp(j, i) = otherMat4f.mat4f[4 * i] * mat4f[j] + otherMat4f.mat4f[4 * i + 1] * mat4f[j + 4] +
				otherMat4f.mat4f[4 * i + 2] * mat4f[j + 8] + otherMat4f.mat4f[4 * i + 3] * mat4f[j + 12];
		}
	}
	return tmp;
}

Mat4F& Mat4F::operator*=(const Mat4F& otherMat4f)
{
	Mat4F tmp;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp(j, i) = otherMat4f.mat4f[4 * i] * mat4f[j] + otherMat4f.mat4f[4 * i + 1] * mat4f[j + 4] +
				otherMat4f.mat4f[4 * i + 2] * mat4f[j + 8] + otherMat4f.mat4f[4 * i + 3] * mat4f[j + 12];
		}
	}
	return *this = tmp;
}

Vec4F Mat4F::operator*(const Vec4F& vec4f)const
{
	Vec4F tmp(mat4f[0] * vec4f.x + mat4f[4] * vec4f.y + mat4f[8] * vec4f.z + mat4f[12] * vec4f.w,
		mat4f[1] * vec4f.x + mat4f[5] * vec4f.y + mat4f[9] * vec4f.z + mat4f[13] * vec4f.w,
		mat4f[2] * vec4f.x + mat4f[6] * vec4f.y + mat4f[10] * vec4f.z + mat4f[14] * vec4f.w,
		mat4f[3] * vec4f.x + mat4f[7] * vec4f.y + mat4f[11] * vec4f.z + mat4f[15] * vec4f.w);

	return tmp;
}

Mat4F& Mat4F::operator=(const Mat4F& otherMat4f)
{
	memcpy(mat4f, otherMat4f.mat4f, sizeof(float) * 16);

	return *this;
}

Mat4F& Mat4F::operator=(Mat4F&& otherMat4f)noexcept
{
	delete[] mat4f; mat4f = otherMat4f.mat4f;

	otherMat4f.mat4f = nullptr;

	return *this;
}

float& Mat4F::operator()(int row, int col)
{
	return mat4f[col * 4 + row];
}


Vec4F Mat4F::row(int r)const
{
	return Vec4F(mat4f[r], mat4f[r + 4], mat4f[r + 8], mat4f[r + 12]);
}

Vec4F Mat4F::col(int c)const
{
	return Vec4F(mat4f[c * 4], mat4f[c * 4 + 1], mat4f[c * 4 + 2], mat4f[c * 4 + 3]);
}

Mat4F Mat4F::transpose(const Mat4F& otherMat4f)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < i; j++) {
			float temp = otherMat4f.mat4f[i + j * 4];
			otherMat4f.mat4f[i + j * 4] = otherMat4f.mat4f[j + i * 4];
			otherMat4f.mat4f[j + i * 4] = temp;
		}
	}

	return Mat4F(otherMat4f);
}

Mat4F Mat4F::inverse(const Mat4F& otherMat4f) //Utiliziamo il metodo di inversione di Gauss
{
	int i, j, k;

	float a[32] = { otherMat4f.mat4f[0], otherMat4f.mat4f[4], otherMat4f.mat4f[8], otherMat4f.mat4f[12], 1.f , 0.f , 0.f , 0.f
				   ,otherMat4f.mat4f[1], otherMat4f.mat4f[5], otherMat4f.mat4f[9], otherMat4f.mat4f[13], 0.f , 1.f , 0.f , 0.f
				   ,otherMat4f.mat4f[2], otherMat4f.mat4f[6], otherMat4f.mat4f[10], otherMat4f.mat4f[14], 0.f , 0.f , 1.f , 0.f
				   ,otherMat4f.mat4f[3], otherMat4f.mat4f[7], otherMat4f.mat4f[11], otherMat4f.mat4f[15], 0.f , 0.f , 0.f , 1.f };

	float d;

	/************************************************************************************/

	for (i = 24; i >= 8; i -= 8)
	{
		if (a[i - 8] < a[i])
		{
			for (j = i; j < i + 8; j++)
			{
				swap(a[j], a[j - 8]);
			}
		}
	}

	/************************************************************************************/

	for (k = 0, i = 0; i <= 24; i += 8, k++)
	{
		for (j = 0; j <= 24; j += 8)
		{
			if (j != i)
			{
				d = a[j + k] / a[i + k];
				for (int l = i, k = j; k < j + 8; k++, l++)
				{
					a[k] -= a[l] * d;
				}
			}
		}
	}

	/************************************************************************************/

	for (k = 0, i = 0; i <= 24; i += 8, k++)
	{
		d = 1.f / a[i + k];
		for (j = i; j < i + 8; j++)
		{
			a[j] *= d;
		}
	}

	return *this = Mat4F(a[4], a[5], a[6], a[7]
		, a[12], a[13], a[14], a[15]
		, a[20], a[21], a[22], a[23]
		, a[28], a[29], a[30], a[31]);
}

Mat4F Mat4F::translation(float a, float b, float c)
{
	return Mat4F(1.f, 0.f, 0.f, a
		, 0.f, 1.f, 0.f, b
		, 0.f, 0.f, 1.f, c
		, 0.f, 0.f, 0.f, 1.f);
}

Mat4F Mat4F::scaling(float scale)
{
	return Mat4F(scale, 0.f, 0.f, 0.f
		, 0.f, scale, 0.f, 0.f
		, 0.f, 0.f, scale, 0.f
		, 0.f, 0.f, 0.f, 1.f);
}

Mat4F Mat4F::rotationX(float deg)
{
	float sinT = sin(toRad(deg));
	float cosT = cos(toRad(deg));
	return Mat4F(1.f, 0.f, 0.f, 0.f
		, 0.f, cosT, -sinT, 0.f
		, 0.f, sinT, cosT, 0.f
		, 0.f, 0.f, 0.f, 1.f);
}

Mat4F Mat4F::rotationY(float deg)
{
	float sinT = sin(toRad(deg));
	float cosT = cos(toRad(deg));
	return Mat4F(cosT, 0.f, sinT, 0.f
		, 0.f, 1.f, 0.f, 0.f
		, -sinT, 0.f, cosT, 0.f
		, 0.f, 0.f, 0.f, 1.f);
}

Mat4F Mat4F::rotationZ(float deg)
{
	float sinT = sin(toRad(deg));
	float cosT = cos(toRad(deg));
	return Mat4F(cosT, -sinT, 0.f, 0.f
		, cosT, sinT, 0.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 0.f, 1.f);
}

ostream& operator<<(ostream& output, const Mat4F& mat4f)
{
	if (mat4f.mat4f) {
		output << "Matrix: \n";
		for (int i = 0; i < 4; i++)
		{
			output << "|" << mat4f.mat4f[i] << "\t" << mat4f.mat4f[i + 4] << "\t"
				<< mat4f.mat4f[i + 8] << "\t" << mat4f.mat4f[i + 12] << "|" << endl;
		}
	}
	else
	{
		output << "Matrice vuota" << endl;
	}

	return output;
}