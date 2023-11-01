//CLASSI AGGIUNTIVE
#include "Shader_Class.h"
#include "Camera_Class.h"
#include "Mesh_Class.h"
#include "Model_Class.h"

#include "VAO.h"
#include "VBO.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void renderModel(Shader_Class& ourModelShader, Model_Class& ourModel, float scale, float angle, Vec3F rotatore, float xTrasl, float yTrasl, float zTrasl, Mat4F view, Mat4F projectionCamera);

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

float deltaTime = 0.0f;	//Time between current frame and last frame
float lastFrame = 0.0f; //Time of last frame

//Illuminazione
//appresenta la posizione della sorgente luminosa nelle coordinate del world space
Vec3F lightPos(5.0f, 2.0f, 0.0f);

//rappresentano le scale degli oggetti di scena
float scala1 = 0.008f;
float scala2 = 0.01f;
float scala3 = 0.4f;
float scala4 = 0.03f;

//rappresenta il centro del world space per gli oggetti di scena 
Vec3F traslatore = Vec3F(0, 0, 0);

//rappresentano le rotazioni nei vari assi per gli oggetti di scena 
Vec3F rotatore1 = Vec3F(1, 0, 0);
Vec3F rotatore2 = Vec3F(0, 1, 0);
Vec3F rotatore3 = Vec3F(0, 0, 1);

class View_Class
{
public:

    View_Class(GLFWwindow* window)
    {
		Vec4F coloreSfondo = { 0.1f,0.1f,0.1f,1.0f };
		camera.VelocitaMovimento = 5.f;
		
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		Shader_Class ourModelShader("Model_VS.vs", "Model_FS.fs");
		Shader_Class simpleDepthShader("point_shadow_depth.vs", "point_shadow_depth.fs", "point_shadow_depth.gs");

		Model_Class ourModel("C:/Modelli/Map/MuseoMod.obj");
		Model_Class alienModel("C:/Modelli/Map2/Alien/Reptile Alien Creature.obj");
		Model_Class saturn("C:/Modelli/Map2/Saturn/Stylized_Planets.obj");
		Model_Class dog("C:/Modelli/Map2/Dog/Doguinho.obj");
		Model_Class skull("C:/Modelli/Map2/Skull/animal_skull_statuete.obj");

		// configure depth map FBO
	// -------------------------------------------------------------------------------------------------------------------------
		const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
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
	// -------------------------------------------------------------------------------------------------------------------------
		ourModelShader.use();
		ourModelShader.setInt("material.specular", 0);
		ourModelShader.setInt("material.diffuse", 0);
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

		VAO VAO_LightCube;
		VAO_LightCube.Bind();

		VBO VBO(vertices, sizeof(vertices));

		VAO_LightCube.LinkVBO(VBO, 0);
		VAO_LightCube.Unbind();
		VBO.Unbind();

		Mat4F matriceConti = Mat4F();
		Mat4F projectionCamera = Mat4F();
		Mat4F view = Mat4F();

		// Ciclo di rendering
	// -------------------------------------------------------------------------------------------------------------------------
		while (!glfwWindowShouldClose(window))
		{
			// Per farlo girare alla stessa velocità su ogni pc
			float currentFrame = static_cast<float> (glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Input
	// -------------------------------------------------------------------------------------------------------------------------
			processInput(window);

			// Rendering
	// -------------------------------------------------------------------------------------------------------------------------
			glClearColor(coloreSfondo.x, coloreSfondo.y, coloreSfondo.z, coloreSfondo.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			lightPos.x = static_cast<float>(sin(glfwGetTime() * 0.7) * 9.0);

			// 0. create depth cubemap transformation matrices
	// -------------------------------------------------------------------------------------------------------------------------
			float far_plane = 200.0f;
			Mat4F shadowProj = shadowProj.projectionMat4F(90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, far_plane);
			std::vector<Mat4F> shadowTransforms;

			shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(1.0f, 0.0f, 0.0f), Vec3F(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(-1.0f, 0.0f, 0.0f), Vec3F(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, 1.0f, 0.0f), Vec3F(0.0f, 0.0f, 1.0f)));
			shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, -1.0f, 0.0f), Vec3F(0.0f, 0.0f, -1.0f)));
			shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, 0.0f, 1.0f), Vec3F(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * matriceConti.lookat(lightPos, lightPos + Vec3F(0.0f, 0.0f, -1.0f), Vec3F(0.0f, -1.0f, 0.0f)));

			// 1. render scene to depth cubemap
	// -------------------------------------------------------------------------------------------------------------------------
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			simpleDepthShader.use();

			for (unsigned int i = 0; i < 6; ++i)
				simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

			simpleDepthShader.setFloat("far_plane", far_plane);
			simpleDepthShader.setVec3("lightPos", lightPos);

			renderModel(simpleDepthShader, ourModel, scala1, 0, rotatore1, traslatore.x + 0 / scala1, traslatore.y + 0 / scala1, traslatore.z + 0 / scala1, view, projectionCamera);
			renderModel(simpleDepthShader, alienModel, scala2, 180, rotatore2, traslatore.x + (-4 / scala2), traslatore.y + (0 / scala2), traslatore.z + (7.5 / scala2), view, projectionCamera);
			renderModel(simpleDepthShader, saturn, scala3, 16, rotatore3, traslatore.x + (-4.45 / scala3), traslatore.y + (1.1 / scala3), traslatore.z + (-1.25 / scala3), view, projectionCamera);
			renderModel(simpleDepthShader, dog, scala3, 16, rotatore3, traslatore.x + (8 / scala3), traslatore.y + (0 / scala3), traslatore.z + (-8 / scala3), view, projectionCamera);
			renderModel(simpleDepthShader, skull, scala4, -90, rotatore2, traslatore.x + (8.95 / scala4), traslatore.y + (0.7 / scala4), traslatore.z + (-1.2 / scala4), view, projectionCamera);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 2. render scene as normal 
	// -------------------------------------------------------------------------------------------------------------------------
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

			renderModel(ourModelShader, ourModel, scala1, 0, rotatore1, traslatore.x + 0 / scala1, traslatore.y + 0 / scala1, traslatore.z + 0 / scala1, view, projectionCamera);
			renderModel(ourModelShader, alienModel, scala2, 180, rotatore2, traslatore.x + (-4 / scala2), traslatore.y + (0 / scala2), traslatore.z + (7.5 / scala2), view, projectionCamera);
			renderModel(ourModelShader, saturn, scala3, 16, rotatore3, traslatore.x + (-4.45 / scala3), traslatore.y + (1.1 / scala3), traslatore.z + (-1.25 / scala3), view, projectionCamera);
			renderModel(ourModelShader, dog, scala3, 0, rotatore1, traslatore.x + (8 / scala3), traslatore.y + (0 / scala3), traslatore.z + (-8 / scala3), view, projectionCamera);
			renderModel(ourModelShader, skull, scala4, -90, rotatore2, traslatore.x + (8.95 / scala4), traslatore.y + (0.7 / scala4), traslatore.z + (-1.2 / scala4), view, projectionCamera);

			// glfw: scambia i buffer e gestisci gli eventi di input (tasti premuti/rilasciati, movimenti del mouse, ecc.)
	// -------------------------------------------------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// Opzionale: dealloca tutte le risorse una volta che hanno terminato il loro scopo:
	// -------------------------------------------------------------------------------------------------------------------------
		VAO_LightCube.Delete();
		VBO.Delete();
    };
};

// Processa tutti gli input: verifica se i tasti rilevanti sono premuti/rilasciati in questo frame e reagisci di conseguenza
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
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

void renderModel(Shader_Class& ourModelShader, Model_Class& ourModel, float scale, float angle, Vec3F rotatore, float xTrasl, float yTrasl, float zTrasl, Mat4F view, Mat4F projectionCamera)
{
	Mat4F modelObject = Mat4F(1.0f);

	ourModelShader.use();
	ourModelShader.setMat4("projection", projectionCamera);
	ourModelShader.setMat4("view", view);

	modelObject = modelObject.scaling(modelObject, scale);
	modelObject = modelObject.translation(modelObject, xTrasl, yTrasl, zTrasl);
	modelObject = modelObject.rotateAllAxis(modelObject, angle, rotatore);

	ourModelShader.setMat4("model", modelObject);

	ourModelShader.setVec3("viewPos", camera.Posizione);

	//proprietà dei materiali
	ourModelShader.setFloat("material.shininess", 32.0f);

	ourModelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	ourModelShader.setVec3("dirLight.ambient", 0.025f, 0.025f, 0.025f);
	ourModelShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
	ourModelShader.setVec3("dirLight.specular", 0.25f, 0.25f, 0.25f);

	for (int i = 0; i < 1; i++)
	{
		ourModelShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.5f, 0.5f, 0.5f);
		ourModelShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
		ourModelShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
		ourModelShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
		ourModelShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.04f);
		ourModelShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.02f);
	}

	ourModel.Draw(ourModelShader);
}

