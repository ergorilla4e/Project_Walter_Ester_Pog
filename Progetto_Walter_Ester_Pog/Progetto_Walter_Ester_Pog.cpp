#define STB_IMAGE_IMPLEMENTATION

#include "Generale.h"
#include "View_Class.h"

int main()
{
	// Inizializza e configura GLFW
// -------------------------------------------------------------------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// Crea la finestra e controlla se ci e' riuscito
// -------------------------------------------------------------------------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Progetto_Lista_Pettinari", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Errore: Impossibile creare la finestra GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Carica tutte le funzioni di OpenGL tramite glad
// -------------------------------------------------------------------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Errore: Impossibile inizializzare GLAD" << std::endl;
		return -1;
	}

	// Crea nella heap l'oggetto che esegue il programma, al termine del main libera la memoria occupata automaticamente
// -------------------------------------------------------------------------------------------------------------------------
	unique_ptr<View_Class> PlayView(new View_Class(window));

// -------------------------------------------------------------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
