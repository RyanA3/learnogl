#include "camera.h"

Camera::Camera() {
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	this->pitch = 0.0f;
	this->yaw = -90.0f;
};

Camera::Camera(glm::vec3 pos, float pitch, float yaw) {
	this->pos = pos;
	this->pitch = pitch;
	this->yaw = yaw;
	updateVectors();
};

void Camera::updateVectors() {
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
};

void Camera::setPitchYaw(float pitch, float yaw) {

	//Limit pitch to prevent funkyness at pitch=90
	if (pitch > 89.0f) pitch = 89.0f;
	else if (pitch < -89.0f) pitch = -89.0f;

	this->pitch = pitch;
	this->yaw = yaw;
	updateVectors();

};



SpectatorCamera::SpectatorCamera() {
	this->sensitivity = 0.1f;
	this->base_speed = 2.5f;
};

SpectatorCamera::SpectatorCamera(float sensitivity, float base_speed, glm::vec3 pos, float pitch, float yaw)
	: Camera(pos, pitch, yaw) {
	this->sensitivity = sensitivity;
	this->base_speed = base_speed;
};

void SpectatorCamera::processMouseInput(GLFWwindow* window, double xpos, double ypos) {

	//Calculate difference between new and previous mouse positions
	float xoff = xpos - cursor_last_x;
	float yoff = cursor_last_y - ypos; //Reversed since y coordinates increase from bottom to top
	cursor_last_x = xpos;
	cursor_last_y = ypos;

	//Multiply the difference by sensitivity
	xoff *= sensitivity;
	yoff *= sensitivity;

	//Add mouse movement to pitch & yaw
	this->setPitchYaw(pitch + yoff, yaw + xoff);
};

void SpectatorCamera::processKeyInput(GLFWwindow* window, float delta_time) {
	speed = base_speed * delta_time;

	//Process input for camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += speed * forward;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= speed * forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= glm::normalize(glm::cross(forward, up)) * speed;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += glm::normalize(glm::cross(forward, up)) * speed;
};