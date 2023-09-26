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
void renderScene(Shader_Class& shader,Model_Class& model);

// Impostazioni
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
bool shadows = true;
bool shadowsKeyPressed = false;

// camera
Camera_Class camera(Vec3F(0.0f, 1.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
//Illuminazione
//appresenta la posizione della sorgente luminosa nelle coordinate dello spazio mondiale
Vec3F lightPos(0.0f, 2.0f, 0.0f);

int main()
{
  
    Vec4F coloreSfondo = { 0.1f,0.1f,0.1f,1.0f };
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


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Shader_Class lightCubeShader("Light.vs", "Light.fs");
    Shader_Class ourModelShader("Model_VS.vs", "Model_FS.fs");
    Shader_Class simpleDepthShader("point_shadow_depth.vs", "point_shadow_depth.fs", "point_shadow_depth.gs");

    //Model_Class ourModel("C:/Modelli/backpack/backpack.obj");
    Model_Class ourModel("C:/Modelli/Map/Gallery_Nom_04.obj");

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    ourModelShader.use();
    ourModelShader.setInt("material.diffuse", 0);
    //ourModelShader.setInt("material.specular", 1);
    ourModelShader.setInt("depthMap", 1);

    float vertices[] = {
        // positions          // normals         
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                                                 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                                                 
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                                                 
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                                                 
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                                                 
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Mat4F matriceConti = Mat4F();
    Mat4F projectionCamera = Mat4F();
    Mat4F view = Mat4F();

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
        glClearColor(coloreSfondo.x, coloreSfondo.y, coloreSfondo.z, coloreSfondo.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pointLightPositions[0].z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
        // 0. create depth cubemap transformation matrices
// -----------------------------------------------
        float far_plane = 25.0f;
        Mat4F shadowProj = shadowProj.projectionMat4F(90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, far_plane);
        std::vector<Mat4F> shadowTransforms;
        
            shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(1.0f, 0.0f, 0.0f), Vec3F(0.0f, -1.0f, 0.0f)));
            shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(-1.0f, 0.0f, 0.0f), Vec3F(0.0f, -1.0f, 0.0f)));
            shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, 1.0f, 0.0f), Vec3F(0.0f, 0.0f, 1.0f)));
            shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, -1.0f, 0.0f), Vec3F(0.0f, 0.0f, -1.0f)));
            shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, 0.0f, 1.0f), Vec3F(0.0f, -1.0f, 0.0f)));
            shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, 0.0f, -1.0f), Vec3F(0.0f, -1.0f, 0.0f)));


            // 1. render scene to depth cubemap
    // --------------------------------
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            simpleDepthShader.use();
            for (unsigned int i = 0; i < 6; ++i)
                simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            simpleDepthShader.setFloat("far_plane", far_plane);
            simpleDepthShader.setVec3("lightPos", lightPos);
            renderScene(simpleDepthShader, ourModel);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // 2. render scene as normal 
    // -------------------------
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ourModelShader.use();
            projectionCamera = projectionCamera.projectionMat4F(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 100.0f);
            Mat4F view = camera.GetMatriceVisualizzazione();
            ourModelShader.setMat4("projection", projectionCamera);
            ourModelShader.setMat4("view", view);
            // set lighting uniforms
            ourModelShader.setVec3("lightPos", lightPos);
            ourModelShader.setVec3("viewPos", camera.Posizione);
            ourModelShader.setInt("shadows", shadows); // enable/disable shadows by pressing 'SPACE'
            ourModelShader.setFloat("far_plane", far_plane);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
            renderScene(ourModelShader, ourModel);
        

        //Disegno dell'oggetto lampada
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projectionCamera);
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
        ourModelShader.setMat4("projection", projectionCamera);
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
    {
        shadows = !shadows;
        shadowsKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        shadowsKeyPressed = false;
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

void renderScene(Shader_Class& shader,Model_Class& model)
{
    Mat4F modelObject = Mat4F(1.0f);
    modelObject = modelObject.scaling(modelObject, 0.008f);
    shader.setMat4("model", modelObject);
    model.Draw(shader);

}
