#pragma once
#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#define MAX_BONE_INFLUENCE 4

using namespace std;

// Struttura rappresentante un singolo vertice nella mesh
struct Vertex {
    // Posizione
    Vec3F Position;
    // Normale
    Vec3F Normal;
    // Coordinate delle texture
    Vec2F TexCoords;
};

// Struttura rappresentante una texture
struct Texture {
    unsigned int id;
    string type;
    string path;
};

// Classe che rappresenta una mesh
class Mesh_Class {
public:
    // Dati della mesh
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    // Costruttore
    Mesh_Class(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // Impostiamo i buffer dei vertici e i puntatori degli attributi.
        setupMesh();
    }

    // Renderizza la mesh
    void Draw(Shader_Class& shader)
    {
        // Collega le texture appropriate
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Attiva l'unità di texture corretta prima del binding
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = to_string(specularNr++);
            else if (name == "texture_normal")
                number = to_string(normalNr++);
           
            // Imposta il campione sull'unità di texture corretta
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // E infine collega la texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        // Disegna la mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Ripristina le impostazioni predefinite
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // Dati di rendering
    unsigned int VBO, EBO;

    // Inizializza tutti gli oggetti/array di buffer
    void setupMesh()
    {
        // Crea buffer/array
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // Carica i dati nei buffer dei vertici
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Specifichiamo ad OpenGL come leggere i buffer
        // Posizioni dei vertici
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Normali dei vertici
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // Coordinate delle texture dei vertici
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
       
        // Disassociamo il Vertex
        glBindVertexArray(0);
    }
};

#endif
