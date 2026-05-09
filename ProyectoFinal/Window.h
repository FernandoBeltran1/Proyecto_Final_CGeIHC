#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	
	// ============================ Entradas por teclado ==========================
	// ---- animacion zeppelin
	bool animacionZeppelin() { return flagAnimacionZeppelin; }

	bool luzTren() { return flagLuzTren; }
	bool luzEstatua() { return flagLuzEstatua; }

	
	// ============================ Camara por teclado ==========================
	int getCamaraActual() { return camaraActual; }
	int getVistaEstaticaActual() { return vistaEstaticaActual; }
		
	// ============================================================================
		// Locomotora
	GLfloat getLocoPosZ() { return locoPos; }
	GLfloat getLocoVel() { return locoVel; }
	void    updateLoco(GLfloat deltaTime);

	// Bangboo
	GLfloat getBangbooPosX() { return bangbooPosX; }
	GLfloat getBangbooPosZ() { return bangbooPosZ; }
	void updateBangboo(GLfloat deltaTime);

	// Manato
	GLfloat getManatoX() { return manatoPosX; }
	GLfloat getManatoZ() { return manatoPosZ; }
	GLfloat getManatoAngle() { return manatoAngle; }
	GLint   getManatoDir() { return manatoDir; }
	bool    getManatoMoving() { return manatoMoving; }
	GLfloat getManatoWalkTime() { return manatoWalkTime; }
	void    updateManato(GLfloat deltaTime);



	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;

	// -----------------
	GLfloat locoVel;
	GLfloat locoPos;
	bool locoAdelante;
	bool locoAtras;
	const float LOCO_VEL_MAX = 2.0f;
	const float LOCO_ACELERACION = 0.5f;
	const float LOCO_FRICCION = 0.5f;
	const float LOCO_LARGO_VIA = 180.0f;
	bool valor_switch;
	// ========== Bangboo ========== //
	GLfloat bangbooPosX;
	GLfloat bangbooPosZ;
	const float BANGBOO_VEL = 0.8f;

	// Manato
	GLfloat manatoPosX;
	GLfloat manatoPosZ;
	GLfloat manatoAngle;   // ángulo de rotación en Y (grados)
	GLint   manatoDir;     // 1=adelante, -1=atrás, 2=izq, -2=der
	bool    manatoMoving;
	GLfloat manatoWalkTime; // acumula tiempo para ciclo de caminata
	const float MANATO_VEL = 0.5f;


	
	// ============================ Entradas por teclado ==========================
	bool flagAnimacionZeppelin;
	bool flagLuzTren, flagLuzEstatua;

	// ============================ camara por teclado ==========================
	int camaraActual;
	int vistaEstaticaActual;
	
	// ========================================= //


	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

