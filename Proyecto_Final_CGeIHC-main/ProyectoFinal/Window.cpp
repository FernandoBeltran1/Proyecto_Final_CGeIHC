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

	locoPos = 0.0f;
	locoVel = 0.0f;
	locoHaciaAdelante = true;   // arranca yendo hacia adelante

	// ========== Bangboo ========== //
	bangbooPosX = -2.0f;  // posici�n inicial igual a la que ten�as en ProyectoFinal.cpp
	bangbooPosZ = 5.0f;

	// Manato
	manatoPosX = 0.0f;
	manatoPosZ = 15.0f;
	manatoAngle = 0.0f;
	manatoDir = 1;
	manatoMoving = false;
	manatoWalkTime = 0.0f;


	// ============================= FLAGS ======================
	flagAnimacionZeppelin = false;

	// -------- Spotlights --------
	flagLuzEstatua = false;
	flagLuzTren = false;

	// ============================= C�mara ======================

	camaraActual = 1;
	vistaEstaticaActual = 0;

	// ==========================================================

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
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
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
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

	// (Locomotora ahora con animación automática — sin control por teclado)


	// ------------------ Movimiento MANATO -----------------------
// M = adelante, N = atr�s, B = izquierda, H = derecha
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



	// ------------------ Movimiento Zeppelin -----------------------
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		theWindow->flagAnimacionZeppelin = !theWindow->flagAnimacionZeppelin;
	}

	// ------------------ Prende apaga spotlights -----------------------
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		theWindow->flagLuzTren = !theWindow->flagLuzTren;
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		theWindow->flagLuzEstatua = !theWindow->flagLuzEstatua;
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


	// ------------------ Cambio de camaras -----------------------
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		theWindow->camaraActual = 1;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		theWindow->camaraActual = 2;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		theWindow->camaraActual = 3;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		if (theWindow->camaraActual == 4) {
			theWindow->vistaEstaticaActual++;

			if (theWindow->vistaEstaticaActual > 2) {
				theWindow->vistaEstaticaActual = 0;
			}
		}
		else {
			theWindow->camaraActual = 4;
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

void Window::updateLoco(GLfloat deltaTime)
{
	// Aceleración automática según la dirección actual
	if (locoHaciaAdelante)
		locoVel += LOCO_ACELERACION * deltaTime;
	else
		locoVel -= LOCO_ACELERACION * deltaTime;

	// Limitar velocidad
	if (locoVel > LOCO_VEL_MAX) locoVel = LOCO_VEL_MAX;
	if (locoVel < -LOCO_VEL_MAX) locoVel = -LOCO_VEL_MAX;

	locoPos += locoVel * deltaTime;

	// Al llegar al final → invertir dirección
	if (locoPos >= LOCO_LARGO_VIA)
	{
		locoPos = LOCO_LARGO_VIA;
		locoVel = 0.0f;
		locoHaciaAdelante = false;
	}
	// Al llegar al inicio → invertir dirección
	if (locoPos <= 0.0f)
	{
		locoPos = 0.0f;
		locoVel = 0.0f;
		locoHaciaAdelante = true;
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

	// Calcula el �ngulo seg�n la direcci�n
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
