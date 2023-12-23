#include "camera.h"

Camera::Camera() {

};

Camera::Camera(glm::vec3 pos, float pitch, float yaw) {
	this->pos = pos;
	this->pitch = pitch;
	this->yaw = yaw;
	updateForwardVector();
};

void Camera::updateForwardVector() {
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
	updateForwardVector();
	this->shouldUpdateView = true;

};

void Camera::setPos(glm::vec3 pos) {
	this->pos = pos;
	this->shouldUpdateView = true;
}

void Camera::addPos(glm::vec3 dir) {
	this->pos += dir;
	this->shouldUpdateView = true;
}

void Camera::uploadMatrices(Shader& shader) {

	if (shouldUpdateProjection) {
		glm::mat4 projection_matrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
		shader.setMat4("projection", projection_matrix);
	}

	if (shouldUpdateView) {
		glm::mat4 view_matrix = glm::lookAt(pos, pos + forward, up);
		shader.setMat4("view", view_matrix);
		shader.setVec3("view_pos", pos);
	}

}

void Camera::postdrawUpdate() {
	shouldUpdateProjection = false;
	shouldUpdateView = false;
}



SpectatorCamera::SpectatorCamera() 
	: Camera() {};

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

	//Forward/backward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		addPos(speed * forward * glm::vec3(1.0f, 0.0f, 1.0f));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		addPos(-speed * forward * glm::vec3(1.0f, 0.0f, 1.0f));

	//Strafe left/right
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
		addPos(-(glm::normalize(glm::cross(forward, up) * glm::vec3(1.0f, 0.0f, 1.0f)) * speed));
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		addPos((glm::normalize(glm::cross(forward, up) * glm::vec3(1.0f, 0.0f, 1.0f)) * speed));

	//Up/down
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		addPos(glm::vec3(0, -speed, 0));
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
		addPos(glm::vec3(0, speed, 0));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		addPos(glm::vec3(0, speed * 10, 0));
	
};


