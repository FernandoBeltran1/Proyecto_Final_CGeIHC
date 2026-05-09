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
//para probar el importer
//#include<assimp/Importer.hpp>

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

Texture pisoTexture_piedra;
Texture pisoTexture_tierra;

Texture FuegoTexture;


// ================== MODELOS ======================= //

Model lampara;
Model letrero;
Model castillo;
Model arbol;
Model librero_medieval;

Model laboratorio_frank;
Model frank;
Model cientifico_cabeza;
Model cientifico_torso;
Model cientifico_brazo_der;
Model cientifico_brazo_izq;
Model cientifico_piernas;

Model zeppelin_base;
Model zeppelin_helice_abajo;
Model zeppelin_helice_arriba;

Model reflector;
Model estatua;


// ================== LUCES ======================= //

// luz direccional Dia
DirectionalLight mainLightDia;
DirectionalLight mainLightNoche;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

// Arreglos Spotlights
SpotLight spotLights1[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];
SpotLight spotLights3[MAX_SPOT_LIGHTS];
SpotLight spotLights4[MAX_SPOT_LIGHTS];


// ================== SKYBOX ======================= //

Skybox skyboxDia;
Skybox skyboxNoche;

float horarioDiaNoche = 0.0f;
bool flagDiaNoche = true;


// ================================================= //

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
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

	// ========================================= TEXTURAS ========================================= //
	unsigned int fuegoIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat fuegoVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f,

	};
	


	// ========================================= //
		
	// 0
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	// 1
	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	// 2 Piso
	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	// 3 Fuego 
	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(fuegoVertices, fuegoIndices, 64, 12);
	meshList.push_back(obj4);


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	//calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	//calcAverageNormals(aguaIndices, 6, aguaVertices, 64, 8, 5);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
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

	};	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	1.0f,	//3
		// right
		//x		y		z		S		T
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
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);  

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();
																					// ajuste velocidad camara 0.3f to 1.5f
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.5f, 0.5f);


	// ================== TEXTURAS ======================= //
	pisoTexture_piedra = Texture("Textures/piso_piedra.tga");
	pisoTexture_piedra.LoadTextureA();

	pisoTexture_tierra = Texture("Textures/piso_tierra.tga");
	pisoTexture_tierra.LoadTextureA();

	FuegoTexture = Texture("Textures/fuego.tga");
	FuegoTexture.LoadTextureA();
	
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

	zeppelin_base = Model();
	zeppelin_base.LoadModel("Models/zeppelin_base.obj");

	zeppelin_helice_abajo = Model();
	zeppelin_helice_abajo.LoadModel("Models/zeppelin_helice_abajo.obj");

	zeppelin_helice_arriba = Model();
	zeppelin_helice_arriba.LoadModel("Models/zeppelin_helice_arriba.obj");

	reflector = Model();
	reflector.LoadModel("Models/reflector.obj");

	laboratorio_frank = Model();
	laboratorio_frank.LoadModel("Models/laboratorio_frank.obj");

	cientifico_torso = Model();
	cientifico_torso.LoadModel("Models/cientifico_torso.obj");

	cientifico_cabeza = Model();
	cientifico_cabeza.LoadModel("Models/cientifico_cabezaT.obj");

	cientifico_brazo_der = Model();
	cientifico_brazo_der.LoadModel("Models/cientifico_brazo_der.obj");

	cientifico_brazo_izq = Model();
	cientifico_brazo_izq.LoadModel("Models/cientifico_brazo_izq.obj");

	cientifico_piernas = Model();
	cientifico_piernas.LoadModel("Models/cientifico_cuerpo_abajo.obj");

	frank = Model();
	frank.LoadModel("Models/franki.obj");

	estatua = Model();
	estatua.LoadModel("Models/estatua.obj");
	// ========================================= //
		
	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");*/

	// ================== SKYBOX DIA ======================= //
	std::vector<std::string> skyboxFacesDia;

	skyboxFacesDia.push_back("Textures/Skybox/Dia/pos_rt.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/neg_lf.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/neg_dn.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/pos_up.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/pos_bk.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/neg_ft.jpg");

	skyboxDia = Skybox(skyboxFacesDia);

	// ================== SKYBOX NOCHE ======================= //
	std::vector<std::string> skyboxFacesNoche;

	skyboxFacesNoche.push_back("Textures/Skybox/Noche/pos_rt.png");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche/neg_lf.png");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche/neg_dn.png");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche/pos_up.png");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche/pos_bk.png");
	skyboxFacesNoche.push_back("Textures/Skybox/Noche/neg_ft.png");

	skyboxNoche = Skybox(skyboxFacesNoche);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ================== LUCES ======================= //
	// 
	// ============== Directional Light ==================== //
	//luz direccional Dia
	mainLightDia = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	mainLightNoche = DirectionalLight(0.4f, 0.4f, 0.6f,
		0.35f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//luz direccional Noche
	
	// ============== POINTLIGHTS ==================== //
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	// ---------------------------------------- Lampara blanca
	pointLights[0] = PointLight(0.992f, 0.925f, 0.686f,  //Lampara color blanco
		0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.025f, 0.003f);
	pointLightCount++;

	pointLights[1] = PointLight(0.992f, 0.925f, 0.686f,  //Lampara color blanco
		0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.025f, 0.003f);
	pointLightCount++;

	pointLights[2] = PointLight(0.992f, 0.925f, 0.686f,  //Lampara color blanco
		0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.025f, 0.003f);
	pointLightCount++;

	//pointLights[3] = PointLight(1.0f, 0.698f, 0.0f,  //Lampara china
	//	0.0f, 5.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	// ================================================== SPOTLIGHTS ===================================== //
	
	// Contador de spotlights
	unsigned int spotLightCount1 = 0;
	unsigned int spotLightCount2 = 0;
	unsigned int spotLightCount3 = 0;
	unsigned int spotLightCount4 = 0;

	// --------------------- Arreglo 1 -----------------
	//linterna pegada a la cámara siempre VA
	spotLights1[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,  // Pos
		0.0f, -1.0f, 0.0f, // DIR
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount1++;

	// Tren
	spotLights1[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-2.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount1++;

	// Estatua
	spotLights1[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-2.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount1++;
	// -------------------------------------------------

	// --------------------- Arreglo 2 -----------------
	//linterna pegada a la cámara siempre VA
	spotLights2[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,  // Pos
		0.0f, -1.0f, 0.0f, // DIR
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount2++;

	// Tren
	spotLights2[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-2.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount2++;

	// -------------------------------------------------

	// --------------------- Arreglo 3 -----------------
	//linterna pegada a la cámara siempre VA
	spotLights3[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,  // Pos
		0.0f, -1.0f, 0.0f, // DIR
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount3++;

	// Estatua
	spotLights3[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-2.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount3++;
	// -------------------------------------------------

	// --------------------- Arreglo 4 -----------------
	//linterna pegada a la cámara siempre VA
	spotLights4[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,  // Pos
		0.0f, -1.0f, 0.0f, // DIR
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount4++;
	// -------------------------------------------------

	
	// ================================================ //	


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
	
	
	
	//Configuration
	GLfloat now = 0.0f;
	glm::vec3 lowerLight;


	
	
	// =================== Variables ================= //
	glm::mat4 model(1.0);
	glm::mat4 identidad(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	identidad = glm::mat4(1.0);
	
	glm::vec3 pos_spotlights;

	// =================== ANIMACION GENERAL ================= //
	glm::vec3 pos_obj;
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	// ==================== ZEPPELIN ================ //
	float heliceGiro = 0.0f;

	float tiempoFuego = 0.0;
	float toffsetfuegou = 0.0f;
	float toffsetfuegov = 0.0f;

	// Variables Zeppelin
	// Movimiento
	float tiempoZeppelin = 0.0f;
	float velocidadZeppelin = 0.25f;

	// Forma de la elipse
	float radioZeppelinX = 100.0f;
	float radioZeppelinZ = 80.0f;

	// Centro
	float centroZeppelinX = 0.0f;
	float centroZeppelinY = 50.0f;
	float centroZeppelinZ = 0.0f;

	// Ángulo inicial (elige dónde quieres que empiece)
	float anguloInicioZeppelin = 90.0f * toRadians;

	// Posición inicial 
	float posZeppelinX = centroZeppelinX + radioZeppelinX * cos(anguloInicioZeppelin);
	float posZeppelinZ = centroZeppelinZ + radioZeppelinZ * sin(anguloInicioZeppelin);

	// Dirección inicial 
	float dirZeppelinX = -radioZeppelinX * sin(anguloInicioZeppelin);
	float dirZeppelinZ = radioZeppelinZ * cos(anguloInicioZeppelin);

	float giroZeppelin = atan2(dirZeppelinZ, -dirZeppelinX);
	
	float anguloActualZeppelin = 0.0f;

	// ==================== CIENTIFICO ================ //
	
	float brazo_rotacion = 0.0f;
	float cientificoAnimacion = 0.0f;

	// ======================================= //

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		
		// ------------------ Cambio de camaras -----------------------
		// camera.keyControl(mainWindow.getsKeys(), deltaTime);
		// camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		// ------------------ Cambio de camaras -----------------------
		camera.controlarCamara(
			mainWindow.getsKeys(),
			deltaTime,
			mainWindow.getXChange(),
			mainWindow.getYChange(),
			mainWindow.getCamaraActual(),
			mainWindow.getVistaEstaticaActual()
		);


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// // ================================================================= SKYBOX  ========================================================================================================= //
		// Dia  -    0  -> 40 seg
		// noche -   41 -> 80 seg
		horarioDiaNoche = fmod(glfwGetTime(), 80.0f);
		if (horarioDiaNoche <= 40.0f) flagDiaNoche = true;
		else flagDiaNoche = false;

		//skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		
		if(flagDiaNoche) skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		else skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		
		// ================================================================= SKYBOX FIN ========================================================================================================= //
		
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Para que no afecte a otras texturas
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		// ================================================================= LUCES INICIO ================================================================
		
		// ------------------------------------------------- MAIN
		if (flagDiaNoche) shaderList[0].SetDirectionalLight(&mainLightDia);
		else shaderList[0].SetDirectionalLight(&mainLightNoche);
				
		// ------------------------------------------------- SPOTLIGHTS ( TECLADO )
		// T -> Tren
		// E -> Estatua
		if (mainWindow.luzTren() && mainWindow.luzEstatua()) {
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights1[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderList[0].SetSpotLights(spotLights1, spotLightCount1);
		}
		else if (mainWindow.luzTren()){
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
		}
		else if (mainWindow.luzEstatua()){
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights3[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
		}
		else{
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights4[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderList[0].SetSpotLights(spotLights4, spotLightCount4);
		}
		
		
		// ------------------------------------------------- POINTLIGHTS - Ciclo Dia y Noche
		if (flagDiaNoche) shaderList[0].SetPointLights(pointLights, 0);
		else shaderList[0].SetPointLights(pointLights, pointLightCount);
		
		// ================================================================= LUCES FIN ================================================================
		
		// ----------------------------------------- PISO -----------------------------------------
		// Piedra
		model = identidad;
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture_piedra.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		
		// Tierra
		model = identidad;
		model = glm::translate(model, glm::vec3(200.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture_tierra.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
				
		// ================================================== Proyecto final  ================================================================= //

		// QUETZALLI ZARATE MENES
		
		// ----------------------------------------- LAMPARA -----------------------------------------
		// Lampara 1
		model = identidad;
		model = glm::translate(model, glm::vec3(80.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		pos_obj = glm::vec3(model[3]);
		pos_obj.y += 37.0f;
		pos_obj.x -= 1.0f;
		pointLights[0].SetPos(pos_obj);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		// Lampara 2
		model = identidad;
		model = glm::translate(model, glm::vec3(-40.0f, -1.0f, 120.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		pos_obj = glm::vec3(model[3]);
		pos_obj.y += 37.0f;
		pos_obj.x -= 1.0f;
		pointLights[1].SetPos(pos_obj);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		// Lampara 3
		model = identidad;
		model = glm::translate(model, glm::vec3(-40.0f, -1.0f, -120.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		pos_obj = glm::vec3(model[3]);
		pos_obj.y += 37.0f;
		pos_obj.x -= 1.0f;
		pointLights[2].SetPos(pos_obj);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		// ----------------------------------------- LETRERO -----------------------------------------
		// Cartel 1 
		model = identidad;
		model = glm::translate(model, glm::vec3(65.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		letrero.RenderModel();

		// Cartel 2 
		model = identidad;
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, -120.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		letrero.RenderModel();

		// Cartel 3
		model = identidad;
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 100.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		letrero.RenderModel();

		// ----------------------------------------- LIBRERO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(220.0, -1.0f, 100.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		librero_medieval.RenderModel();

		// ----------------------------------------- ARBOL -----------------------------------------
		// 1
		model = identidad;
		model = glm::translate(model, glm::vec3(120.0, -1.1f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		// 2
		model = identidad;
		model = glm::translate(model, glm::vec3(120.0, -1.1f, -250.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		// 3
		model = identidad;
		model = glm::translate(model, glm::vec3(275.0, -1.1f, -100.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		// 4
		model = identidad;
		model = glm::translate(model, glm::vec3(120.0, -1.1f, 250.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		// 5
		model = identidad;
		model = glm::translate(model, glm::vec3(275.0, -1.1f, 100.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		// ----------------------------------------- CASTILLO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(200.0, -1.0f, -160.0f));
		model = glm::scale(model, glm::vec3(9.5f, 9.5f, 9.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		castillo.RenderModel();

		// ----------------------------------------- FRANK -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(230.0, -0.9f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		frank.RenderModel();

		// ----------------------------------------- LABORATORIO FRANK -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(200.0, -0.9f, 200.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laboratorio_frank.RenderModel();

		// ----------------------------------------- Científico -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(150.0, 17.0f, 200.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cientifico_torso.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.56f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cientifico_cabeza.RenderModel();
		
		cientificoAnimacion = fmod(glfwGetTime(), 10.0f);
		
		if (cientificoAnimacion >= 0.0f && cientificoAnimacion <= 5.0f) brazo_rotacion += deltaTime * 0.3;
		else brazo_rotacion -= deltaTime * 0.3;

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.02f, 0.52f, 0.145f));
		model = glm::rotate(model, brazo_rotacion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cientifico_brazo_der.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.02f, 0.48f, -0.135f));
		model = glm::rotate(model, brazo_rotacion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cientifico_brazo_izq.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cientifico_piernas.RenderModel();

		// ----------------------------------------- ZEPPELIN -----------------------------------------
		// INICIA ANIMACIÓN CON LA TECLA -> Z
		if (mainWindow.animacionZeppelin()) {
			tiempoZeppelin += 0.01f * deltaTime * velocidadZeppelin;

			anguloActualZeppelin = anguloInicioZeppelin + tiempoZeppelin;

			posZeppelinX = centroZeppelinX + radioZeppelinX * cos(anguloActualZeppelin);
			posZeppelinZ = centroZeppelinZ + radioZeppelinZ * sin(anguloActualZeppelin);

			dirZeppelinX = -radioZeppelinX * sin(anguloActualZeppelin);
			dirZeppelinZ = radioZeppelinZ * cos(anguloActualZeppelin);

			giroZeppelin = atan2(dirZeppelinZ, -dirZeppelinX);

			// ------------- Giro helice ---------------------
			heliceGiro += deltaTime * 3.0f;
			if (heliceGiro >= 360.0f) heliceGiro = 0.0f;

			// ------------- TEXTURA FUEGO -------------------
			tiempoFuego += 0.05 * deltaTime;

			// Movimiento ondulado en U y V
			toffsetfuegou = sin(tiempoFuego * 3.0f) * 0.04f;
			toffsetfuegov = 0.0f;

			if (toffsetfuegov > 1.0f) tiempoFuego = 0.0f;

			toffset = glm::vec2(toffsetfuegou, toffsetfuegov);

			model = modelaux;
			model = glm::translate(model, glm::vec3(7.5f, 0.6f, 0.0f));
			model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
			model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//blending: transparencia o traslucidez
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			FuegoTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[3]->RenderMesh();
			glDisable(GL_BLEND);
		}
		// Para que no afecte a otras texturas
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		// 
		// Zeppelin base
		model = identidad;
		model = glm::translate(model, glm::vec3(posZeppelinX,centroZeppelinY,posZeppelinZ));
		model = glm::rotate(model, giroZeppelin, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zeppelin_base.RenderModel();

		// helice arriba
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, 9.0f, 0.0f));
		model = glm::rotate(model, heliceGiro * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zeppelin_helice_arriba.RenderModel();

		// helice izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.8f, 0.7f, 1.75f));
		model = glm::rotate(model, heliceGiro * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zeppelin_helice_abajo.RenderModel();

		// helice derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.8f, 0.7f, -1.75f));
		model = glm::rotate(model, heliceGiro * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zeppelin_helice_abajo.RenderModel();
						
		// Para que no afecte a otras texturas
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		// ================================================================= LUCES POR TECLADO ================================================================
		// ----------------------------------------- ESTATUA / REFLECTOR -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(-130.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		// ----------------------------- pointlights luces -----------------------------
		if (mainWindow.luzTren() && mainWindow.luzEstatua()) {
			pos_spotlights = glm::vec3(model[3]);
			pos_spotlights.y += 30.0f;
			pos_spotlights.x += 5.0f;
			spotLights1[2].SetColor(glm::vec3(0.0f, 1.0f, 0.0f)); // Verde
			spotLights1[2].SetPos(pos_spotlights);
			spotLights1[2].SetDir(glm::vec3(0.5f, -1.0f, 0.0f));
		}
		else if (mainWindow.luzEstatua()) {
			pos_spotlights = glm::vec3(model[3]);
			pos_spotlights.y += 30.0f;
			pos_spotlights.x += 5.0f;
			spotLights3[1].SetColor(glm::vec3(0.0f, 1.0f, 0.0f)); // Verde
			spotLights3[1].SetPos(pos_spotlights);
			spotLights3[1].SetDir(glm::vec3(0.5f, -1.0f, 0.0f));
		}
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reflector.RenderModel(); 

		// ejemplo objeto alumbrado
		model = identidad;
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estatua.RenderModel(); 

		// ----------------------------------------- TREN -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, 100.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		// ----------------------------- pointlights luces -----------------------------
		if (mainWindow.luzTren() && mainWindow.luzEstatua()) {
			pos_spotlights = glm::vec3(model[3]);
			pos_spotlights.y += 20.0f;
			pos_spotlights.x -= 5.0f;
			spotLights1[1].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); // Cyan
			spotLights1[1].SetPos(pos_spotlights);
			spotLights1[1].SetDir(glm::vec3(0.3f, -1.0f, 0.0f));
		}
		else if (mainWindow.luzTren()) {
			pos_spotlights = glm::vec3(model[3]);
			pos_spotlights.y += 20.0f;
			pos_spotlights.x -= 5.0f;
			spotLights2[1].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); // Cyan
			spotLights2[1].SetPos(pos_spotlights);
			spotLights2[1].SetDir(glm::vec3(0.3f, -1.0f, 0.0f));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		letrero.RenderModel(); // cambiar por modelo de tren
		// ================================================================= LUCES POR TECLADO ================================================================




		// ==================================================================================================================================== //

		// EJEMPLO TRANSPARENCIA
		//model = identidad;
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////blending: transparencia o traslucidez
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Example.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		//glDisable(GL_BLEND);

		
		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
