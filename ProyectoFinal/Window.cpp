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
	locoAdelante = false;
	locoAtras = false;

	// ========== Bangboo ========== //
	bangbooPosX = -2.0f;  // posición inicial igual a la que tenías en ProyectoFinal.cpp
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

	// ============================= Cámara ======================

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
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final", NULL, NULL);

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



	// ------------------ Movimiento Zeppelin -----------------------
	if (key == GLFW_KEY_Z && action == GLFW_PRESS){
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
