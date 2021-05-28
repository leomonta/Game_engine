#include "Camera.hpp"
#include <iostream>

// create the camera from a position
Camera::Camera(glm::vec3 pos) {
	m_cameraPosition = pos;
	m_cameraRight	 = glm::vec3(1.0f, 0.0f, 0.0f);
	m_cameraUp		 = glm::vec3(0.0f, 1.0f, 0.0f);
	m_cameraFront	 = glm::vec3(0.0f, 0.0f, 1.0f);
	m_cameraWatching = glm::vec3(0.0f, 0.0f, -1.0f);
}

// move the camera by an offset and return the view matrix
glm::mat4 Camera::move(glm::vec3 offset) {

	m_cameraPosition += offset;

	//												the Up is the y axys
	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraWatching, glm::vec3(0.0f, 1.0f, 0.0f));
}

// return the perspective projection matrix
glm::mat4 Camera::getPerspective(float width, float height) {
	return glm::perspective(glm::radians(m_FOV), width / height, 0.1f, 100.0f);
}

// get the view matrix from mouse coordinate
glm::mat4 Camera::point(float posx, float posy) {

	float sensitivity = 0.1f;

	m_pitch += posy * sensitivity;
	m_yaw += posx * sensitivity;

	m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

	m_cameraWatching.x = float(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
	m_cameraWatching.y = float(sin(glm::radians(m_pitch)));
	m_cameraWatching.z = float(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
	m_cameraWatching, m_cameraFront = glm::normalize(m_cameraWatching);

	m_cameraFront.y = 0.0f;
	m_cameraRight	= glm::cross(m_cameraFront, m_cameraUp);

	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraWatching, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::getFrontSpeed() {
	return m_cameraFront * m_cameraSpeedConst;
}

glm::vec3 Camera::getRightSpeed() {
	return m_cameraRight * m_cameraSpeedConst;
}

// update FOV
void Camera::zoom(float diff_FOV) {
	m_FOV = glm::clamp(m_FOV + diff_FOV, 10.0f, 110.0f);
}