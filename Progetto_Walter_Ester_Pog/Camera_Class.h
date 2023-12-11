#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

// Definisce diverse opzioni possibili per il movimento della telecamera.
enum Camera_Movement {
    AVANTI,
    INDIETRO,
    SINISTRA,
    DESTRA
};

// Valori predefiniti della telecamera
const float YAW = -90.0f; //Rotaziione l'ungo l'asse Y
const float PITCH = 0.0f; //Rotazione lungo l'asse X
const float VELOCITA = 2.5f;
const float SENSIBILITA_MOUSE = 0.1f;
const float ZOOM = 55.0f;

class Camera_Class
{
public:
    // Attributi della telecamera
    Vec3F Posizione;
    Vec3F Fronte;
    Vec3F Su;
    Vec3F Destra;
    Vec3F SuMondo;
    // Angoli di Eulero
    float Yaw;
    float Pitch;
    // Opzioni della telecamera
    float VelocitaMovimento;
    float SensibilitaMouse;
    float Zoom;

    // Costruttore con vettori
    Camera_Class(Vec3F posizione = Vec3F(0.0f, 0.0f, 0.0f), Vec3F su = Vec3F(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Fronte(Vec3F(0.0f, 0.0f, -1.0f)), VelocitaMovimento(VELOCITA), SensibilitaMouse(SENSIBILITA_MOUSE), Zoom(ZOOM)
    {
        Posizione = posizione;
        SuMondo = su;
        Yaw = yaw;
        Pitch = pitch;
        AggiornaVettoriTelecamera();
    }

    // Costruttore con valori scalari
    Camera_Class(float posX, float posY, float posZ, float suX, float suY, float suZ, float yaw, float pitch) : Fronte(Vec3F(0.0f, 0.0f, -1.0f)), VelocitaMovimento(VELOCITA), SensibilitaMouse(SENSIBILITA_MOUSE), Zoom(ZOOM)
    {
        Posizione = Vec3F(posX, posY, posZ);
        SuMondo = Vec3F(suX, suY, suZ);
        Yaw = yaw;
        Pitch = pitch;
        AggiornaVettoriTelecamera();
    }

    // Restituisce la matrice di visualizzazione calcolata utilizzando gli angoli di Eulero e la matrice LookAt
    Mat4F GetMatriceVisualizzazione()
    {
        Mat4F matrice = Mat4F();
        return matrice.lookat(move(Posizione), move(Posizione + Fronte), move(Su));
    }

    // Elabora l'input ricevuto da tastiera. 
    inline void ProcessKeyboard(Camera_Movement direzione, float deltaTime)
    {
        float velocita = VelocitaMovimento * deltaTime;
        if (direzione == AVANTI)
            Posizione += Fronte * velocita;
        if (direzione == INDIETRO)
            Posizione -= Fronte * velocita;
        if (direzione == SINISTRA)
            Posizione -= Destra * velocita;
        if (direzione == DESTRA)
            Posizione += Destra * velocita;
    }

    // Elabora l'input ricevuto dal mouse.
    inline void ProcessMouseMovement(float xoffset, float yoffset, bool limitaPitch = true)
    {
        xoffset *= SensibilitaMouse;
        yoffset *= SensibilitaMouse;

        Yaw += xoffset;
        Pitch += yoffset;

        // Assicura che quando il pitch è fuori dai limiti, lo schermo non si ribalti
        if (limitaPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Aggiorna i vettori Fronte, Destra e Su utilizzando gli angoli di Eulero aggiornati
        AggiornaVettoriTelecamera();
    }

    // Elabora l'input ricevuto da un evento di scorrimento della rotella del mouse. 
    inline void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 55.0f)
            Zoom = 55.0f;
    }

private:
    // Calcola il vettore Fronte dalla nuova posizione degli angoli di Eulero della telecamera
    inline void AggiornaVettoriTelecamera()
    {
        // Calcola il nuovo vettore Fronte
        Vec3F fronte;
        fronte.x = cos(Yaw * (M_PI / 180.0f)) * cos(Pitch * (M_PI / 180.0f));
        fronte.y = sin(Pitch * (M_PI / 180.0f));
        fronte.z = sin(Yaw * (M_PI / 180.0f)) * cos(Pitch * (M_PI / 180.0f));
        Fronte = fronte.normalize(); // Normalizza il vettore Fronte per evitare problemi di lunghezza quando si guarda verso l'alto o verso il basso, il che comporterebbe un movimento più lento.

        // Ricalcola anche i vettori Destra e Su
        Destra = (Fronte ^ SuMondo).normalize(); // Normalizza i vettori poiché la loro lunghezza si avvicina a 0 quanto più si guarda verso l'alto o verso il basso, il che comporta un movimento più lento.
        Su = (Destra ^ Fronte).normalize();
    }
};

#endif
