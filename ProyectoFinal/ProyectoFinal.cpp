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

//======shaders fernando=====//
Shader shaderHojas;
//======shaders fernando=====//


Camera camera;

// ================== TEXTURAS FERNANDO ======================= //

//Texture pisoTexture;
Texture grassTexture;
Texture aguaTexture;


// ================== TEXTURAS QUETZALLI ======================= //

Texture pisoTexture_piedra;
Texture pisoTexture_tierra;

Texture FuegoTexture;

// ================== TEXTURAS QUETZALLI ======================= //


// ================== MODELOS QUETZALLI ======================= //

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
Model estatua_dios_polo;
// ================== MODELOS QUETZALLI ======================= //

//-----------INICIA DECLARACIÓN MODELOS FERNANDO-----------------//
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

//-----------TERMINA DECLARACIÓN MODELOS FERNANDO-----------------//

// ----------- MODELOS ANGEL -------------------

// ---------------------------------------------
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

// Quetza
GLfloat deltaTime1 = 0.0f;
GLfloat lastTime1 = 0.0f;
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

//-----------INICIO FUNCION ANIMACION COCHE FERNANDO--------------------------//
//funcion para la animacion del coche de fernando


const glm::vec3 elipseCentro = glm::vec3(-18.0f, 0.0f, 25.0f);
const float     elipseRadioX = 14.0f;
const float     elipseRadioZ = 18.0f;
const int       elipsePuntos = 32;
static std::vector<glm::vec3> GenerarElipse()
{
	std::vector<glm::vec3> puntos;
	for (int i = 0; i <= elipsePuntos; i++)
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

//------------FIN FUNCION ANIMACION COCHE FERNANDO--------------------------//




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

	//=========TEXTURA PASTO FERNANDO===========//

	aguaTexture = Texture("Textures/agua.png");
	aguaTexture.LoadTextureA();

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

	estatua_dios_polo = Model();
	estatua_dios_polo.LoadModel("Models/estatua.obj");
	// ========================================= //

	// =================INICIA LLAMADO A MODELOS FERNANDO======================== //
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

	// =================TERMINA LLAMADO A MODELOS FERNANDO======================== //

	// ============================================= MODELOS ANGEL =============================================
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

	//Arbol cargado
	Tronco_arbol.LoadModel("Models/Tronco_arbol.obj");
	Hojas_arbol.LoadModel("Models/Hojas_arbol.obj");

	//Gummygo
	Gummy_Body.LoadModel("Models/Gummygo_Body.obj");
	Gummy_RightArm.LoadModel("Models/Gummygo_RightArm.obj");
	Gummy_LeftArm.LoadModel("Models/Gummygo_LeftArm.obj");
	Gummy_Tail.LoadModel("Models/Gummygo_tail.obj");
	Gummy_Hat.LoadModel("Models/Hat.obj");


	// ============================================= MODELOS ANGEL =============================================

	
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
	/// ----------------- Quetza ----------------
	pointLights[0] = PointLight(0.992f, 0.925f, 0.686f,  //Lampara color blanco
		0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.025f, 0.003f);
	pointLightCount++;

	/// ----------------- Quetza ----------------

	//pointLights[3] = PointLight(1.0f, 0.698f, 0.0f,  //Lampara china
	//	0.0f, 5.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.3f, 0.2f, 0.1f);
	//pointLightCount++;

		// ===============INICIA POINTLIGHTS FERNANDO========================== //

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

	// ===================TERMINAN POINTLIGHTS FERNANDO========================== //


	/// ----------------- Quetza ----------------
	pointLights[48] = PointLight(0.992f, 0.925f, 0.686f,  //Lampara color blanco
		0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.025f, 0.003f);
	pointLightCount++;

	pointLights[49] = PointLight(0.992f, 0.925f, 0.686f,  //Lampara color blanco
		0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.025f, 0.003f);
	pointLightCount++;
	/// ----------------- Quetza ----------------


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
	GLfloat bobOffset = 0.0f;
	glm::vec3 lowerLight;


	
	
	// =================== Variables ================= //
	glm::mat4 model(1.0);
	glm::mat4 identidad(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	identidad = glm::mat4(1.0);
	
	glm::vec3 pos_spotlights;

	// ========================== VARIABLES ANGEL ==========================
	float separacion = 30.0f;
	GLfloat bangboo_flotacion = 0.0f;

	// ?? Animación procedural de Manato
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



	//===============INICIAN VARIABLES FERNANDO===========//
	glm::mat4 modelBase(1.0);
	glm::mat4 modelPasto(1.0);
	glm::mat4 modelBaseTp = glm::mat4(1.0);
	glm::mat4 modelTp = glm::mat4(1.0);
	glm::mat4 modelBaseEstatua = glm::mat4(1.0);
	glm::mat4 modelCuerpoMuneca = glm::mat4(1.0);
	glm::mat4 modelMuneca = glm::mat4(1.0);
	glm::mat4 modelCharBase = glm::mat4(1.0);
	glm::mat4 modelTorso = glm::mat4(1.0);
	glm::mat4 modelCabeza = glm::mat4(1.0);
	glm::mat4 modelHombroDer = glm::mat4(1.0);
	glm::mat4 modelManoDer = glm::mat4(1.0);
	glm::mat4 modelHombroIzq = glm::mat4(1.0);
	glm::mat4 modelManoIzq = glm::mat4(1.0);
	glm::mat4 modelMusloDer = glm::mat4(1.0);
	glm::mat4 modelTibiaDer = glm::mat4(1.0);
	glm::mat4 modelPieDer = glm::mat4(1.0);
	glm::mat4 modelMusloIzq = glm::mat4(1.0);
	glm::mat4 modelTibiaIzq = glm::mat4(1.0);
	glm::mat4 modelPieIzq = glm::mat4(1.0);
	glm::mat4 modelEstatua = glm::mat4(1.0);
	glm::mat4 modelSantuario = glm::mat4(1.0);
	glm::mat4 modelCoche = glm::mat4(1.0);

	glm::vec3 tpWorldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 santuarioPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightWorldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lampWorldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 baseTpLocalPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 from = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 to = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 yae_worldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 carPos(0.0f), carTangent(0.0f, 0.0f, 1.0f);
	glm::vec3 carRight(1.0f, 0.0f, 0.0f);
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 lampSupWorldPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 dir = glm::vec3(1.0f, 1.0f, 1.0f);


	identidad = glm::mat4(1.0);

	int lamp2Count = 0.0f;
	int lamp3Count = 0.0f;
	int lamp_base_count = 0.0f;
	int lamp1_count = 0.0f;
	int nextWP = 0.0f;
	int yae_waypoint = 0;


	float tiempoActual = 0.0f;
	float pitchC = 0.0f;
	float legSwingR = 0.0f;
	float legSwingL = 0.0f;
	float kneeBendR = 15.0f;
	float kneeBendL = 15.0f;
	float armSwingR = 0.0f;
	float armSwingL = 0.0f;
	float bobY = 0.0f;
	float spd = 0.0f;
	float segLen = 0.0f;
	float yae_segT = 0.0f;
	float yae_yaw = 0.0f;
	float yae_pitch = 0.0f;
	float yae_walkTime = 0.0f;
	float yae_walkCycle = 0.0f;
	float tCoche = 0.0f;
	float rotllanta = 0.0f;
	float yawCoche = 0.0f;
	float pitchCoche = 0.0f;


	const float vel_yae = 17.0f;
	const float velTCoche = 0.0006f;

	bool cochePausado = false;
	bool teclaR = false;



	// =================== TERMINAN VARIABLES FERNANDO ================= //

	//===================INICIA ANIMACIONES FERNANDO=====================//

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

			{ glm::vec3(-1.275f,  5.854f, -25.645f), 0.3f },
			{ glm::vec3(-1.3635f, 5.848f, -31.405f), 0.3f },

			{ glm::vec3(-6.921f,  5.054f, -33.28f),  0.2f },
			{ glm::vec3(-10.133f, 4.165f, -32.725f), 0.2f },
			{ glm::vec3(-13.846f, 2.725f, -31.44f),  0.2f },

			{ glm::vec3(-10.133f, 4.165f, -32.725f), 0.2f },
			{ glm::vec3(-6.921f,  5.054f, -33.28f),  0.2f },
			{ glm::vec3(-1.3635f, 5.848f, -31.405f), 0.3f },
	};

	yae_worldPos = rutaYae[0].pos;

	//===================TERMINA ANIMACIONES FERNANDO=====================//



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

		// ---- Quetza
		deltaTime1 = now - lastTime;
		deltaTime1 += (now - lastTime) / limitFPS;
		lastTime1 = now;

		// ------------- MODIFICACIONES FERNANDO
		GLfloat rawDelta = now - lastTime;
		deltaTime = rawDelta + rawDelta / limitFPS;
		lastTime = now;
		// ------------- MODIFICACIONES FERNANDO

		//====================INICIA IMPLEMENTACION ANIMACIONES FERNANDO=======//
//para el tp
		tp_rotY += 0.1f * deltaTime;
		if (tp_rotY >= 360.0f) tp_rotY -= 360.0f;
		tp_bobTime += deltaTime;

		//para la muñeca fomu
		muneca_rotTime += deltaTime;

		//para la yae

		{
			nextWP = (yae_waypoint + 1) % (int)rutaYae.size();
			from = rutaYae[yae_waypoint].pos;
			to = rutaYae[nextWP].pos;
			spd = rutaYae[yae_waypoint].speed;

			segLen = glm::length(to - from);
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

				dir = to - from;
				if (glm::length(glm::vec2(dir.x, dir.z)) > 0.001f)
					yae_yaw = glm::degrees(atan2f(dir.x, dir.z));

				float hDist = glm::length(glm::vec2(dir.x, dir.z));
				if (hDist > 0.001f)
					yae_pitch = glm::degrees(atan2f(dir.y, hDist));
			}
		}
		yae_walkCycle = fmodf((float)glfwGetTime() * vel_yae, 2.0f * 3.14159f);


		//animacion para el coche de fernando

		bool* keys = mainWindow.getsKeys();
		bool teclaRActual = keys[GLFW_KEY_R];
		if (teclaRActual && !teclaR) {
			tCoche = 0.0f;
			rotllanta = 0.0f;
		}
		teclaR = teclaRActual;

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

		//====================FIN IMPLEMENTACION ANIMACIONES FERNANDO=======//




		//Recibir eventos del usuario
		glfwPollEvents();
		
		// ------------------ Cambio de camaras -----------------------
		// camera.keyControl(mainWindow.getsKeys(), deltaTime);
		// camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		// ------------------ Cambio de camaras -----------------------
		camera.controlarCamara(
			mainWindow.getsKeys(),
			deltaTime1,
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
		pointLights[48].SetPos(pos_obj);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		// Lampara 3
		model = identidad;
		model = glm::translate(model, glm::vec3(-40.0f, -1.0f, -120.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		pos_obj = glm::vec3(model[3]);
		pos_obj.y += 37.0f;
		pos_obj.x -= 1.0f;
		pointLights[49].SetPos(pos_obj);
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
		
		if (cientificoAnimacion >= 0.0f && cientificoAnimacion <= 5.0f) brazo_rotacion += deltaTime1 * 0.3;
		else brazo_rotacion -= deltaTime1 * 0.3;

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
		// INICIA ANIMACIÓN CON LA TECLA -> X
		if (mainWindow.animacionZeppelin()) {
			tiempoZeppelin += 0.01f * deltaTime1 * velocidadZeppelin;

			anguloActualZeppelin = anguloInicioZeppelin + tiempoZeppelin;

			posZeppelinX = centroZeppelinX + radioZeppelinX * cos(anguloActualZeppelin);
			posZeppelinZ = centroZeppelinZ + radioZeppelinZ * sin(anguloActualZeppelin);

			dirZeppelinX = -radioZeppelinX * sin(anguloActualZeppelin);
			dirZeppelinZ = radioZeppelinZ * cos(anguloActualZeppelin);

			giroZeppelin = atan2(dirZeppelinZ, -dirZeppelinX);

			// ------------- Giro helice ---------------------
			heliceGiro += deltaTime1 * 3.0f;
			if (heliceGiro >= 360.0f) heliceGiro = 0.0f;

			// ------------- TEXTURA FUEGO -------------------
			tiempoFuego += 0.05 * deltaTime1;

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
		estatua_dios_polo.RenderModel();

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


		// =============================================================================== ANGEL ================================================================================================

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





















		// ======================================================================================================================================================================================



		//========================OBJETOS FERNANDO=========================================//
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

		//========================TERMINAN OBJETOS FERNANDO=========================================//
		//
		model = identidad;
		model = glm::translate(model, glm::vec3(150.0, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		castillo.RenderModel();


		//blending: transparencia o traslucidez
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 0.6f, 9.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		



		// ============INICIAN RENDERIZADOS FERNANDO================================ //
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
			{  -14.836f,  1.703f, -33.37f  },
			{   -12.856f,  1.747f, -29.51f  },
			{   -10.783f,  3.27f,  -35.13f  },
			{   -9.483f,  3.06f,  -30.32f  },
			{   -6.871f,    3.973f, -30.85f  },
			{   -6.971f,  4.134f, -35.71f  },

			//entrada por el tp
			{   1.1f,    4.848f, -31.4f   },
			{  -3.827f,  4.848f, -31.41f  },
			{   1.182f,  4.86f,  -25.66f  },
			{  -3.732f,  4.848f, -25.63f  },

			//costado por la casa de entrada
			{   5.656f,  7.681f,  -2.562f },
			{   5.621f,  7.681f,   2.167f },
			{  -8.394f,  7.681f,  -2.545f },
			{  -8.362f,  7.681f,   2.268f },

			//costados de las otras dos casas
			{ -15.244f,  8.397f,  11.3f   },
			{ -21.774f,  8.397f,  14.56f  },
			{ -21.774f,  8.397f,  17.85f  },
			{ -15.324f,  8.397f,  21.04f  },
			{  12.546f,  8.397f,  11.38f  },
			{  18.976f,  8.397f,  14.54f  },
			{  18.976f,  8.397f,  17.97f  },
			{  12.546f,  8.397f,  21.02f  },

			//centro del patio interior
			{  -3.894f,  7.185f,  13.73f  },
			{   1.145f,  7.185f,  13.71f  },
			{   1.1f,    7.185f,  18.82f  },
			{  -3.839f,  7.185f,  18.82f  },

			//cerca del arbol principal
			{ -5.864f,  9.999f, 27.84f },
			{   3.123f, 9.999f, 27.85f  }
		};
		lamp2Count = sizeof(lamp2_positions) / sizeof(lamp2_positions[0]);

		for (int i = 0; i < lamp2Count; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, lamp2_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp2.RenderModel();

			santuarioPos = glm::vec3(modelSantuario[3]);
			lightWorldPos = santuarioPos + lamp2_positions[i] + glm::vec3(0.0f, 0.5f, 0.0f);
			pointLights[i + 1].SetPos(lightWorldPos);
		}

		//shaderList[0].SetPointLights(pointLights, pointLightCount);

		glDisable(GL_CULL_FACE);
		//-------------------------------LAMP3-----------------------------------//
		glm::vec3 lamp3_positions[] = {
			{ -16.01f,  5.433f, -0.88f },
			{ -15.94f,  5.179f, -4.07f }
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

		//---------------------------------LAMPARAS SEPARADAS-----------------------------------//
		glm::vec3 lamp_base_positions[] = {
				 {  1.102f,  4.102f, -19.98f },  // 1
				 { -3.766f,  4.134f, -19.98f },  // 2
				 {  0.900f,  7.172f, 20.001f},   //3 
				 {   -3.515,  7.076f,  20.001f},  // 4

		};
		lamp_base_count = sizeof(lamp_base_positions) / sizeof(lamp_base_positions[0]);

		for (int i = 0; i < lamp_base_count; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, lamp_base_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp_base.RenderModel();

			modelBase = model;
			model = modelBase;
			model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp_sup.RenderModel();

			lampSupWorldPos = glm::vec3(model[3]);
			lightWorldPos = lampSupWorldPos + glm::vec3(0.0f, 0.5f, 0.0f);
			pointLights[31 + i].SetPos(lightWorldPos);
		}

		//----------------------------------LAMP1-------------------------------------//
		glm::vec3 lamp1_positions[] = {
				{ 3.542f,  7.120f, -29.99f },
				{ 3.539f,  7.126f, -26.71f },
				{ 1.645f,  9.102f, -12.44f },
				{ -4.233f, 9.134f, -12.44f  },
				{ 1.223f,  10.502f, -5.02f },
				{ -4.223f, 10.534f, -5.02f  },
				{ 1.623f,  10.402f, 7.70f },
				{ -4.300f,  10.402f, 7.70f },
				{  -8.894f, 10.2f,  14.855f  },
				{  -8.839f,  10.2f,  17.7f  },
				{   6.145f,  10.2f,  14.855f  },
				{   6.1f,    10.2f,  17.7f  },
		};
		lamp1_count = sizeof(lamp1_positions) / sizeof(lamp1_positions[0]);

		for (int i = 0; i < lamp1_count; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, lamp1_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lamp1.RenderModel();

			lampWorldPos = glm::vec3(model[3]);
			lightWorldPos = lampWorldPos + glm::vec3(0.0f, 0.5f, 0.0f);
			pointLights[35 + i].SetPos(lightWorldPos);
		}

		glEnable(GL_CULL_FACE);

		//------------------------------------BASE TP----------------------------//		
		baseTpLocalPos = glm::vec3(-6.427f, 5.231f, -27.4f);
		modelBaseTp = modelSantuario;
		modelBaseTp = glm::translate(modelBaseTp, baseTpLocalPos);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseTp));
		base_tp.RenderModel();

		//------------------------------------------TP-----------------------//
		bobOffset = 0.3f * sinf(tp_bobTime * 0.01f);

		modelTp = modelBaseTp;
		model = modelTp;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f + bobOffset, 0.0f));
		model = glm::rotate(model, tp_rotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tp.RenderModel();

		tpWorldPos = glm::vec3(model[3]);
		pointLights[47].SetPos(tpWorldPos + glm::vec3(0.0f, 1.5f, 0.0f));
		
		//shaderList[0].SetPointLights(pointLights, pointLightCount);

		//--------------------------------ESTATUA ZORRO-----------------------------//


		glm::vec3 baseEstatua_positions[] = {
			{ -4.115f,  6.676f,  25.001f },
			{  1.500f,  6.672f,  25.001f }
		};

		glm::vec3 estatua_positions[] = {
			{ -0.081f, 5.476f,  0.181f },
			{ 0.181f, 5.492f,  0.181f }
		};

		for (int i = 0; i < 2; i++)
		{
			modelBaseEstatua = modelSantuario;
			modelBaseEstatua = glm::translate(modelBaseEstatua, baseEstatua_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseEstatua));
			base_estatua.RenderModel();

			modelEstatua = modelBaseEstatua;
			modelEstatua = glm::translate(modelEstatua, estatua_positions[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstatua));
			estatua.RenderModel();
		}

		// ------------------------------PUERTAS TORII-----------------------------------//
		glm::vec3 puerta_positions[] = {
				{ -12.846f,  1.725f, -31.64f  },
				{ -9.133f,  3.165f, -32.725f },
				{  -5.921f,  4.054f, -33.28f  }
		};

		float puerta_rotations[] = {
			25.0f,
			10.0f,
			0.0f
		};

		for (int i = 0; i < 3; i++)
		{
			model = modelSantuario;
			model = glm::translate(model, puerta_positions[i]);
			model = glm::rotate(model, puerta_rotations[i] * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			puerta.RenderModel();
		}

		// rendedr para la muñeca

	   //---------------------------------CUERPO FOMU---------------------------------------------//
		modelCuerpoMuneca = modelSantuario;
		modelCuerpoMuneca = glm::translate(modelCuerpoMuneca, glm::vec3(-1.372f, 7.185f, 16.27f));
		modelCuerpoMuneca = glm::scale(modelCuerpoMuneca, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCuerpoMuneca));
		cuerpo_muñeca.RenderModel();

		//------------------------------------CABEZA-------------------------------------------------//
		modelMuneca = modelCuerpoMuneca;
		modelMuneca = glm::translate(modelMuneca, glm::vec3(0.0f, 2.0f, 0.0f));
		GLfloat muneca_rotAngle = 45.0f * sinf(muneca_rotTime * 0.05f);
		modelMuneca = glm::rotate(modelMuneca, muneca_rotAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMuneca));
		muñeca.RenderModel();

		//render para el personaje
		// -----------------------------------------YAE MIKO-----------------------------------------//

		tiempoActual = fmodf((float)glfwGetTime(), 6.2831f);
		pitchC = (yae_pitch > 0.0f) ? -(yae_pitch * 0.5f) : -(yae_pitch * 0.8f);

		legSwingR = sinf(tiempoActual * 2.0f) * 45.0f;
		legSwingL = -sinf(tiempoActual * 2.0f) * 45.0f;
		armSwingR = -sinf(tiempoActual * 2.0f) * 45.0f;
		armSwingL = sinf(tiempoActual * 2.0f) * 45.0f;
		bobY = sinf(tiempoActual * 4.0f) * 0.015f;

		// base del personaje
		modelCharBase = modelSantuario;
		modelCharBase = glm::translate(modelCharBase, yae_worldPos + glm::vec3(0.0f, bobY + 1.0f, 0.0f));
		modelCharBase = glm::rotate(modelCharBase, (yae_yaw + 180.0f) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelCharBase = glm::rotate(modelCharBase, pitchC * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//torso
		modelTorso = modelCharBase;
		modelTorso = glm::scale(modelTorso, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorso));
		torso.RenderModel();

		// cabeza
		modelCabeza = modelTorso;
		modelCabeza = glm::translate(modelCabeza, glm::vec3(0.0f, 0.118f, 0.010f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabeza));
		cabeza.RenderModel();

		// hombro der
		modelHombroDer = modelTorso;
		modelHombroDer = glm::translate(modelHombroDer, glm::vec3(0.034f, 0.090f, 0.020f));
		modelHombroDer = glm::rotate(modelHombroDer, armSwingR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHombroDer));
		hombro_der.RenderModel();

		// mano der
		modelManoDer = modelHombroDer;
		modelManoDer = glm::translate(modelManoDer, glm::vec3(0.048f, -0.040f, 0.001f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelManoDer));
		mano_der.RenderModel();

		// hombor izq
		modelHombroIzq = modelTorso;
		modelHombroIzq = glm::translate(modelHombroIzq, glm::vec3(-0.033f, 0.092f, 0.019f));
		modelHombroIzq = glm::rotate(modelHombroIzq, armSwingL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHombroIzq));
		hombro_izq.RenderModel();

		//mano izq
		modelManoIzq = modelHombroIzq;
		modelManoIzq = glm::translate(modelManoIzq, glm::vec3(-0.059f, -0.045f, 0.002f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelManoIzq));
		mano_izq.RenderModel();

		//muslo der
		modelMusloDer = modelTorso;
		modelMusloDer = glm::translate(modelMusloDer, glm::vec3(0.025f, -0.073f, 0.002f));
		modelMusloDer = glm::rotate(modelMusloDer, legSwingR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMusloDer));
		muslo_der.RenderModel();

		// tibia der
		modelTibiaDer = modelMusloDer;
		modelTibiaDer = glm::translate(modelTibiaDer, glm::vec3(-0.007f, -0.119f, 0.005f));
		modelTibiaDer = glm::rotate(modelTibiaDer, -kneeBendR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTibiaDer));
		tibia_der.RenderModel();

		// pie der
		modelPieDer = modelTibiaDer;
		modelPieDer = glm::translate(modelPieDer, glm::vec3(-0.005f, -0.145f, 0.018f));
		modelPieDer = glm::rotate(modelPieDer, (kneeBendR * 0.5f) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPieDer));
		pie_der.RenderModel();

		// muslo izq
		modelMusloIzq = modelTorso;
		modelMusloIzq = glm::translate(modelMusloIzq, glm::vec3(-0.027f, -0.076f, 0.001f));
		modelMusloIzq = glm::rotate(modelMusloIzq, legSwingL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMusloIzq));
		muslo_izq.RenderModel();

		// tibia izq
		modelTibiaIzq = modelMusloIzq;
		modelTibiaIzq = glm::translate(modelTibiaIzq, glm::vec3(0.007f, -0.117f, 0.008f));
		modelTibiaIzq = glm::rotate(modelTibiaIzq, -kneeBendL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTibiaIzq));
		tibia_izq.RenderModel();

		//pie izq
		modelPieIzq = modelTibiaIzq;
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

		if (flagDiaNoche) shaderHojas.SetDirectionalLight(&mainLightDia);
		else shaderHojas.SetDirectionalLight(&mainLightNoche);
		//shaderHojas.SetDirectionalLight(&mainLight);
		
		//shaderHojas.SetPointLights(pointLights, pointLightCount);
		if (flagDiaNoche) shaderHojas.SetPointLights(pointLights, 0);
		else shaderHojas.SetPointLights(pointLights, pointLightCount);

		//shaderHojas.SetSpotLights(spotLights, spotLightCount);
		if (mainWindow.luzTren() && mainWindow.luzEstatua()) {
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights1[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderHojas.SetSpotLights(spotLights1, spotLightCount1);
		}
		else if (mainWindow.luzTren()) {
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderHojas.SetSpotLights(spotLights2, spotLightCount2);
		}
		else if (mainWindow.luzEstatua()) {
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights3[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderHojas.SetSpotLights(spotLights3, spotLightCount3);
		}
		else {
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights4[0].SetFlash(lowerLight, camera.getCameraDirection());

			shaderHojas.SetSpotLights(spotLights4, spotLightCount4);
		}

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

		//render arbusto
		model = modelSantuario;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		arbusto.RenderModel();
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		// =========================TERMINAN RENDERIZADOS FERNANDO============================ //





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
