#pragma once
#include "Generale.h"

class Vec2F
{
public:

    float x, y;

    Vec2F();
    Vec2F(float);
    Vec2F(float, float);
    Vec2F(const Vec2F&);

    Vec2F operator+(const Vec2F&) const;          // Somma tra vettori
    Vec2F& operator+=(const Vec2F&);              // Somma tra vettori
    Vec2F operator-(const Vec2F&) const;          // Differenza tra vettori
    Vec2F& operator-=(const Vec2F&);              // Differenza tra vettori
    Vec2F operator*(float) const;                 // Prodotto per scalare
    Vec2F& operator*=(float);                     // Prodotto per scalare
    float operator*(const Vec2F&) const;          // Prodotto scalare
    Vec2F operator%(const Vec2F&) const;          // Prodotto componente per componente (Es: Scalo un vettore in una o più direzioni diverse)
    Vec2F operator/(float) const;                 // Divisione per scalare
    Vec2F& operator/=(float);                     // Divisione per scalare
    Vec2F operator-() const;                      // Moltiplicazione per -1 (Es: Cambio direzione del vettore)
    bool operator==(const Vec2F&) const;          // Verifica se due vettori sono uguali
    bool operator!=(const Vec2F&) const;          // Verifica se due vettori sono diversi
    bool operator>(const Vec2F&) const;           // Verifica se un vettore è maggiore di un altro
    bool operator<(const Vec2F&) const;           // Verifica se un vettore è minore di un altro
    Vec2F& operator=(const Vec2F&);               // Copia un vettore in un altro

    float normaQD() const;                        // Norma quadra
    float norma() const;                         // Norma
    Vec2F toNormalize() const;                   // Ritorna il vettore normalizzato lasciando l'originale invariato
    Vec2F& normalize();                          // Normalizza il vettore
    // Riflessione
};

ostream& operator<<(ostream&, const Vec2F&);      // Stampa in sequenza Vec2F
Vec2F operator*(float, const Vec2F&);             // Moltiplica in sequenza prodotti per scalari
