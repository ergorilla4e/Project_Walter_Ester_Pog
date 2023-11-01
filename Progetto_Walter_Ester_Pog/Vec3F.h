#pragma once
#ifndef VEC3F_H
#define VEC3F_H

class Vec3F
{
public:

	float x, y, z;

	Vec3F();
	Vec3F(float);
	Vec3F(float, float, float);
	Vec3F(const Vec3F&);

	Vec3F operator+(const Vec3F&)const;			//Somma tra vettori
	Vec3F& operator+=(const Vec3F&);			//Somma tra vettori
	Vec3F operator-(const Vec3F&)const;			//Differenza tra vettori
	Vec3F& operator-=(const Vec3F&);			//Differenza tra vettori
	Vec3F operator*(float)const;				//Prodotto per scalare
	Vec3F& operator*=(float);					//Prodotto per scalare
	float operator*(const Vec3F&)const;			//Prodotto scalare
	Vec3F operator^(const Vec3F&)const;			//Prodotto vettoriale
	Vec3F operator%(const Vec3F&)const;			//Prodotto componente per componente (Es:Scalo un vettore in una o più direzioni diverse)
	Vec3F operator/(float)const;				//Divisione per scalare
	Vec3F& operator/=(float);					//Divisione per scalare
	Vec3F operator-()const;						//Moltiplicazione per -1 (Es:Cambio direzione del vettore)
	bool operator==(const Vec3F&)const;			//Verifica se due vettori sono uguali
	bool operator!=(const Vec3F&)const;			//Verifica se due vettori sono diversi
	bool operator>(const Vec3F&)const;			//Verifica se un vettore è maggiore di un altro
	bool operator<(const Vec3F&)const;			//Verifica se un vettore è minore di un altro
	Vec3F& operator=(const Vec3F&);				//Copia un vettore in un altro

	float normaQD()const;						//norma quadra
	float norma()const;							//norma
	Vec3F toNormalize()const;					//ritorna il vettore normalizzato lasciando l'originale invariato
	Vec3F& normalize();							//normalizza il vettore
	//riflessione


};

ostream& operator<<(ostream&, const Vec3F&);	//Stampa in sequenza Vec3F
Vec3F operator*(float, const Vec3F&);			//Moltiplica in sequenza prodotti per scalari

#endif