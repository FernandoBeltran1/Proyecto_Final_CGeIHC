#include "Window.h"
#include <cmath>

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

	// Posicion y velocidad locomotora
	locoPos = 0.0f;
	locoVel = 0.0f;
	locoAdelante = false;
	locoAtras = false;

	// Posicion y banderas de prender nave
	posNave = 0.0f;
	estadoNave = 1;

	// ========== Practica (8) =========== //
	/*posPezX = 0.0f;
	posPezY = 0.0f;

	pezSubiendo = true;
	direccionPez = 1;

	spotDirPez = 1;

	luzLampara = false;
	luzPez = false;
	*/
	// ========== Bangboo ========== //
	bangbooPosX = -2.0f;  // posición inicial igual a la que tenías en ProyectoFinal.cpp
	bangbooPosZ = 5.0f;
	// ============================= //
	// ===================================== //

	// Manato
	manatoPosX = 0.0f;
	manatoPosZ = 15.0f;
	manatoAngle = 0.0f;
	manatoDir = 1;
	manatoMoving = false;
	manatoWalkTime = 0.0f;

	OpcionColorCoche = 0;
	valor_switch = false;

	// Ejercicio clase
	posCoche = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	// Inicializacion de GLFW
	if (!glfwInit())
	{
		printf("Fallo inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Practica08: Iluminacion II", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);
	createCallbacks();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo inicializacion de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, bufferWidth, bufferHeight);
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

	// ------------------ Locomotora -----------------------
	if (key == GLFW_KEY_Y)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			theWindow->locoAdelante = true;
		else if (action == GLFW_RELEASE)
			theWindow->locoAdelante = false;
	}
	if (key == GLFW_KEY_G)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			theWindow->locoAtras = true;
		else if (action == GLFW_RELEASE)
			theWindow->locoAtras = false;
	}
	// ------------------ Locomotora FIN -------------------

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
	/*if (key == GLFW_KEY_J)
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

	// ========== Practica (8) Ejercicio3 luces separadas =========== //
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		theWindow->luzLampara = !theWindow->luzLampara;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		theWindow->luzPez = !theWindow->luzPez;
	}
	// =============================================================== //

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
	*/
	// Registro general de teclas (necesario para la camara y getsKeys())
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			theWindow->keys[key] = true;
		else if (action == GLFW_RELEASE)
			theWindow->keys[key] = false;
	}

	// ------------------ Movimiento MANATO -----------------------
// M = adelante, N = atrás, B = izquierda, H = derecha
	if (key == GLFW_KEY_M)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->manatoMoving = true;
			theWindow->manatoDir = 1;
			theWindow->manatoAngle = 0.0f;
		}
		else if (action == GLFW_RELEASE)
			theWindow->manatoMoving = false;
	}
	if (key == GLFW_KEY_N)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->manatoMoving = true;
			theWindow->manatoDir = -1;
			theWindow->manatoAngle = 180.0f;
		}
		else if (action == GLFW_RELEASE)
			theWindow->manatoMoving = false;
	}
	if (key == GLFW_KEY_B)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->manatoMoving = true;
			theWindow->manatoDir = 2;
			theWindow->manatoAngle = 270.0f;
		}
		else if (action == GLFW_RELEASE)
			theWindow->manatoMoving = false;
	}
	if (key == GLFW_KEY_H)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->manatoMoving = true;
			theWindow->manatoDir = -2;
			theWindow->manatoAngle = 90.0f;
		}
		else if (action == GLFW_RELEASE)
			theWindow->manatoMoving = false;
	}
	// ------------------ Movimiento MANATO FIN -----------------------
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

void Window::updateLoco(GLfloat deltaTime)
{
	if (locoAdelante)
		locoVel += LOCO_ACELERACION * deltaTime;
	else if (locoAtras)
		locoVel -= LOCO_ACELERACION * deltaTime;
	else
	{
		if (locoVel > 0.0f)
			locoVel -= LOCO_FRICCION * deltaTime;
		else if (locoVel < 0.0f)
			locoVel += LOCO_FRICCION * deltaTime;

		if (locoVel > -0.05f && locoVel < 0.05f)
			locoVel = 0.0f;
	}

	if (locoVel > LOCO_VEL_MAX) locoVel = LOCO_VEL_MAX;
	if (locoVel < -LOCO_VEL_MAX) locoVel = -LOCO_VEL_MAX;

	locoPos += locoVel * deltaTime;

	if (locoPos >= LOCO_LARGO_VIA)
	{
		locoPos = LOCO_LARGO_VIA;  // tope al final
		locoVel = 0.0f;            // frena completamente
	}
	if (locoPos <= 0.0f)
	{
		locoPos = 0.0f;            // tope al inicio
		locoVel = 0.0f;            // frena completamente
	}
}

void Window::updateBangboo(GLfloat deltaTime)
{
	if (keys[GLFW_KEY_LEFT])
		bangbooPosX -= BANGBOO_VEL * deltaTime;

	if (keys[GLFW_KEY_RIGHT])
		bangbooPosX += BANGBOO_VEL * deltaTime;

	if (keys[GLFW_KEY_UP])
		bangbooPosZ += BANGBOO_VEL * deltaTime;

	if (keys[GLFW_KEY_DOWN])
		bangbooPosZ -= BANGBOO_VEL * deltaTime;
}

void Window::updateManato(GLfloat deltaTime)
{
	bool arriba = keys[GLFW_KEY_M];
	bool abajo = keys[GLFW_KEY_N];
	bool izquierda = keys[GLFW_KEY_B];
	bool derecha = keys[GLFW_KEY_H];

	manatoMoving = arriba || abajo || izquierda || derecha;

	if (!manatoMoving)
	{
		manatoWalkTime = 0.0f;
		return;
	}

	manatoWalkTime += deltaTime * 4.0f;

	float dx = 0.0f, dz = 0.0f;

	if (arriba)    dz += MANATO_VEL * deltaTime;
	if (abajo)     dz -= MANATO_VEL * deltaTime;
	if (derecha)   dx += MANATO_VEL * deltaTime;
	if (izquierda) dx -= MANATO_VEL * deltaTime;

	// Calcula el ángulo según la dirección
	if (dx != 0.0f || dz != 0.0f)
	{
		manatoAngle = atan2(dx, dz) * (180.0f / 3.14159265f);
	}

	manatoPosX += dx;
	manatoPosZ += dz;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}