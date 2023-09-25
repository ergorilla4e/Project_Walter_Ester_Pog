#define STB_IMAGE_IMPLEMENTATION
#include "Generale.h"

//CLASSI AGGIUNTIVE
#include "Shader_Class.h"
#include "Texture_Class.h"
#include "Camera_Class.h"
#include "Mesh_Class.h"
#include "Model_Class.h"

using namespace std;
using namespace rapidxml;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Impostazioni
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera_Class camera(Vec3F(0.0f, 1.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
//Illuminazione
//appresenta la posizione della sorgente luminosa nelle coordinate dello spazio mondiale
Vec3F lightPos(0.0f, 0.0f, 0.0f);

int main()
{
    // Apri il file "base.fs" in modalità lettura
    //ifstream theFile("base.fs");

    // PROVE DI LIBRERIA ALGEBRA LINEARE

    // Creazione di un vettore di 4 elementi con valori specifici
    Vec4F vettore = { 0.1f,0.1f,0.1f,1.0f };
    camera.VelocitaMovimento = 5.f;
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Progetto_Lista_Pettinari", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Errore: Impossibile creare la finestra GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Carica tutte le funzioni di OpenGL tramite glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Errore: Impossibile inizializzare GLAD" << std::endl;
        return -1;
    }

    // Shader_Class shader("VertexShader.vs", "FragmentShader.fs");
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    //Per disegnare il cubo colarato
   // Shader_Class modelShader("Model.vs", "Model.fs");
    //Per disegnare il cubo luminoso
    //Shader_Class lightShader("Light.vs", "Light.fs");

    Shader_Class lightCubeShader("Light.vs", "Light.fs");
    Shader_Class ourModelShader("Model_VS.vs", "Model_FS.fs");

    //Model_Class ourModel("C:/Modelli/backpack/backpack.obj");
    Model_Class ourModel("C:/Modelli/Map/Gallery_Nom_04.obj");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //0.0f,  0.0f,
                                                   //
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //0.0f,  0.0f,
                                                   //
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  //1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  //1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  //0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  //0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  //0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  //1.0f,  0.0f,
                                                   //
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  //1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  //1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  //0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  //0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  //0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  //1.0f,  0.0f,
                                                   //
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  //0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  //1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  //1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  //1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  //0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  //0.0f,  1.0f,
                                                   //
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  //0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  //1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  //1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  //1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  //0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  //0.0f,  1.0f
    };


    //posizione della luce puntiforme 
    Vec3F pointLightPositions[] = {
    Vec3F(-4.4f, 2.8f, -1.2f),
    Vec3F(8.3f, 2.8f, -1.2f),
    Vec3F(-3.8f, 2.8f, 9.2f),
    Vec3F(7.9f, 2.8f, -9.2f),
    Vec3F(7.9f, 2.8f, 9.0f)
    };
  

    unsigned int VBO, lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightCubeVAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //caricamento della texture 
    //unsigned int texture1, texture2;
    //Texture_Class textureClass(1);
    //textureClass.calcoloTexture("container2.png", true, false, GL_REPEAT, GL_LINEAR, &texture1);
    //textureClass.calcoloTexture("container2_specular.png", true, false, GL_REPEAT, GL_LINEAR, &texture2);

    //Configurazione shader
    ourModelShader.use();
    ourModelShader.setInt("material.diffuse", 0);
    ourModelShader.setInt("material.specular", 1);
    // Ciclo di rendering
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Per farlo girare alla stessa velocità su ogni pc
        float currentFrame = static_cast<float> (glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        // -----
        processInput(window);

        // Rendering
        // ------
        glClearColor(vettore.x, vettore.y, vettore.z, vettore.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //modelShader.use()
       //  modelShader.setMat4("projection", projection);
      //  modelShader.setMat4("view", view);

        
        //lightCubeShader.use();

        //view/projection trasformation 
        Mat4F projection = projection.projectionMat4F(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 100.0f);
        Mat4F view = camera.GetMatriceVisualizzazione();

        //view = view.transpose(view);
        //lightShader.setMat4("projection", projection);
        //lightShader.setMat4("view", view);
        //trasformazioni globali
        //model = model.rotationZ(model, 30.0f);
        //lightShader.setMat4("model", model);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1); 
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);
        //// render del cubo
        //glBindVertexArray(cubeVAO);
       

       /* for (unsigned int i = 0; i < 10; i++)
        {
            Mat4F model = Mat4F(1.0f);
            model = model.translation(model, cubePosition[i].x,cubePosition[i].y, cubePosition[i].z);
            float angle = 20.0f * i;
            model = model.rotateAllAxis(model, angle, Vec3F(1.0f, 0.3f, 0.5f));
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/


        //Disegno dell'oggetto lampada
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 5; i++)
        {
            Mat4F modelLight = Mat4F(1.0f);
            modelLight = modelLight.translation(modelLight, pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            modelLight = modelLight.scaling(modelLight, 0.2f);
            lightCubeShader.setMat4("model", modelLight);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        Mat4F modelObject = Mat4F(1.0f);
        ourModelShader.use();
        ourModelShader.setMat4("projection", projection);
        ourModelShader.setMat4("view", view);

        modelObject = modelObject.scaling(modelObject, 0.008f);

        ourModelShader.setMat4("model", modelObject);

        //ourModelShader.use();
        ourModelShader.setVec3("viewPos", camera.Posizione);

        //proprietà dei materiali
        ourModelShader.setFloat("material.shininess", 32.0f);

        ourModelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourModelShader.setVec3("dirLight.ambient", 0.025f, 0.025f, 0.025f);
        ourModelShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
        ourModelShader.setVec3("dirLight.specular", 0.25f, 0.25f, 0.25f);

        for (int i = 0; i < 5; i++)
        {
            ourModelShader.setVec3("pointLights[" + to_string(i) + "].position", pointLightPositions[i]);
            ourModelShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
            ourModelShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
            ourModelShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
            ourModelShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
            ourModelShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.09f);
            ourModelShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032f);
        }

        ourModel.Draw(ourModelShader);

         // glfw: scambia i buffer e gestisci gli eventi di input (tasti premuti/rilasciati, movimenti del mouse, ecc.)
         // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Opzionale: dealloca tutte le risorse una volta che hanno terminato il loro scopo:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    { 
        camera.ProcessKeyboard(AVANTI, deltaTime);
        cout << camera.Posizione << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(INDIETRO, deltaTime);
        cout << camera.Posizione << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(SINISTRA, deltaTime);
        cout << camera.Posizione << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DESTRA, deltaTime);
        cout << camera.Posizione << endl;
    }

    
}

// glfw: ogni volta che la dimensione della finestra cambia (per resize dell'OS o dell'utente), questa funzione di callback viene eseguita
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Assicura che il viewport corrisponda alle nuove dimensioni della finestra; nota che width e height saranno significativamente più grandi rispetto a quanto specificato su display retina.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}