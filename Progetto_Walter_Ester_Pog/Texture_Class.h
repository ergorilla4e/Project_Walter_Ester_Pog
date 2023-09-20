#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Generale.h"

class Texture_Class
{
    int Texture;
public:
    Texture_Class(int Texture)
    {
        this->Texture = Texture;
    }

    void calcoloTexture(const char* fileName, bool inverti = false, bool rgbORrgba = false, unsigned int rep = GL_REPEAT, unsigned int lin = GL_LINEAR, unsigned int* texture = 0)
    {
        // Genera una texture OpenGL vuota
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        // Collega questa texture come bersaglio corrente per le prossime operazioni GL_TEXTURE_2D
        // Tutte le operazioni GL_TEXTURE_2D future avranno effetto su questa texture
        // Imposta i parametri di avvolgimento della texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, rep);  // Imposta l'avvolgimento della texture su GL_REPEAT (metodo di avvolgimento predefinito)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, rep);
        // Imposta i parametri di filtraggio della texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lin);

        // Flip image
        stbi_set_flip_vertically_on_load(inverti);

        // Carica l'immagine, crea la texture e genera i mipmaps
        int width, height, nrChannels;
        // La funzione FileSystem::getPath(...) fa parte del repository GitHub in modo da poter trovare file su qualsiasi IDE/piattaforma; sostituiscila con il percorso del tuo file immagine.
        unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
        if (data)
        {
            // Specifica i dati dell'immagine come texture OpenGL
            if (rgbORrgba)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            // Genera i mipmaps per la texture
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {

            cout << "Impossibile caricare la texture" << endl;
        }
        // Libera la memoria allocata per i dati dell'immagine
        stbi_image_free(data);
    }
};
#endif
