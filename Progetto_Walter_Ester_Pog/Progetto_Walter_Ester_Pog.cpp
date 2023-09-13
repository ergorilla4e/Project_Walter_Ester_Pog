#define STB_IMAGE_IMPLEMENTATION

#include "Generale.h"

using namespace std;
using namespace rapidxml;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Impostazioni
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Sorgenti dei vertex e fragment shader
// 
// Con Uniform
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos, 1.0);\n"
//"}\0";
//
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"uniform vec4 ourColor;\n" //essendo definita come uniforme si può accedere e modificare il colore da qualsiasi punto del programma e in qualsiasi momento
//"void main()\n"
//"{\n"
//"   FragColor = ourColor;\n"
//"}\n\0";


int main()
{
    // Apri il file "base.fs" in modalità lettura
    ifstream theFile("base.fs");

    // PROVE DI LIBRERIA ALGEBRA LINEARE

    // Creazione di un vettore di 4 elementi con valori specifici
    Vec4F vettore = { 0.7f,0.3f,0.7f,1.0f };

    //----------------------------------

    // Inizializza e configura GLFW
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//#ifdef APPLE
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

    // Creazione della finestra GLFW
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Errore: Impossibile creare la finestra GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Carica tutte le funzioni di OpenGL tramite glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Errore: Impossibile inizializzare GLAD" << std::endl;
        return -1;
    }

    //// Compila e collega il programma dei nostri shader
    //// ------------------------------------
    //// Shader vertex
    //unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);
    //// Verifica gli errori di compilazione dello shader
    //int success;
    //char infoLog[512];
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //    std::cout << "ERRORE::SHADER::VERTEX::COMPILAZIONE_FALLITA\n" << infoLog << std::endl;
    //}
    //// Shader fragment
    //unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    //// Verifica gli errori di compilazione dello shader
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERRORE::SHADER::FRAGMENT::COMPILAZIONE_FALLITA\n" << infoLog << std::endl;
    //}
    //// Collega gli shader
    //unsigned int shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);
    //// Verifica gli errori di collegamento
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    std::cout << "ERRORE::SHADER::PROGRAMMA::COLLEGAMENTO_FALLITO\n" << infoLog << std::endl;
    //}
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    // Imposta i dati dei vertici e configura gli attributi dei vertici
    // ------------------------------------------------------------------
    //float vertices[] = {

    //    -0.5f, -0.5f, 0.0f, // sinistra  
    //     0.5f, -0.5f, 0.0f, // destra 
    //     0.0f,  0.5f, 0.0f  // sopra   
    //};
   
    Shader_Class shader("VertexShader.vs", "FragmentShader.fs");

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0,1,3, //first triangle
        1,2,3, //second triangle
    };
    unsigned int VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Collega prima il Vertex Array Object (VAO), poi collega e imposta il buffer dei vertici e infine configura gli attributi dei vertici.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributi posizione
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attributi colori
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //Attributi coordinate texture
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // E' possibile separare il VAO in seguito in modo che altre chiamate VAO non modifichino accidentalmente questo VAO, ma ciò accade raramente. Modifica altro
    // VAO richiede comunque una chiamata a glBindVertexArray, quindi generalmente non svincoliamo VAO (nè VBO= quando non è direttamente necessario.
    // glBindVertexArray(0)
    //// Nota che questo è consentito, la chiamata a glVertexAttribPointer registra VBO come il Vertex Buffer Object (VBO) associato all'attributo dei vertici, quindi successivamente possiamo scollegare in modo sicuro.
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //// Puoi scollegare il VAO in seguito in modo che altre chiamate al VAO non possano modificare accidentalmente questo VAO, ma ciò accade raramente. Modificare altri VAO richiede comunque una chiamata a glBindVertexArray, quindi di solito non scollegano i VAO (né i VBO) quando non è direttamente necessario.
    //glBindVertexArray(0);

    //// Sblocca questa chiamata per disegnare poligoni in wireframe.
    ////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   /* glBindVertexArray(VAO);*/

    //Dato che abbiamo un solo shader, potremmo anche attivare il nostro shader un volta in anticipo, se lo dedesideriamo
    //glUseProgram(shaderProgram);

    //Caricamento e creazione texture
    unsigned int texture1, texture2;
    // Genera una texture OpenGL vuota
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Collega questa texture come bersaglio corrente per le prossime operazioni GL_TEXTURE_2D
    // Tutte le operazioni GL_TEXTURE_2D future avranno effetto su questa texture
    // Imposta i parametri di avvolgimento della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Imposta l'avvolgimento della texture su GL_REPEAT (metodo di avvolgimento predefinito)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Imposta i parametri di filtraggio della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Carica l'immagine, crea la texture e genera i mipmaps
    int width, height, nrChannels;
    // La funzione FileSystem::getPath(...) fa parte del repository GitHub in modo da poter trovare file su qualsiasi IDE/piattaforma; sostituiscila con il percorso del tuo file immagine.
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // Specifica i dati dell'immagine come texture OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // Genera i mipmaps per la texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
       
        cout << "Impossibile caricare la texture" << endl;
    }
    // Libera la memoria allocata per i dati dell'immagine
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Collega questa texture come bersaglio corrente per le prossime operazioni GL_TEXTURE_2D
    // Tutte le operazioni GL_TEXTURE_2D future avranno effetto su questa texture
    // Imposta i parametri di avvolgimento della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Imposta l'avvolgimento della texture su GL_REPEAT (metodo di avvolgimento predefinito)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Imposta i parametri di filtraggio della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);

    // Carica l'immagine, crea la texture e genera i mipmaps
    // La funzione FileSystem::getPath(...) fa parte del repository GitHub in modo da poter trovare file su qualsiasi IDE/piattaforma; sostituiscila con il percorso del tuo file immagine.
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // Specifica i dati dell'immagine come texture OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // Genera i mipmaps per la texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {

        cout << "Impossibile caricare la texture" << endl;
    }
    // Libera la memoria allocata per i dati dell'immagine
    stbi_image_free(data);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    shader.setInt("texture2", 1);

    glBindVertexArray(VAO);

    // Ciclo di rendering
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Input
        // -----
        processInput(window);

        // Rendering
        // ------
        glClearColor(vettore.x, vettore.y, vettore.z, vettore.w);
        glClear(GL_COLOR_BUFFER_BIT);

        //// Disegna il nostro primo triangolo
        //glUseProgram(shaderProgram);


        //// Aggiorna uniform shader 
        //double timeValue = glfwGetTime();
        //float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //Collegamento texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //Rendering texture
        shader.use();
        

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        
        // glBindVertexArray(0); // non c'è bisogno di scollegarlo ogni volta


        // glfw: scambia i buffer e gestisci gli eventi di input (tasti premuti/rilasciati, movimenti del mouse, ecc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Opzionale: dealloca tutte le risorse una volta che hanno terminato il loro scopo:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //glDeleteProgram(shaderProgram);

    // glfw: termina, cancellando tutte le risorse GLFW precedentemente allocate.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

// Processa tutti gli input: verifica se i tasti rilevanti sono premuti/rilasciati in questo frame e reagisci di conseguenza
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ogni volta che la dimensione della finestra cambia (per resize dell'OS o dell'utente), questa funzione di callback viene eseguita
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Assicura che il viewport corrisponda alle nuove dimensioni della finestra; nota che width e height saranno significativamente più grandi rispetto a quanto specificato su display retina.
    glViewport(0, 0, width, height);
}
