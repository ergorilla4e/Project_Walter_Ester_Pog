#pragma once
#include "Generale.h"

class Mat4F
{
public:

	float* mat4f = new float[16];

	Mat4F();
	Mat4F(float);
	Mat4F(float v[16]);
	Mat4F(float, float, float, float,
		float, float, float, float,
		float, float, float, float,
		float, float, float, float);
	Mat4F(const Vec3F&, const Vec3F&, const Vec3F&, const Vec3F&);
	Mat4F(const Vec4F&, const Vec4F&, const Vec4F&, const Vec4F&);
	Mat4F(const Mat4F&);
	Mat4F(Mat4F&&)noexcept;
	~Mat4F();

	Mat4F operator*(const Mat4F&)const;			//Prodotto tra matrici
	Mat4F& operator*=(const Mat4F&);			//Prodotto tra matrici
	Vec4F operator*(const Vec4F&)const;			//Prodotto matrice vettore
	Mat4F& operator=(const Mat4F&);				//Copia matrice in un'altra
	Mat4F& operator=(Mat4F&&)noexcept;			//Copia matrice in un'altra eliminando la precedente
	float& operator()(int, int);				//Ritorna la posizione delle coordinate nella matrice

	Vec4F row(int)const;						//Ritorna la i-esima riga
	Vec4F col(int)const;						//Ritrona la j-esima colonna
	Mat4F transpose(const Mat4F&);				//Traspone la matrice
	Mat4F inverse(const Mat4F&);				//Inverte la matrice
	Mat4F translation(float, float, float);		//Trasla la matrice
	Mat4F scaling(float);						//Scala la matrice
	Mat4F rotationX(float);						//Ruota il piano Y.Z con X fisso
	Mat4F rotationY(float);						//Ruota il piano X.Z con Y fisso
	Mat4F rotationZ(float);						//Ruota il piano X.Y con Z fisso


};

ostream& operator<<(ostream&, const Mat4F&);	//Stampa in sequenza Mat4F
