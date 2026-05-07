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
	
	//Nave Banderas
	GLfloat getPosNave() { return posNave; }
	GLint prendeLuzNave() { return estadoNave; }
	
	//GLfloat getPosCoche() { return posCoche; }
	GLfloat MovingPezX() { return posPezX; }
	GLfloat MovingPezY() { return posPezY; }
	GLint getDireccionPez() { return direccionPez; }
	GLint spotLightDirPez() { return spotDirPez;  }

	GLint changeLuzCoche() { return OpcionColorCoche; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	//bool switch_lampara() { return valor_switch; }
	
	bool switch_lampara() { return luzLampara; }
	bool switch_pez() { return luzPez; }

	// Locomotora
	GLfloat getLocoPosZ() { return locoPos; }
	void    updateLoco(GLfloat deltaTime);

	// Bangboo
	GLfloat getBangbooPosX() { return bangbooPosX; }
	GLfloat getBangbooPosZ() { return bangbooPosZ; }
	void updateBangboo(GLfloat deltaTime);
	
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
	GLfloat posCoche, posNave;
	GLint OpcionColorCoche, estadoNave;

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

	// ========== Práctica (8) =========== //
	GLfloat posPezX, posPezY;
	bool moverIzquierda, moverDerecha;
	bool pezSubiendo;
	int direccionPez, spotDirPez;
	// ========================================= //
	// Ejercicio3
	// 3. - Agregar una luz de tipo puntual  de color azul ligada 
	// al bulbo del pez que puedan prender y apagar de forma 
	// independiente con teclado tanto la luz de la lámpara de su 
	// práctica 7 como esta luz(la luz de la lámpara debe de ser puntual, 
	// si la crearon spotlight en su reporte 7 tienen que cambiarla a 
	// luz puntual), de tal forma que se pueda ver : las 2 luces apagadas, 
	// las 2 luces prendidas, una luz prendida y una luz apagada y viceversa.
	bool luzLampara;
	bool luzPez;
	
	
	
	// ========================================= //



	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

