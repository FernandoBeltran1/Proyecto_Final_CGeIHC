/*
Práctica 8: Iluminación 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


// ================== TEXTURAS ======================= //

Texture pisoTexture;

// ================== MODELOS ======================= //

Model lampara;
Model letrero;
Model castillo;
Model arbol;
Model librero_medieval;

// Modelos separados de las vías del tren
Model Vias_Madera;    // Durmientes de madera  -> Textures/Madera.jpg
Model Vias_Rieles;    // Rieles metálicos       -> Textures/Metal.jpg
Model Vias_Uniones;   // Uniones/tornillos      -> Textures/tronco.jpg

//=====================Locomotora==============
Model Locomotora;
GLfloat locomotora_posZ = 0.0f;   // posición actual sobre las vías
GLfloat locomotora_velZ = 0.0f;   // velocidad actual
const float LOCO_VEL_MAX = 15.0f;  // unidades/seg máximo
const float LOCO_ACELERACION = 8.0f; // aceleración al presionar tecla
const float LOCO_FRICCION = 4.0f;  // frenado al soltar
const float LOCO_LARGO_VIA = 9.0f * 20.0f; // longitud total del circuito (20 tramos)


//Modelo del avatar principal
Model bangboo;

//Peronaje MANATO Principal 1
Model Manato_Cuerpo;
Model Manato_BrazoDerecho;
Model Manato_BrazoIzquierdo;
Model Manato_AntebrazoDerecho;
Model Manato_AntebrazoIzquierdo;
Model Manato_PiernaDerecha;
Model Manato_PiernaIzquierda;
Model Manato_Cola;

//Arboles cuadrados para el escenario
Model Tronco_arbol;
Model Hojas_arbol;

//Modelos del segundo NPC
Model Gummy_Body;
Model Gummy_RightArm;
Model Gummy_LeftArm;
Model Gummy_Tail;
Model Gummy_Hat;

// ================== SISTEMA DE HUMO ======================= //
struct Particula {
	glm::vec3 pos;
	glm::vec3 vel;
	float vida;
	float vidaMax;
	float tamanio;
	float alpha;
};
const int MAX_PARTICULAS = 60;
Particula particulas[MAX_PARTICULAS];
Texture humoTexture;
Mesh* humoQuad;
bool humoActivo = false;
float humoIntensidad = 1.0f;

void inicializarParticula(Particula& p, glm::vec3 posChimenea) {
	p.pos = posChimenea;
	p.vel = glm::vec3(
		((rand() % 100) / 100.0f - 0.5f) * 0.3f,
		0.8f + ((rand() % 100) / 100.0f) * 0.5f,
		((rand() % 100) / 100.0f - 0.5f) * 0.3f
	);
	p.vidaMax = 1.5f + ((rand() % 100) / 100.0f);
	p.vida = p.vidaMax;
	p.tamanio = 0.3f + ((rand() % 100) / 100.0f) * 0.4f;
	p.alpha = 1.0f;
}
// ========================Lycaon============================ //
Model Lycaon;
// ================== LUCES ======================= //

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// ========================================= //


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;



// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	
	// 0
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	// 1
	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	// 2 Piso
	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	unsigned int humoIndices[] = { 0, 1, 2, 2, 3, 0 };
	GLfloat humoVertices[] = {
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f
	};
	humoQuad = new Mesh();
	humoQuad->CreateMesh(humoVertices, humoIndices, 32, 6);
	for (int i = 0; i < MAX_PARTICULAS; i++)
		particulas[i].vida = 0.0f;
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// back
		8, 9, 10,
		10, 11, 8,
		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
		// right
		4, 5, 6,
		6, 7, 4,
	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	1.0f,	//3
		// right
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		// left
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		// bottom
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		// top
		-0.5f, 0.5f,  0.5f,		0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		-0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);
}


int main()
{
	Window mainWindow(1366, 768);
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	// ================== TEXTURAS ======================= //
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	humoTexture = Texture("Textures/humo.png");
	humoTexture.LoadTextureA();

	// ================== MODELOS ======================= //

	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");

	letrero = Model();
	letrero.LoadModel("Models/letrero.obj");

	castillo = Model();
	castillo.LoadModel("Models/castillo.obj");

	librero_medieval = Model();
	librero_medieval.LoadModel("Models/librero.obj");

	arbol = Model();
	arbol.LoadModel("Models/arbol.obj");

	// -------------------- VÍAS DEL TREN -------------------- //

	Vias_Madera = Model();
	Vias_Madera.LoadModel("Models/Madera_vias.obj");   // Durmientes -> Madera.jpg

	Vias_Rieles = Model();
	Vias_Rieles.LoadModel("Models/Rieles_vias.obj");   // Rieles     -> Metal.jpg

	Vias_Uniones = Model();
	Vias_Uniones.LoadModel("Models/Uniones_vias.obj"); // Uniones    -> tronco.jpg

	// ==================Modelo de la locomotora===================== //
	Locomotora = Model();
	Locomotora.LoadModel("Models/Locomotora.obj");

	//---------------------Avatar principal--------------------

	bangboo = Model();
	bangboo.LoadModel("Models/bangboo.obj");

	//--------------MANATO PERSONAJE------------------------
	Manato_Cuerpo = Model();
	Manato_Cuerpo.LoadModel("Models/Cuerpo_Manato.obj");

	Manato_BrazoDerecho = Model();
	Manato_BrazoDerecho.LoadModel("Models/BrazoDerecho_Manato.obj");

	Manato_BrazoIzquierdo = Model();
	Manato_BrazoIzquierdo.LoadModel("Models/BrazoIzquierdo_Manato.obj");

	Manato_AntebrazoDerecho = Model();
	Manato_AntebrazoDerecho.LoadModel("Models/AntebrazoDerecho_Manato.obj");

	Manato_AntebrazoIzquierdo = Model();
	Manato_AntebrazoIzquierdo.LoadModel("Models/AntebrazoIzquierdo_Manato.obj");

	Manato_PiernaDerecha = Model();
	Manato_PiernaDerecha.LoadModel("Models/PiernaDerecha_Manato.obj");

	Manato_PiernaIzquierda = Model();
	Manato_PiernaIzquierda.LoadModel("Models/PiernaIzquierda_Manato.obj");

	Manato_Cola = Model();
	Manato_Cola.LoadModel("Models/Cola_Manato.obj");

	Lycaon = Model();
	Lycaon.LoadModel("Models/Lycaon.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/pos_rt.jpg");
	skyboxFaces.push_back("Textures/Skybox/neg_lf.jpg");
	skyboxFaces.push_back("Textures/Skybox/neg_dn.jpg");
	skyboxFaces.push_back("Textures/Skybox/pos_up.jpg");
	skyboxFaces.push_back("Textures/Skybox/pos_bk.jpg");
	skyboxFaces.push_back("Textures/Skybox/neg_ft.jpg");

	//Arbol cargado
	Tronco_arbol.LoadModel("Models/Tronco_arbol.obj");
	Hojas_arbol.LoadModel("Models/Hojas_arbol.obj");

	//Gummygo
	Gummy_Body.LoadModel("Models/Gummygo_Body.obj");
	Gummy_RightArm.LoadModel("Models/Gummygo_RightArm.obj");
	Gummy_LeftArm.LoadModel("Models/Gummygo_LeftArm.obj");
	Gummy_Tail.LoadModel("Models/Gummygo_tail.obj");
	Gummy_Hat.LoadModel("Models/Hat.obj");
	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ================== LUCES ======================= //
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	// ============== POINTLIGHTS ==================== //
	unsigned int pointLightCount = 0;
	// Lámpara color blanco
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// ============== SPOTLIGHTS ===================== //
	unsigned int spotLightCount = 0;
	// Linterna pegada a la cámara
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount++;

	// ================================================ //

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	GLfloat now = 0.0f;
	glm::vec3 lowerLight;

	// =================== Variables ================= //
	glm::mat4 model(1.0);
	glm::mat4 identidad(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float separacion = 30.0f;
	GLfloat bangboo_flotacion = 0.0f;

	// ── Animación procedural de Manato
	float manato_bodyBob = 0.0f;
	float manato_bodySwing = 0.0f;
	float manato_legSwing = 0.0f;
	float manato_armSwing = 0.0f;

	float mwt = 0.0f; // tiempo acumulado de caminata

	// Animación NPC Gummy
	float gummy_armAngle = 0.0f;
	float gummy_tailAngle = 0.0f;
	float gummy_tiempo = 0.0f;

	// Variables humo
	bool humoActivo = false;
	float humoIntensidad = 1.0f;

	identidad = glm::mat4(1.0);

	// =================== ANIMACION ================= //
	glm::vec3 pos_obj;


	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		// Información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// ================================================================= LUCES INICIO ================================================================

		// MAIN
		shaderList[0].SetDirectionalLight(&mainLight);

		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// SPOTLIGHTS
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// POINTLIGHTS
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		// ================================================================= LUCES FIN ================================================================

		// ----------------------------------------- PISO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ================================================== Proyecto final  ================================================================= //

		// ----------------------------------------- LAMPARA -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(30.0, -1.0f, 0.0f));
		pos_obj = glm::vec3(model[3]);
		pos_obj.y += 7.0f;
		pointLights[0].SetPos(pos_obj);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		// ----------------------------------------- LETRERO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(60.0, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		letrero.RenderModel();

		// ----------------------------------------- LIBRERO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(90.0, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		librero_medieval.RenderModel();

		// ----------------------------------------- ARBOL -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(120.0, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		// ----------------------------------------- CASTILLO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(150.0, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		castillo.RenderModel();

		// ==================================================================================================================================== //


		// ========================== VÍAS DEL TREN ========================== //
		
		float separacion = 9.0f; // ajusta según el tamaño de tu modelo

		for (int i = 0; i < 20; i++)
		{
			model = identidad;
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, i * separacion));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			// ===================================================================================================================
			// --- Durmientes de madera (Madera.jpg) ---
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Vias_Madera.RenderModel();

			// --- Rieles metálicos (Metal.jpg) ---
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Vias_Rieles.RenderModel();

		}

		// ----------------------------------------- ARBOLES EN AMBOS LADOS DE LAS VIAS -----------------------------------------
		for (int i = 0; i < 12; i++)
		{
			// --- Lado DERECHO ---
			model = identidad;
			model = glm::translate(model, glm::vec3(6.0f, -1.0f, i * 15.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Tronco_arbol.RenderModel();
			Hojas_arbol.RenderModel();

			// --- Lado IZQUIERDO ---
			model = identidad;
			model = glm::translate(model, glm::vec3(-6.0f, -1.0f, i * 15.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Tronco_arbol.RenderModel();
			Hojas_arbol.RenderModel();
		}
		// =======================================================================================================================
		// ===================================================================================================================

		// ----------------------------------------- BANGBOO -----------------------------------------
		mainWindow.updateBangboo(deltaTime);
		bangboo_flotacion += deltaTime * 0.05f;

		model = identidad;
		model = glm::translate(model, glm::vec3(
			mainWindow.getBangbooPosX(),
			0.5f + sin(bangboo_flotacion) * 1.2f,
			mainWindow.getBangbooPosZ()
		));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bangboo.RenderModel();
		// ===========================================================================================
		// ==================================================================== //


		// ========================== LOCOMOTORA ========================== //
		mainWindow.updateLoco(deltaTime);

		model = identidad;
		model = glm::translate(model, glm::vec3(2.0f, 1.4f, mainWindow.getLocoPosZ()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Locomotora.RenderModel();
		// ================================================================ //

		// ========================== HUMO CHIMENEA ========================== //
		glm::vec3 posChimenea = glm::vec3(2.0f, 4.5f, mainWindow.getLocoPosZ() + 1.5f);

		if (mainWindow.getLocoVel() != 0.0f)
			humoActivo = true;

		if (mainWindow.getLocoPosZ() >= LOCO_LARGO_VIA - 1.0f)
			humoIntensidad -= deltaTime * 0.3f;

		// Reiniciar cuando el tren vuelve al inicio
		if (mainWindow.getLocoPosZ() <= 0.1f && humoIntensidad <= 0.0f)
		{
			humoActivo = false;
			humoIntensidad = 1.0f;
			for (int i = 0; i < MAX_PARTICULAS; i++)
				particulas[i].vida = 0.0f;  // matar todas las partículas vivas
		}

		humoIntensidad = glm::clamp(humoIntensidad, 0.0f, 1.0f);

		if (humoActivo && humoIntensidad > 0.0f) {
			int emitir = (int)(humoIntensidad * 2);
			for (int i = 0; i < MAX_PARTICULAS && emitir > 0; i++) {
				if (particulas[i].vida <= 0.0f) {
					inicializarParticula(particulas[i], posChimenea);
					emitir--;
				}
			}
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		humoTexture.UseTexture();

		for (int i = 0; i < MAX_PARTICULAS; i++) {
			if (particulas[i].vida <= 0.0f) continue;
			particulas[i].vida -= deltaTime;
			particulas[i].pos += particulas[i].vel * deltaTime;
			particulas[i].vel.x *= 0.98f;
			particulas[i].vel.z *= 0.98f;
			float t = particulas[i].vida / particulas[i].vidaMax;
			particulas[i].alpha = t * humoIntensidad;

			glm::mat4 view = camera.calculateViewMatrix();
			model = identidad;
			model = glm::translate(model, particulas[i].pos);
			model[0][0] = view[0][0]; model[0][1] = view[1][0]; model[0][2] = view[2][0];
			model[1][0] = view[0][1]; model[1][1] = view[1][1]; model[1][2] = view[2][1];
			model[2][0] = view[0][2]; model[2][1] = view[1][2]; model[2][2] = view[2][2];
			float s = particulas[i].tamanio * (2.0f - t);
			model = glm::scale(model, glm::vec3(s, s, s));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glm::vec3 colorHumo = glm::vec3(particulas[i].alpha);
			glUniform3fv(uniformColor, 1, glm::value_ptr(colorHumo));
			humoQuad->RenderMesh();
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		// =================================================================== //

		// ----------------------------------------- MANATO -----------------------------------------
		mainWindow.updateManato(deltaTime);
		mwt = mainWindow.getManatoWalkTime();

		if (mainWindow.getManatoMoving())
		{
			manato_bodyBob = sin(mwt * 2.0f) * 0.06f;
			manato_bodySwing = sin(mwt * 2.0f) * 2.5f;
			manato_legSwing = sin(mwt * 2.0f) * 25.0f;
			manato_armSwing = -sin(mwt * 2.0f) * 20.0f;
		}
		else
		{
			manato_bodyBob *= 0.85f;
			manato_bodySwing *= 0.85f;
			manato_legSwing *= 0.85f;
			manato_armSwing *= 0.85f;
		}

		// Posición base de Manato
		glm::vec3 manato_pos = glm::vec3(
			mainWindow.getManatoX(),
			-1.0f + manato_bodyBob,
			mainWindow.getManatoZ()
		);
		float manato_ang = mainWindow.getManatoAngle();

		// --- CUERPO ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, manato_bodySwing * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_Cuerpo.RenderModel();

		// --- BRAZO DERECHO ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-0.238f, 1.494f, 0.0f));
		model = glm::rotate(model, manato_armSwing * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.238f, -1.494f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_BrazoDerecho.RenderModel();

		// --- BRAZO IZQUIERDO ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.240f, 1.493f, 0.0f));
		model = glm::rotate(model, -manato_armSwing * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.240f, -1.493f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_BrazoIzquierdo.RenderModel();

		// --- ANTEBRAZO DERECHO ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-0.376f, 1.315f, 0.0f));
		model = glm::rotate(model, manato_armSwing * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.376f, -1.315f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_AntebrazoDerecho.RenderModel();

		// --- ANTEBRAZO IZQUIERDO ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.407f, 1.296f, 0.0f));
		model = glm::rotate(model, -manato_armSwing * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.407f, -1.296f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_AntebrazoIzquierdo.RenderModel();

		// --- PIERNA DERECHA ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-0.143f, 0.975f, 0.0f));
		model = glm::rotate(model, manato_legSwing * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.143f, -0.975f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_PiernaDerecha.RenderModel();

		// --- PIERNA IZQUIERDA ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.123f, 0.968f, 0.0f));
		model = glm::rotate(model, -manato_legSwing * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.123f, -0.968f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_PiernaIzquierda.RenderModel();

		// --- COLA ---
		model = identidad;
		model = glm::translate(model, manato_pos);
		model = glm::rotate(model, manato_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-0.007f, 1.139f, 0.0f));
		model = glm::rotate(model, manato_bodySwing * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.007f, -1.139f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Manato_Cola.RenderModel();
		// ===========================================================================================
		// ===========================================================================================

		// ----------------------------------------- NPC GUMMY -----------------------------------------
		gummy_tiempo += deltaTime * 0.1f;

		// Brazo derecho: saluda inclinándose con el sombrero (oscila entre -30° y -90°)
		gummy_armAngle = 45.0f + sin(gummy_tiempo * 2.5f) * 45.0f;

		// Cola: oscila suavemente izquierda y derecha (±15°)
		gummy_tailAngle = sin(gummy_tiempo * 1.8f) * 15.0f;

		glm::vec3 gummyPos = glm::vec3(8.0f, -1.0f, 20.0f); // ajusta posición en la escena

		// --- Cuerpo ---
		model = identidad;
		model = glm::translate(model, gummyPos);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Gummy_Body.RenderModel();

		// --- Brazo izquierdo (animado, saluda arriba y abajo) ---
		model = identidad;
		model = glm::translate(model, gummyPos);
		model = glm::translate(model, glm::vec3(0.422f, 2.179f, 0.0f));      // pivote hombro izquierdo
		model = glm::rotate(model, gummy_armAngle * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // rotar en X
		model = glm::translate(model, glm::vec3(-0.422f, -2.179f, 0.0f));    // regreso al origen
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Gummy_LeftArm.RenderModel();

		// --- Brazo derecho (animado, saluda arriba y abajo) ---
		model = identidad;
		model = glm::translate(model, gummyPos);
		model = glm::translate(model, glm::vec3(-0.349f, 2.152f, 0.0f));    // pivot en hombro izquierdo
		model = glm::rotate(model, gummy_armAngle * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // rota en X
		model = glm::translate(model, glm::vec3(0.349f, -2.152f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Gummy_RightArm.RenderModel();

		// --- Sombrero (estático en la cabeza) ---
		model = identidad;
		model = glm::translate(model, gummyPos);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Gummy_Hat.RenderModel();

		// --- Cola (oscila en Y) ---
		model = identidad;
		model = glm::translate(model, gummyPos);
		model = glm::rotate(model, gummy_tailAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Gummy_Tail.RenderModel();
		// ===========================================================================================

		//Lycaon
		//Lycaon
		model = identidad;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 10.0f));  // frente a la cámara al inicio
		// Sin rotación extra — el modelo ya está orientado correctamente en Blender
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lycaon.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
