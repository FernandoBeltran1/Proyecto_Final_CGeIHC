#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;

	//Posicion y banderas de prender nave
	posNave = 0.0f;
	estadoNave = 1;

	// ========== Práctica (8) =========== //
	posPezX = 0.0f;
	posPezY = 0.0f;

	pezSubiendo = true;
	direccionPez = 1;

	spotDirPez = 1;

	luzLampara = false;
	luzPez = false;
	// ========================================= //


	OpcionColorCoche = 0;
	valor_switch = false;
	
	//Ejericio clase
	posCoche = 0.0f;
	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica08: Iluminacion II", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// ------------------ Movimiento Nave -----------------------
	if (key == GLFW_KEY_O)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->posNave -= 1.0f;
			theWindow->estadoNave = 1;
		}
	}

	if (key == GLFW_KEY_L)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->posNave += 1.0f;
			theWindow->estadoNave = -1;
		}
	}
	
	// ------------------ Movimiento Pez -----------------------
	if (key == GLFW_KEY_J)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->posPezX -= 0.1f;
			theWindow->direccionPez = -1;

			if (theWindow->posPezX < -7.0f)
				theWindow->posPezX = -7.0f;

			if (theWindow->posPezY >= 2.0f)
				theWindow->pezSubiendo = false;

			if (theWindow->posPezY <= -2.0f)
				theWindow->pezSubiendo = true;

			if (theWindow->pezSubiendo)
				theWindow->posPezY += 0.2f;
			else
				theWindow->posPezY -= 0.2f;
		}
	}
	if (key == GLFW_KEY_K)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->posPezX += 0.1f;
			theWindow->direccionPez = 1;

			if (theWindow->posPezX > 7.0f)
				theWindow->posPezX = 7.0f;

			if (theWindow->posPezY >= 2.0f)
				theWindow->pezSubiendo = false;

			if (theWindow->posPezY <= -2.0f)
				theWindow->pezSubiendo = true;

			if (theWindow->pezSubiendo)
				theWindow->posPezY += 0.2f;
			else
				theWindow->posPezY -= 0.2f;
		}
	}
	// ------------------ Movimiento Pez FIN -----------------------

	// ========== Práctica (8) Ejercicio3 tener luces separadas =========== //
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		theWindow->luzLampara = !theWindow->luzLampara;
		//printf("Luz lampara: %d'\n", theWindow->luzLampara);
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		theWindow->luzPez = !theWindow->luzPez;
		//printf("Luz pez: %d'\n", theWindow->luzPez);
	}
	// ========== Práctica (8) Ejercicio3 tener luces separadas =========== //

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		theWindow->spotDirPez = 1;
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		theWindow->spotDirPez = 2;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		theWindow->spotDirPez = 3;
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (theWindow->OpcionColorCoche >= 5) theWindow->OpcionColorCoche = 0; 
		else theWindow->OpcionColorCoche += 1;
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
