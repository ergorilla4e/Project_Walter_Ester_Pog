#pragma once
#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Funzione ausiliaria per caricare una texture da file
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

// Classe che rappresenta un modello 3D composto da una o più mesh
class Model_Class
{
public:
    // Dati del modello
    vector<Texture> textures_loaded;   // memorizza tutte le texture caricate fino a quel momento, ottimizzazione per evitare di caricare più volte la stessa texture.
    vector<Mesh_Class> meshes;          // vettore delle mesh che compongono il modello
    string directory;                   // percorso della directory in cui è situato il modello
    bool gammaCorrection;               // indica se è attiva la correzione gamma

    // Costruttore, richiede un percorso per il file del modello 3D.
    Model_Class(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // Disegna il modello, e quindi tutte le sue mesh
    void Draw(Shader_Class& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    // Carica un modello con estensioni supportate da ASSIMP dal file e memorizza le mesh risultanti nel vettore meshes.
    void loadModel(string const& path)
    {
        // legge il file tramite ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // verifica gli errori
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // se non è zero
        {
            cout << "ERRORE::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // recupera il percorso della directory del file
        directory = path.substr(0, path.find_last_of('/'));

        // processa ricorsivamente il nodo radice di ASSIMP
        processNode(scene->mRootNode, scene);
    }

    // Processa un nodo in modo ricorsivo. Processa ogni singola mesh situata nel nodo e ripete questo processo sui nodi figli (se presenti).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // processa ogni mesh situata nel nodo corrente
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // l'oggetto nodo contiene solo indici per indicizzare gli oggetti effettivi nella scena.
            // la scena contiene tutti i dati, il nodo serve solo per tenere le cose organizzate (come le relazioni tra i nodi).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // dopo aver processato tutte le mesh (se presenti), processa ricorsivamente ciascun nodo figlio
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    // Processa una mesh, estrae i dati e restituisce un oggetto Mesh_Class creato dai dati della mesh estratti.
    Mesh_Class processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // dati da riempire
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // scorri ogni vertice della mesh
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            Vec3F vector; // dichiariamo un vettore segnaposto poiché ASSIMP utilizza la sua classe di vettori che non si converte direttamente alla classe vec3 di glm, quindi trasferiamo i dati a questo vettore segnaposto glm::vec3 prima.
            // posizioni
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normali
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // coordinate delle texture
            if (mesh->mTextureCoords[0]) // la mesh contiene coordinate delle texture?
            {
                Vec2F vec;
                // un vertice può contenere fino a 8 diverse coordinate delle texture. Facciamo quindi l'ipotesi che non useremo modelli in cui un vertice può avere più coordinate delle texture, quindi prendiamo sempre il primo set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = Vec2F(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // ora scorri ogni faccia della mesh e recupera gli indici dei vertici corrispondenti
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // recupera tutti gli indici della faccia e memorizzali nel vettore indices
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // processa i materiali
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // assumiamo una convenzione per i nomi dei sampler negli shader. Ogni texture diffusa dovrebbe essere chiamata
        // 'texture_diffuseN' dove N è un numero sequenziale da 1 a MAX_SAMPLER_NUMBER. 
        // Lo stesso vale per le altre texture come riassunto dalla seguente lista:
        // diffusa: texture_diffuseN
        // speculare: texture_specularN
        // normale: texture_normalN

        // 1. mappe diffuse
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. mappe speculari
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. mappe normali
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. mappe di altezza
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // restituisci un oggetto mesh creato dai dati estratti della mesh
        return Mesh_Class(vertices, indices, textures);
    }

    // Controlla tutte le texture dei materiali di un dato tipo e carica le texture se non sono già caricate.
    // Le informazioni richieste vengono restituite come una struttura Texture.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // controlla se la texture è stata già caricata e in tal caso, continua alla prossima iterazione: salta il caricamento di una nuova texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // una texture con lo stesso percorso del file è già stata caricata, continua con la successiva. (ottimizzazione)
                    break;
                }
            }
            if (!skip)
            {   // se la texture non è stata ancora caricata, caricala
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // memorizzala come texture caricata per l'intero modello, per assicurarci di non caricare inutilmente texture duplicate.
            }
        }
        return textures;
    }
};

// Funzione per caricare una texture da file
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Errore nel caricare la texture al percorso: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

#endif
