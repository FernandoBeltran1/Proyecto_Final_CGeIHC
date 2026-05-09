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

