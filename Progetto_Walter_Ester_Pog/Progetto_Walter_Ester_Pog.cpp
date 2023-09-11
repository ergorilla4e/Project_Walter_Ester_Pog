#include "Generale.h"

using namespace std;
using namespace rapidxml;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Impostazioni
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Sorgenti dei vertex e fragment shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // Apri il file "base.fs" in modalità lettura
    ifstream theFile("base.fs");

    // PROVE DI LIBRERIA ALGEBRA LINEARE

    // Creazione di un vettore di 4 elementi con valori specifici
    Vec4F vettore = { 0.8f,0.2f,0.2f,1.0f };

    //----------------------------------

    // Inizializza e configura GLFW
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

    // Compila e collega il programma dei nostri shader
    // ------------------------------------
    // Shader vertex
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Verifica gli errori di compilazione dello shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERRORE::SHADER::VERTEX::COMPILAZIONE_FALLITA\n" << infoLog << std::endl;
    }
    // Shader fragment
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Verifica gli errori di compilazione dello shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERRORE::SHADER::FRAGMENT::COMPILAZIONE_FALLITA\n" << infoLog << std::endl;
    }
    // Collega gli shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Verifica gli errori di collegamento
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERRORE::SHADER::PROGRAMMA::COLLEGAMENTO_FALLITO\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Imposta i dati dei vertici e configura gli attributi dei vertici
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // sinistra  
         0.5f, -0.5f, 0.0f, // destra 
         0.0f,  0.5f, 0.0f  // sopra   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Collega prima il Vertex Array Object (VAO), poi collega e imposta il buffer dei vertici e infine configura gli attributi dei vertici.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Nota che questo è consentito, la chiamata a glVertexAttribPointer registra VBO come il Vertex Buffer Object (VBO) associato all'attributo dei vertici, quindi successivamente possiamo scollegare in modo sicuro.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Puoi scollegare il VAO in seguito in modo che altre chiamate al VAO non possano modificare accidentalmente questo VAO, ma ciò accade raramente. Modificare altri VAO richiede comunque una chiamata a glBindVertexArray, quindi di solito non scollegano i VAO (né i VBO) quando non è direttamente necessario.
    glBindVertexArray(0);

    // Sblocca questa chiamata per disegnare poligoni in wireframe.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

        // Disegna il nostro primo triangolo
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // visto che abbiamo solo un VAO non c'è bisogno di collegarlo
        glBindVertexArray(VAO); // collegiamo comunque il VAO per mantenere un po' di ordine
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glDeleteProgram(shaderProgram);

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
