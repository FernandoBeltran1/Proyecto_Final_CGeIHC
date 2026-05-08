/*
Proyecto Final
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

//======shaders fernando=====//
Shader shaderHojas;

Camera camera;


// ================== TEXTURAS ======================= //

Texture pisoTexture;
Texture grassTexture;
Texture aguaTexture;


// ================== MODELOS ======================= //
//modelos quesadilla
Model lampara;
Model letrero;
Model castillo;
Model arbol;
Model librero_medieval;


//modelos fernando
Model santuario_narukami;
Model lamp1;
Model lamp2;
Model lamp3;
Model lamp_sup;
Model lamp_base;
Model hojas;
Model arbusto;
Model arbusto2;
Model tp;
Model base_tp;
Model puerta;
Model estatua;
Model base_estatua;
Model coche;
Model llanta;

Model cabeza;
Model hombro_izq;
Model hombro_der;
Model mano_izq;
Model mano_der;
Model torso;
Model muslo_izq;
Model muslo_der;
Model tibia_izq;
Model tibia_der;
Model pie_izq;
Model pie_der;

Model muñeca;
Model cuerpo_muñeca;


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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	//calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	//calcAverageNormals(aguaIndices, 6, aguaVertices, 64, 8, 5);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	//======nuevo shader fernando=====///
	shaderHojas.CreateFromFiles("shader_hojas.vert",
		"shader_hojas.frag");
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



//funcion para la animacion del coche de fernando

// Centro, radios y cantidad de puntos ajustables aquí
const glm::vec3 elipseCentro = glm::vec3(-18.0f, 0.0f, 25.0f);
const float     elipseRadioX = 14.0f;   // ancho
const float     elipseRadioZ = 18.0f;   // largo
const int       elipsePuntos = 32;      // más puntos = más suave

static std::vector<glm::vec3> GenerarElipse()
{
	std::vector<glm::vec3> puntos;
	for (int i = 0; i <= elipsePuntos; i++)  // <= para cerrar el loop
	{
		float angulo = (2.0f * 3.14159265f * i) / elipsePuntos;
		float x = elipseCentro.x + elipseRadioX * cosf(angulo);
		float z = elipseCentro.z + elipseRadioZ * sinf(angulo);
		puntos.push_back(glm::vec3(x, elipseCentro.y, z));
	}
	return puntos;
}

static const std::vector<glm::vec3> trackWaypoints = GenerarElipse();
glm::vec3 CatmullRom(const glm::vec3& p0, const glm::vec3& p1,
	const glm::vec3& p2, const glm::vec3& p3, float t)
{
	float t2 = t * t, t3 = t2 * t;
	return 0.5f * (
		(2.0f * p1) +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
		);
}

static glm::vec3 SamplePath(float t)
{
	int n = (int)trackWaypoints.size();
	float s = t * (n - 1);
	int i1 = glm::clamp((int)s, 0, n - 1);
	int i0 = glm::clamp(i1 - 1, 0, n - 1);
	int i2 = glm::clamp(i1 + 1, 0, n - 1);
	int i3 = glm::clamp(i1 + 2, 0, n - 1);
	float lt = s - (float)i1;
	return CatmullRom(trackWaypoints[i0], trackWaypoints[i1],
		trackWaypoints[i2], trackWaypoints[i3], lt);
}

void GetPathPosAndTangent(float t, glm::vec3& outPos, glm::vec3& outTangent)
{
	outPos = SamplePath(t);
	const float eps = 0.001f;
	glm::vec3 posA = SamplePath(glm::clamp(t - eps, 0.0f, 1.0f));
	glm::vec3 posB = SamplePath(glm::clamp(t + eps, 0.0f, 1.0f));
	glm::vec3 diff = posB - posA;
	if (glm::length(diff) < 1e-6f)
		outTangent = glm::vec3(0.0f, 0.0f, 1.0f);
	else
		outTangent = glm::normalize(diff);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	// ================== TEXTURAS ======================= //
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	aguaTexture = Texture("Textures/agua.png");
	aguaTexture.LoadTextureA();

	//=========Textura para el piso con pasto fernando===========//
	grassTexture = Texture("Textures/Grass.png");
	grassTexture.LoadTextureA();

	
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

	// =================modelos fernando======================== //
	santuario_narukami = Model();
	santuario_narukami.LoadModel("Models/santuario_narukami.obj");

	lamp1 = Model();
	lamp1.LoadModel("Models/Narukami_Lamp_1.obj");

	lamp2 = Model();
	lamp2.LoadModel("Models/Narukami_Lampara_dos.obj");

	lamp3 = Model();
	lamp3.LoadModel("Models/narukami_lampara_3.obj");

	lamp_sup = Model();
	lamp_sup.LoadModel("Models/lampara_supp.obj");

	lamp_base = Model();
	lamp_base.LoadModel("Models/base_lampara.obj");

	hojas = Model();
	hojas.LoadModel("Models/hojas_sakura.obj");

	arbusto = Model();
	arbusto.LoadModel("Models/arbustos_narukami.obj");

	arbusto2 = Model();
	arbusto2.LoadModel("Models/arbustos_narukami2.obj");

	tp = Model();
	tp.LoadModel("Models/tp.obj");

	base_tp = Model();
	base_tp.LoadModel("Models/tp_base.obj");

	puerta = Model();
	puerta.LoadModel("Models/puerta_torii_narukami.obj");

	estatua = Model();
	estatua.LoadModel("Models/estatua_zorro_narukami.obj");

	base_estatua = Model();
	base_estatua.LoadModel("Models/base_zorro_narukami.obj");

	coche = Model();
	coche.LoadModel("Models/coche.obj");

	llanta = Model();
	llanta.LoadModel("Models/llantas_coche.obj");

	cabeza = Model();
	cabeza.LoadModel("Models/yae_head.obj");

	mano_izq = Model();
	mano_izq.LoadModel("Models/mano_izq_yae.obj");

	mano_der = Model();
	mano_der.LoadModel("Models/mano_der_yae.obj");

	torso = Model();
	torso.LoadModel("Models/torso_yae.obj");

	hombro_izq = Model();
    hombro_izq.LoadModel("Models/hombro_izq_yae.obj");

	hombro_der = Model();
	hombro_der.LoadModel("Models/hombro_der_yae.obj");

	muslo_izq = Model();
	muslo_izq.LoadModel("Models/muslo_izq_yae.obj");

	muslo_der = Model();
	muslo_der.LoadModel("Models/muslo_der_yae.obj");

	tibia_izq = Model();
	tibia_izq.LoadModel("Models/tibia_izq_yae.obj");

	tibia_der = Model();
	tibia_der.LoadModel("Models/tibia_der_yae.obj");

	pie_izq = Model();
	pie_izq.LoadModel("Models/pie_izq_yae.obj");

	pie_der = Model();
	pie_der.LoadModel("Models/pie_der_yae.obj");

	muñeca = Model();
	muñeca.LoadModel("Models/mascara_kitsune.obj");

	cuerpo_muñeca = Model();
	cuerpo_muñeca.LoadModel("Models/muneca_cuerpo.obj");

	// ================termina modelos fernando========================= //

	// ========================================= //


	std::vector<std::string> skyboxFaces;
	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");*/

	skyboxFaces.push_back("Textures/Skybox/pos_rt.jpg");
	skyboxFaces.push_back("Textures/Skybox/neg_lf.jpg");
	skyboxFaces.push_back("Textures/Skybox/neg_dn.jpg");
	skyboxFaces.push_back("Textures/Skybox/pos_up.jpg");
	skyboxFaces.push_back("Textures/Skybox/pos_bk.jpg");
	skyboxFaces.push_back("Textures/Skybox/neg_ft.jpg");


	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ================== LUCES ======================= //
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	
	// ============== POINTLIGHTS ==================== //
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	// ---------------------------------------- Lampara blanca
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,  //Lampara color blanco
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// ===============pointlights fernando========================== //

	// Lámparas del santuario 
	for (int i = 1; i <= 28; i++)
	{
		pointLights[i] = PointLight(1.0f, 0.85f, 0.6f, 
			0.0f, 1.5f,
			0.0f, 0.0f, 0.0f,
			0.5f, 0.3f, 0.1f);
		pointLightCount++;
	}

	// Lámparas lamp3
	pointLights[29] = PointLight(1.0f, 0.85f, 0.6f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		0.5f, 0.3f, 0.1f);
	pointLightCount++;

	pointLights[30] = PointLight(1.0f, 0.85f, 0.6f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		0.5f, 0.3f, 0.1f);
	pointLightCount++;

	//lampara separada
	for (int i = 31; i <= 34; i++)
	{
		pointLights[i] = PointLight(1.0f, 0.85f, 0.6f,
			0.0f, 2.5f,
			0.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.3f);
		pointLightCount++;
	}

	// Lámparas lamp1 
	for (int i = 35; i <= 46; i++)
	{
		pointLights[i] = PointLight(1.0f, 0.85f, 0.6f,
			0.0f, 1.5f,
			0.0f, 0.0f, 0.0f,
			0.5f, 0.3f, 0.1f);
		pointLightCount++;
	}

	// PointLight tp
	pointLights[47] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.4f, 0.3f, 0.1f);
	pointLightCount++;

	// =================terminan pointlights fernando======================== //

	// ============== SPOTLIGHTS ===================== //
	
	// Contador de spotlights
	unsigned int spotLightCount = 0;
	//linterna pegada a la cámara siempre VA
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
	
	// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
	
	
	
	//Configuration
	GLfloat now = 0.0f;
	GLfloat bobOffset = 0.0f;
	glm::vec3 lowerLight;
	
	
	// =================== Variables ================= //
	glm::mat4 model(1.0);
	glm::mat4 identidad(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	//===============inician variables fernando===========//
	glm::mat4 modelBase(1.0);
	glm::mat4 modelPasto(1.0);
	glm::vec3 santuarioPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightWorldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 baseTpLocalPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 modelBaseTp = glm::mat4(1.0);
	glm::mat4 modelTp = glm::mat4(1.0);
	glm::mat4 modelBaseEstatua = glm::mat4(1.0);
	glm::vec3 tpWorldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 baseEstatuaLocalPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 estatuaLocalPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 modelEstatua = glm::mat4(1.0);
	
	glm::mat4 modelSantuario = identidad;
	glm::mat4 modelCoche = identidad;

	identidad = glm::mat4(1.0);

	int lamp2Count = 0.0f;
	int lamp3Count = 0.0f;
	int lamp_base_count = 0.0f;
	int lamp1_count = 0.0f;


	// =================== terminan variables fernnado ================= //

	// =================== ANIMACION ================= //
	glm::vec3 pos_obj;

	// ===============animacion  tp fernando========================== //
	GLfloat tp_rotY = 0.0f;   
	GLfloat tp_bobTime = 0.0f;
	//=================animacion fomu fernando=======================//
	
	GLfloat muneca_rotTime = 0.0f;  

	//===============animacion yae fernando====================//

	struct WaypointYae {
		glm::vec3 pos;
		float speed;
	};

	std::vector<WaypointYae> rutaYae = {

			// === CORREDOR — loop por los centros ===
			{ glm::vec3(-1.275f,  5.854f, -25.645f), 0.3f },  // centro par 9&10
			{ glm::vec3(-1.3635f, 5.848f, -31.405f), 0.3f },  // centro par 7&8

			// === HACIA ESCALERAS por los centros ===
			{ glm::vec3(-6.921f,  5.054f, -33.28f),  0.2f },  // centro par 5&6
			{ glm::vec3(-10.133f, 4.165f, -32.725f), 0.2f },  // centro par 3&4
			{ glm::vec3(-13.846f, 2.725f, -31.44f),  0.2f },  // centro par 1&2

			// === REGRESO ===
			{ glm::vec3(-10.133f, 4.165f, -32.725f), 0.2f },
			{ glm::vec3(-6.921f,  5.054f, -33.28f),  0.2f },
			{ glm::vec3(-1.3635f, 5.848f, -31.405f), 0.3f },
		};

			int       yae_waypoint = 0;
			float     yae_segT = 0.0f;
			float     yae_yaw = 0.0f;
			float     yae_pitch = 0.0f;
			glm::vec3 yae_worldPos = rutaYae[0].pos;
			float     yae_walkTime = 0.0f;
			float     yae_walkCycle = 0.0f;

			const float YAE_ARM_SWING = 35.0f;
			const float YAE_LEG_SWING = 30.0f;
			const float YAE_KNEE_BEND = 18.0f;
			const float YAE_BOB_Y = 0.015f;
			const float YAE_WALK_SPEED_F = 4.0f;

			
			// variables del coche
			float  tCoche = 0.0f;
			const float velTCoche = 0.0006f;   // ajusta velocidad aquí
			bool   cochePausado = false;
			bool   teclaRAnterior = false;
			float  rotllanta = 0.0f;
			float  yawCoche = 0.0f;
			float  pitchCoche = 0.0f;
			glm::vec3 carPos(0.0f), carTangent(0.0f, 0.0f, 1.0f);
			glm::vec3 carRight(1.0f, 0.0f, 0.0f);
			glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
		GLfloat rawDelta = now - lastTime; 
		deltaTime = rawDelta + rawDelta / limitFPS;
		lastTime = now;

		//para el tp
		tp_rotY += 0.1f * deltaTime;   
		if (tp_rotY >= 360.0f) tp_rotY -= 360.0f;
		tp_bobTime += deltaTime;

		//para la muñeca fomu
		muneca_rotTime += deltaTime;

		//para la yae
	
		{
			int nextWP = (yae_waypoint + 1) % (int)rutaYae.size();
			glm::vec3 from = rutaYae[yae_waypoint].pos;
			glm::vec3 to = rutaYae[nextWP].pos;
			float     spd = rutaYae[yae_waypoint].speed;

			float segLen = glm::length(to - from);
			if (segLen > 0.001f)
			{
				yae_segT += (spd / segLen) * rawDelta;

				if (yae_segT >= 1.0f)
				{
					yae_segT -= 1.0f;
					yae_waypoint = nextWP;
					nextWP = (yae_waypoint + 1) % (int)rutaYae.size();
					from = rutaYae[yae_waypoint].pos;
					to = rutaYae[nextWP].pos;
					segLen = glm::length(to - from);
				}

				yae_worldPos = glm::mix(from, to, yae_segT);

				glm::vec3 dir = to - from;
				if (glm::length(glm::vec2(dir.x, dir.z)) > 0.001f)
					yae_yaw = glm::degrees(atan2f(dir.x, dir.z));

				float hDist = glm::length(glm::vec2(dir.x, dir.z));
				if (hDist > 0.001f)
					yae_pitch = glm::degrees(atan2f(dir.y, hDist));
			}
		}
		yae_walkCycle = fmodf((float)glfwGetTime() * YAE_WALK_SPEED_F, 2.0f * 3.14159f);


		//animacion para el coche de fernando
	
		bool* keys = mainWindow.getsKeys();
		bool teclaRActual = keys[GLFW_KEY_R];
		if (teclaRActual && !teclaRAnterior) {
			tCoche = 0.0f;
			rotllanta = 0.0f;
		}
		teclaRAnterior = teclaRActual;

		tCoche += velTCoche * deltaTime;
		if (tCoche >= 1.0f)
			tCoche = 0.0f;   

		rotllanta += 360.0f * deltaTime;
		if (rotllanta > 360.0f) rotllanta -= 360.0f;

		GetPathPosAndTangent(tCoche, carPos, carTangent);
		yawCoche = atan2f(carTangent.x, carTangent.z) * (180.0f / 3.14159265f);
		pitchCoche = asinf(glm::clamp(carTangent.y, -1.0f, 1.0f)) * (180.0f / 3.14159265f);
		carRight = glm::normalize(glm::cross(worldUp, carTangent));
		if (glm::length(carRight) < 0.001f)
			carRight = glm::vec3(1.0f, 0.0f, 0.0f);

		//Recibir eventos del usuario
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
		
		//información en el shader de intensidad especular y brillo
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

		//---------------------------------------PASTO-------------------------------------

		model = identidad;
		model = glm::translate(model, glm::vec3(-180.0f, -0.9f, -200.0f));
		modelPasto = model;
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		grassTexture.UseTexture();
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

		//-----------------------------------------Objetos de fernando-----------------------------
		// ----------------------------------------- COCHE -----------------------------------------
		model = identidad;
		model = glm::translate(model, carPos);
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, yawCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -pitchCoche * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelCoche = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche.RenderModel();

		// ----------------------------------------- LLANTAS COCHE -----------------------------------------
		model = modelCoche;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.5f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta.RenderModel();

		model = modelCoche;
		model = glm::translate(model, glm::vec3(0.0, 1.0f, 4.5f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta.RenderModel();

		//---------------------------terminan objetos fernando----------------------------------//

		// ==================================================================================================================================== //
		//
		model = identidad;
		model = glm::translate(model, glm::vec3(150.0, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		castillo.RenderModel();
		
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
		

	    //blending: transparencia o traslucidez
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 0.6f, 9.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		

		// ============renderizados fernando============================= //
		//agua
		
		aguaTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
	
		//SANTURAIO
		//NO rotar el santuario ni el pasto porque se rompen los pointlights
	
		model = modelPasto;
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 30.0f));
		//model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelSantuario = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	    santuario_narukami.RenderModel();
		
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		glm::vec3 lamp2_positions[] = {

		        //escaleras
				{  -14.836f,  1.703f, -33.37f  },  // 1
				{   -12.856f,  1.747f, -29.51f  },  // 2
				{   -10.783f,  3.27f,  -35.13f  },  // 3
				{   -9.483f,  3.06f,  -30.32f  },  // 4
				{   -6.871f,    3.973f, -30.85f  },  // 5
				{   -6.971f,  4.134f, -35.71f  },  // 6

				//entrada por el tp
				{   1.1f,    4.848f, -31.4f   },  // 6
				{  -3.827f,  4.848f, -31.41f  },  // 8
				{   1.182f,  4.86f,  -25.66f  },  // 9
				{  -3.732f,  4.848f, -25.63f  },  // 10

				//costado por la casa de entrada
				{   5.656f,  7.681f,  -2.562f },  // 11
				{   5.621f,  7.681f,   2.167f },  // 12
				{  -8.394f,  7.681f,  -2.545f },  // 13
				{  -8.362f,  7.681f,   2.268f },  // 14

				//costados de las otras dos casas
				{ -15.244f,  8.397f,  11.3f   },  // 15
				{ -21.774f,  8.397f,  14.56f  },  // 16
				{ -21.774f,  8.397f,  17.85f  },  // 17
				{ -15.324f,  8.397f,  21.04f  },  // 18
				{  12.546f,  8.397f,  11.38f  },  // 19
				{  18.976f,  8.397f,  14.54f  },  // 20
				{  18.976f,  8.397f,  17.97f  },  // 21
				{  12.546f,  8.397f,  21.02f  },  // 22

				//centro del patio interior
				{  -3.894f,  7.185f,  13.73f  },  // 23
				{   1.145f,  7.185f,  13.71f  },  // 24
				{   1.1f,    7.185f,  18.82f  },  // 25
				{  -3.839f,  7.185f,  18.82f  },   // 26

				//cerca del arbol principal
				{ -5.864f,  9.999f, 27.84f },  // 27 
				{   3.123f, 9.999f, 27.85f  }  //28
		};
		lamp2Count = sizeof(lamp2_positions) / sizeof(lamp2_positions[0]);

		for (int i = 0; i < lamp2Count; i++)
		{
			// Jerarquía: se parte de la matriz del santuario (padre)
			model = modelSantuario;
			model = glm::translate(model, lamp2_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp2.RenderModel();

			santuarioPos = glm::vec3(modelSantuario[3]);
			lightWorldPos = santuarioPos + lamp2_positions[i] + glm::vec3(0.0f, 0.5f, 0.0f);
			pointLights[i + 1].SetPos(lightWorldPos);
		}

		shaderList[0].SetPointLights(pointLights, pointLightCount);

		glDisable(GL_CULL_FACE);
		//lamp3
		glm::vec3 lamp3_positions[] = {
			{ -16.01f,  5.433f, -0.88f },  // izquierda
			{ -15.94f,  5.179f, -4.07f }   // derecha
		};
		lamp3Count = sizeof(lamp3_positions) / sizeof(lamp3_positions[0]);;

		for (int i = 0; i < lamp3Count; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, lamp3_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp3.RenderModel();

			santuarioPos = glm::vec3(modelSantuario[3]);
			lightWorldPos = santuarioPos + lamp3_positions[i] + glm::vec3(0.0f, 1.7f, 0.0f);
			pointLights[29 + i].SetPos(lightWorldPos);
		}

		//lamparas separadas
		glm::vec3 lamp_base_positions[] = {
			     {  1.102f,  4.102f, -19.98f },  // 1
				 { -3.766f,  4.134f, -19.98f },  // 2
				 {  0.900f,  7.172f, 20.001f},   //3 
				 {   -3.515,  7.076f,  20.001f},  // 4
				
		};
		lamp_base_count = sizeof(lamp_base_positions) / sizeof(lamp_base_positions[0]);

		for (int i = 0; i < lamp_base_count; i++)
		{
			// Base jerarquizada con santuario
			model = modelSantuario;
			model = glm::translate(model, lamp_base_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp_base.RenderModel();

			// Sup jerarquizada con base
			glm::mat4 modelBase = model;
			model = modelBase;
			model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp_sup.RenderModel();

			// Pointlight jerarquizado con lamp_sup 
			glm::vec3 lampSupWorldPos = glm::vec3(model[3]);
			glm::vec3 lightWorldPos = lampSupWorldPos + glm::vec3(0.0f, 0.5f, 0.0f);
			pointLights[31 + i].SetPos(lightWorldPos);
		}

		//lamp1
		glm::vec3 lamp1_positions[] = {
				{ 3.542f,  7.120f, -29.99f },  // 1
				{ 3.539f,  7.126f, -26.71f },  // 2
				{ 1.645f,  9.102f, -12.44f },  // 3 
				{ -4.233f, 9.134f, -12.44f  },   //4
				{ 1.223f,  10.502f, -5.02f },  //5 
				{ -4.223f, 10.534f, -5.02f  },   //6
				{ 1.623f,  10.402f, 7.70f },  //7
				{ -4.300f,  10.402f, 7.70f },  //8
				{  -8.894f, 10.2f,  14.855f  },  // 9
				{  -8.839f,  10.2f,  17.7f  },   // 10
				{   6.145f,  10.2f,  14.855f  },  // 11
				{   6.1f,    10.2f,  17.7f  },  // 12					   
		};
		lamp1_count = sizeof(lamp1_positions) / sizeof(lamp1_positions[0]);

		for (int i = 0; i < lamp1_count; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, lamp1_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp1.RenderModel();

			glm::vec3 lampWorldPos = glm::vec3(model[3]);
			glm::vec3 lightWorldPos = lampWorldPos + glm::vec3(0.0f, 0.5f, 0.0f);
			pointLights[35 + i].SetPos(lightWorldPos);
		}

		glEnable(GL_CULL_FACE);

		//base tp
		
		baseTpLocalPos = glm::vec3(-6.427f, 5.231f, -27.4f);
		modelBaseTp = modelSantuario;
		modelBaseTp = glm::translate(modelBaseTp, baseTpLocalPos);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseTp));
		base_tp.RenderModel();

	    //tp
		bobOffset = 0.3f * sinf(tp_bobTime * 0.01f);   

		modelTp = modelBaseTp;
		model = modelTp;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f + bobOffset, 0.0f));  
		model = glm::rotate(model, tp_rotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tp.RenderModel();

		tpWorldPos = glm::vec3(model[3]);
		pointLights[47].SetPos(tpWorldPos + glm::vec3(0.0f, 1.5f, 0.0f));
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		//estatua zorrito

		
		glm::vec3 baseEstatua_positions[] = {
			{ -4.115f,  6.676f,  25.001f },  // 1
			{  1.500f,  6.672f,  25.001f }   // 2
		};

		glm::vec3 estatua_positions[] = {
			{ -0.081f, 5.476f,  0.181f },  // 
			{ 0.181f, 5.492f,  0.181f }   //
		};

		for (int i = 0; i < 2; i++)
		{
			glm::mat4 modelBaseEstatua = modelSantuario;
			modelBaseEstatua = glm::translate(modelBaseEstatua, baseEstatua_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseEstatua));
			base_estatua.RenderModel();

			glm::mat4 modelEstatua = modelBaseEstatua;
			modelEstatua = glm::translate(modelEstatua, estatua_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstatua));
			estatua.RenderModel();
		}
		
		// puertas torii
		glm::vec3 puerta_positions[] = {
				{ -12.846f,  1.725f, -31.64f  },  // puerta 1
				{ -9.133f,  3.165f, -32.725f },  // puerta 2
				{  -5.921f,  4.054f, -33.28f  }   // puerta 3
		};

		float puerta_rotations[] = {
			25.0f,   // puerta 1
			10.0f,   // puerta 2
			0.0f     // puerta 3
		};

		for (int i = 0; i < 3; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, puerta_positions[i]);
			model = glm::rotate(model, puerta_rotations[i] * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			puerta.RenderModel();
		}

		// fomureimu

	   // ----------------------------------------- MUÑECA -----------------------------------------

       // CUERPO MUÑECA — jerarquizado con santuario
		glm::mat4 modelCuerpoMuneca = modelSantuario;
		modelCuerpoMuneca = glm::translate(modelCuerpoMuneca, glm::vec3(-1.372f, 7.185f, 16.27f));
		modelCuerpoMuneca = glm::scale(modelCuerpoMuneca, glm::vec3(0.1f, 0.1f, 0.1f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCuerpoMuneca));
		cuerpo_muñeca.RenderModel();

		/// MÁSCARA KITSUNE — jerarquizada con cuerpo muñeca, rotación seno
		glm::mat4 modelMuneca = modelCuerpoMuneca;
		modelMuneca = glm::translate(modelMuneca, glm::vec3(0.0f, 2.0f, 0.0f));
		GLfloat muneca_rotAngle = 45.0f * sinf(muneca_rotTime * 0.05f);
		modelMuneca = glm::rotate(modelMuneca, muneca_rotAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMuneca));
		muñeca.RenderModel();

		// -----------------------------------------yae miko -----------------------------------------
		
		float tiempoActual = fmodf((float)glfwGetTime(), 6.2831f);
		float pitchCorregido = (yae_pitch > 0.0f) ? -(yae_pitch * 0.5f) : -(yae_pitch * 0.8f);

		float legSwingR = sinf(tiempoActual * 2.0f) * 45.0f;
		float legSwingL = -sinf(tiempoActual * 2.0f) * 45.0f;
		float kneeBendR = 15.0f;
		float kneeBendL = 15.0f;
		float armSwingR = -sinf(tiempoActual * 2.0f) * 45.0f;
		float armSwingL = sinf(tiempoActual * 2.0f) * 45.0f;
		float bobY = sinf(tiempoActual * 4.0f) * 0.015f;

		// BASE del personaje
		glm::mat4 modelCharBase = modelSantuario;
		modelCharBase = glm::translate(modelCharBase, yae_worldPos + glm::vec3(0.0f, bobY + 1.0f, 0.0f));
		modelCharBase = glm::rotate(modelCharBase, (yae_yaw + 180.0f) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelCharBase = glm::rotate(modelCharBase, pitchCorregido * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		// TORSO
		glm::mat4 modelTorso = modelCharBase;
		modelTorso = glm::scale(modelTorso, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorso));
		torso.RenderModel();

		// CABEZA
		glm::mat4 modelCabeza = modelTorso;
		modelCabeza = glm::translate(modelCabeza, glm::vec3(0.0f, 0.118f, 0.010f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabeza));
		cabeza.RenderModel();

		// HOMBRO DER
		glm::mat4 modelHombroDer = modelTorso;
		modelHombroDer = glm::translate(modelHombroDer, glm::vec3(0.034f, 0.090f, 0.020f));
		modelHombroDer = glm::rotate(modelHombroDer, armSwingR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHombroDer));
		hombro_der.RenderModel();

		// MANO DER
		glm::mat4 modelManoDer = modelHombroDer;
		modelManoDer = glm::translate(modelManoDer, glm::vec3(0.048f, -0.040f, 0.001f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelManoDer));
		mano_der.RenderModel();

		// HOMBRO IZQ
		glm::mat4 modelHombroIzq = modelTorso;
		modelHombroIzq = glm::translate(modelHombroIzq, glm::vec3(-0.033f, 0.092f, 0.019f));
		modelHombroIzq = glm::rotate(modelHombroIzq, armSwingL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHombroIzq));
		hombro_izq.RenderModel();

		// MANO IZQ
		glm::mat4 modelManoIzq = modelHombroIzq;
		modelManoIzq = glm::translate(modelManoIzq, glm::vec3(-0.059f, -0.045f, 0.002f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelManoIzq));
		mano_izq.RenderModel();

		// MUSLO DER
		glm::mat4 modelMusloDer = modelTorso;
		modelMusloDer = glm::translate(modelMusloDer, glm::vec3(0.025f, -0.073f, 0.002f));
		modelMusloDer = glm::rotate(modelMusloDer, legSwingR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMusloDer));
		muslo_der.RenderModel();

		// TIBIA DER
		glm::mat4 modelTibiaDer = modelMusloDer;
		modelTibiaDer = glm::translate(modelTibiaDer, glm::vec3(-0.007f, -0.119f, 0.005f));
		modelTibiaDer = glm::rotate(modelTibiaDer, -kneeBendR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTibiaDer));
		tibia_der.RenderModel();

		// PIE DER
		glm::mat4 modelPieDer = modelTibiaDer;
		modelPieDer = glm::translate(modelPieDer, glm::vec3(-0.005f, -0.145f, 0.018f));
		modelPieDer = glm::rotate(modelPieDer, (kneeBendR * 0.5f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieDer));
		pie_der.RenderModel();

		// MUSLO IZQ
		glm::mat4 modelMusloIzq = modelTorso;
		modelMusloIzq = glm::translate(modelMusloIzq, glm::vec3(-0.027f, -0.076f, 0.001f));
		modelMusloIzq = glm::rotate(modelMusloIzq, legSwingL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMusloIzq));
		muslo_izq.RenderModel();

		// TIBIA IZQ
		glm::mat4 modelTibiaIzq = modelMusloIzq;
		modelTibiaIzq = glm::translate(modelTibiaIzq, glm::vec3(0.007f, -0.117f, 0.008f));
		modelTibiaIzq = glm::rotate(modelTibiaIzq, -kneeBendL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTibiaIzq));
		tibia_izq.RenderModel();

		// PIE IZQ
		glm::mat4 modelPieIzq = modelTibiaIzq;
		modelPieIzq = glm::translate(modelPieIzq, glm::vec3(0.008f, -0.143f, 0.015f));
		modelPieIzq = glm::rotate(modelPieIzq, (kneeBendL * 0.5f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieIzq));
		pie_izq.RenderModel();
		
		//Los shaders  shader_hojas.frag y shader_hojas.vert no estan en la carpeta de shaders porque
		// por alguna razon si los teo ahi se rompe asi esos se quedan ahi
		// ====================================== SHADER HOJAS===================================//
		shaderHojas.UseShader();
		uniformModel = shaderHojas.GetModelLocation();
		uniformProjection = shaderHojas.GetProjectionLocation();
		uniformView = shaderHojas.GetViewLocation();
		uniformEyePosition = shaderHojas.GetEyePositionLocation();
		uniformSpecularIntensity = shaderHojas.GetSpecularIntensityLocation();
		uniformShininess = shaderHojas.GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition,
			camera.getCameraPosition().x,
			camera.getCameraPosition().y,
			camera.getCameraPosition().z);

		shaderHojas.SetDirectionalLight(&mainLight);
		shaderHojas.SetPointLights(pointLights, pointLightCount);
		shaderHojas.SetSpotLights(spotLights, spotLightCount);

		GLuint uniformTime = shaderHojas.GetTimeLocation();
		glUniform1f(uniformTime, (GLfloat)glfwGetTime());

		model = modelSantuario;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		hojas.RenderModel();
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		// Render de arbusto2 
		model = modelSantuario;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		arbusto2.RenderModel();
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		//arbusto
		model = modelSantuario;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		arbusto.RenderModel();
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		
		// =============terminan renderizados fernando============================ //

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
