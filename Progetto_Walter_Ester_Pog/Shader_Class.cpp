#ifndef SHADER_H
#define SHADER_H

#include "Generale.h"


using namespace std;
class Shader_Class
{
public:
    unsigned int ID;
    //Il costruttore genera lo shader
    //Il costruttore richiede i percorsi sia dei fali di codice sorgente del vertex shader che del fragment shader, che si possono archiviare su disco come semplici file di testo.
    Shader_Class(const char* vertexPath, const char* fragmentPath)
    {
        //1.Recupera il codice sorgente del vertex/fregment dal percorso del file
        string vertexCode;                                                          //Leggo il contenuto dei file come string
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;

        //Assicura che gli oggetti ifstream possanp generare eccezioni:
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

        try
        {
            //Apre il file
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            //Legge il contenuto del buffer del file nello stream
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Chiude gli handler dei file
            vShaderFile.close();
            fShaderFile.close();
            // Converte le stream in stringhe
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

        }
        catch (ifstream::failure& e)
        {
            cout << "ERRORE::SHADER::LETTURA_FILE_NON_RIUSCITA: " << e.what() << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. Compila gli shader
        unsigned int vertex, fragment;
        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // Programma dello shader
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // Cancella gli shader poiché sono stati collegati nel nostro programma e non sono più necessari
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // Attiva lo shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // Funzioni uniformi di utilità
    // ------------------------------------------------------------------------
    void setBool(const string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    // Funzione di utilità per verificare errori di compilazione/collegamento degli shader.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERRORE::ERRORE_DI_COMPILAZIONE_DEGLI_SHADER di tipo: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERRORE::ERRORE_DI_COLLEGAMENTO_DEL_PROGRAMMA di tipo: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
            }
        }
    }
};
#endif

