#define STB_IMAGE_IMPLEMENTATION
#include "Generale.h"

using namespace std;
using namespace rapidxml;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Impostazioni
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    // Apri il file "base.fs" in modalità lettura
    //ifstream theFile("base.fs");

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

    Shader_Class shader("VertexShader.vs", "FragmentShader.fs");

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    Vec3F cubePositions[] = {
    Vec3F(0.0f,  0.0f,  0.0f),
    Vec3F(2.0f,  5.0f, -15.0f),
    Vec3F(-1.5f, -2.2f, -2.5f),
    Vec3F(-3.8f, -2.0f, -12.3f),
    Vec3F(2.4f, -0.4f, -3.5f),
    Vec3F(-1.7f,  3.0f, -7.5f),
    Vec3F(1.3f, -2.0f, -2.5f),
    Vec3F(1.5f,  2.0f, -2.5f),
    Vec3F(1.5f,  0.2f, -1.5f),
    Vec3F(-1.3f,  1.0f, -1.5f)
    };

    //unsigned int indices[] = {
    //    0,1,3, //first triangle
    //    1,2,3, //second triangle
    //};
    unsigned int VBO, VAO;//EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
   // glGenBuffers(1, &EBO);
    // Collega prima il Vertex Array Object (VAO), poi collega e imposta il buffer dei vertici e infine configura gli attributi dei vertici.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributi posizione
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attributi colori
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3 * sizeof(float)));
   // glEnableVertexAttribArray(1);
   // 
    //Attributi coordinate texture
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

    unsigned int texture1,texture2;
    Texture_Class textureClass(1);
    textureClass.calcoloTexture("container.jpg",false, true, GL_REPEAT, GL_LINEAR, &texture1);
    textureClass.calcoloTexture("awesomeface.png",true, false, GL_REPEAT, GL_LINEAR, &texture2);

    shader.use();
    shader.setInt("texture1", 0);
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
       // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        Mat4F model = Mat4F(1.0f);
        Mat4F view = Mat4F(1.0f); 
        Mat4F projection = Mat4F(1.0f);

        //model = model.rotationX(model, (float)glfwGetTime() * -55.0f);
        //model = model.rotationY(model, (float)glfwGetTime() * -55.0f);
        //model = model.rotationZ(model, (float)glfwGetTime() * -55.0f);

        //Matrice che gestisce la posizione del piano di vista rispetto al resto in scena
        view = view.translation(view,0.0f, 0.0f, -3.0f);

        //Matrice che gestisce la prospettiva, gestisce il frustum di camera proiettando i vertici nei pixel su schermo
        projection = projection.projectionMat4F(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT , 400.f, projection);

        //unsigned int modelLoc = glGetUniformLocation(shader.ID,"model");
        //unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        //unsigned int projLoc = glGetUniformLocation(shader.ID, "projection");

        //glUniformMatrix4fv(modelLoc,1,GL_FALSE,&model.mat4f[0]);
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.mat4f[0]);
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection.mat4f[0]);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glBindVertexArray(VAO);

        //Aggiorna la model matrix 10 volte, una per ogni cubo e ne disegna uno per volta su schermo
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            Mat4F model = Mat4F(1.0f);
            model = model.translation(model, cubePositions[i].x , cubePositions[i].y , cubePositions[i].z);

            model = model.rotationX(model, (float)glfwGetTime() * 20.0f);
            model = model.rotationY(model, (float)glfwGetTime() * 20.0f);
            model = model.rotationZ(model, (float)glfwGetTime() * 20.0f);

            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        
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
    //glDeleteBuffers(1, &EBO);

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
