#pragma once
#include "Generale.h"

class Vec4F
{
public:

	float x, y, z, w;

	Vec4F();
	Vec4F(float, float);
	Vec4F(float, float, float, float);
	Vec4F(const Vec4F&);

	Vec4F operator+(const Vec4F&)const;			//Somma tra vettori
	Vec4F& operator+=(const Vec4F&);			//Somma tra vettori
	Vec4F operator-(const Vec4F&)const;			//Differenza tra vettori
	Vec4F& operator-=(const Vec4F&);			//Differenza tra vettori
	Vec4F operator*(float)const;				//Prodotto per scalare
	Vec4F& operator*=(float);					//Prodotto per scalare
	float operator*(const Vec4F&)const;			//Prodotto scalare
	Vec4F operator%(const Vec4F&)const;			//Prodotto componente per componente (Es:Scalo un vettore in una o più direzioni diverse)
	Vec4F operator/(float)const;				//Divisione per scalare
	Vec4F& operator/=(float);					//Divisione per scalare
	Vec4F operator-()const;						//Moltiplicazione per -1 (Es:Cambio direzione del vettore)
	bool operator==(const Vec4F&)const;			//Verifica se due vettori sono uguali
	bool operator!=(const Vec4F&)const;			//Verifica se due vettori sono diversi
	bool operator>(const Vec4F&)const;			//Verifica se un vettore è maggiore di un altro
	bool operator<(const Vec4F&)const;			//Verifica se un vettore è minore di un altro
	Vec4F& operator=(const Vec4F&);				//Copia un vettore in un altro

	float normaQD()const;						//norma quadra
	float norma()const;							//norma
	Vec4F toNormalize()const;					//ritorna il vettore normalizzato
	Vec4F& normalize();							//normalizza il vettore
	//riflessione

	Vec3F xyz()const;							//restituisce un Vec3F


};

ostream& operator<<(ostream&, const Vec4F&);	//Stampa in sequenza Vec4F
Vec4F operator*(float, const Vec4F&);			//Moltiplica in sequenza prodotti per scalari
