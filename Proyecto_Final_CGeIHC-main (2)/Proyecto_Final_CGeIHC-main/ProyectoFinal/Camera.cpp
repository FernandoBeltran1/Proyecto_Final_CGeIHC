#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();

	// ------------------ Cambio de camaras -----------------------
	camaraAnterior = 1;

	for (int i = 0; i < 5; i++)
	{
		estadosCamara[i].position = position;
		estadosCamara[i].yaw = yaw;
		estadosCamara[i].pitch = pitch;
	}

	estadosCamara[2].position.y = 15.0f;
	estadosCamara[3].position.y = 70.0f;
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

// ------------------ Cambio de camaras -----------------------

void Camera::guardarEstado(int camara)
{
	estadosCamara[camara].position = position;
	estadosCamara[camara].yaw = yaw;
	estadosCamara[camara].pitch = pitch;
}

void Camera::restaurarEstado(int camara)
{
	position = estadosCamara[camara].position;
	yaw = estadosCamara[camara].yaw;
	pitch = estadosCamara[camara].pitch;

	update();
}

void Camera::controlarCamara(bool* keys, GLfloat deltaTime, GLfloat xChange, GLfloat yChange, int camaraActual, int vistaEstaticaActual)
{
	if (camaraActual != camaraAnterior)
	{
		guardarEstado(camaraAnterior);
		restaurarEstado(camaraActual);
		camaraAnterior = camaraActual;
	}

	if (camaraActual == 1)
	{
		keyControl(keys, deltaTime);
		mouseControl(xChange, yChange);
	}
	else if (camaraActual == 2)
	{
		keyControlXZ(keys, deltaTime, 15.0f);
		mouseControl(xChange, yChange);
		position.y = 15.0f;
	}
	else if (camaraActual == 3)
	{
		keyControlXZ(keys, deltaTime, 100.0f);
		mouseControl(xChange, yChange);
		position.y = 100.0f;
	}
	else if (camaraActual == 4)
	{
		camaraEstatica(vistaEstaticaActual);
	}
}

void Camera::keyControlXZ(bool* keys, GLfloat deltaTime, GLfloat alturaFija)
{
	GLfloat velocity = moveSpeed * deltaTime;

	glm::vec3 frontXZ = glm::vec3(front.x, 0.0f, front.z);
	glm::vec3 rightXZ = glm::vec3(right.x, 0.0f, right.z);

	if (glm::length(frontXZ) > 0.0f) {
		frontXZ = glm::normalize(frontXZ);
	}

	if (glm::length(rightXZ) > 0.0f) {
		rightXZ = glm::normalize(rightXZ);
	}

	if (keys[GLFW_KEY_W])
	{
		position += frontXZ * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= frontXZ * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= rightXZ * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += rightXZ * velocity;
	}

	position.y = alturaFija;
}

void Camera::camaraEstatica(int vistaEstaticaActual)
{
	glm::vec3 objetivo;

	if (vistaEstaticaActual == 0)
	{
		position = glm::vec3(30.0f, 50.0f, 100.0f);
		objetivo = glm::vec3(0.0f, 10.0f, 0.0f);
	}
	else if (vistaEstaticaActual == 1)
	{
		position = glm::vec3(30.0f, 50.0f, -100.0f);
		objetivo = glm::vec3(40.0f, 15.0f, 0.0f);
	}
	else if (vistaEstaticaActual == 2)
	{
		position = glm::vec3(-30.0f, 50.0f, -100.0f);
		objetivo = glm::vec3(-40.0f, 20.0f, 20.0f);
	}

	front = glm::normalize(objetivo - position);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
