#pragma once
#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#define MAX_BONE_INFLUENCE 4

// Struttura rappresentante un singolo vertice nella mesh
struct Vertex {
    // Posizione
    Vec3F Position;
    // Normale
    Vec3F Normal;
    // Coordinate delle texture
    Vec2F TexCoords;
    // Tangente
    Vec3F Tangent;
    // Bitangente
    Vec3F Bitangent;
    // Indici degli ossi che influenzeranno questo vertice
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // Pesi da ciascun osso
    float m_Weights[MAX_BONE_INFLUENCE];
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

        // Ora che abbiamo tutti i dati necessari, impostiamo i buffer dei vertici e i puntatori degli attributi.
        setupMesh();
    }

    // Renderizza la mesh
    void Draw(Shader_Class& shader)
    {
        // Collega le texture appropriate
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Attiva l'unità di texture corretta prima del binding
            // Recupera il numero della texture (N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            // Imposta il sampler sull'unità di texture corretta
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // E infine bind della texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Disegna la mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Buona pratica ripristinare tutto alle impostazioni predefinite una volta configurato.
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
        // Una cosa fantastica riguardo alle strutture è che la loro disposizione di memoria è sequenziale per tutti i loro elementi.
        // L'effetto è che possiamo passare semplicemente un puntatore alla struttura e si traduce perfettamente in un array glm::vec3/2 che
        // si traduce di nuovo in 3/2 float che si traduce in un array di byte.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Imposta i puntatori degli attributi dei vertici
        // Posizioni dei vertici
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Normali dei vertici
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // Coordinate delle texture dei vertici
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // Tangente dei vertici
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // Bitangente dei vertici
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // Indici degli ossi
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // Pesi degli ossi
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};

#endif
