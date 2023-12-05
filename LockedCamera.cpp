#include "LockedCamera.h"

LockedCamera::LockedCamera()
	: Camera() {
	this->yaw = 0;
	this->pitch = glm::radians(90.0f);
	cross = CoordinateCross();
};

LockedCamera::LockedCamera(glm::vec3 locked_pos, float distance)
	: Camera(locked_pos, 0, 0) {
	this->locked_pos = locked_pos;
	this->distance = distance;
	this->yaw = 0;
	this->pitch = glm::radians(90.0f);
	cross = CoordinateCross(locked_pos, 1.0f);
};

LockedCamera::LockedCamera(glm::vec3 loc, glm::vec3 forward, float pitch, float yaw)
	: Camera(loc, pitch, yaw) {
	this->distance = 1;
	this->forward = forward;
	this->locked_pos = forward + loc;
	cross = CoordinateCross(locked_pos, 1.0f);
}

void LockedCamera::processMouseInput(GLFWwindow* window, double xpos, double ypos) {

	//Calculate difference between new and previous mouse positions
	float xoff = xpos - cursor_last_x;
	float yoff = cursor_last_y - ypos; //Reversed since y coordinates increase from bottom to top
	cursor_last_x = xpos;
	cursor_last_y = ypos;

	//Only revolve the camera if the left mouse button is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
		return;

	//Multiply the difference by sensitivity
	xoff *= rotate_sensitivity;
	yoff *= rotate_sensitivity;

	//Add mouse movement to phi & theta
	this->updatePhiTheta(pitch + yoff, yaw + xoff);

};

void LockedCamera::processKeyInput(GLFWwindow* window, float delta_time) {
	zoom_speed = base_zoom_speed * delta_time;
	move_speed = base_move_speed * delta_time;

	//Process input for camera
	//Updates distance from target
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		distance -= zoom_speed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		distance += zoom_speed;

	//Updates target location
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		locked_pos.z -= move_speed;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		locked_pos.z += move_speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		locked_pos.x -= move_speed;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		locked_pos.x += move_speed;
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		locked_pos.y += move_speed;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		locked_pos.y -= move_speed;

	updateVectors();

};


const float phi_max = glm::radians(179.0f);
const float phi_min = glm::radians(1.0f);
void LockedCamera::updatePhiTheta(float phi, float theta) {

	//Limit phi between 0 and pi radians
	if (phi > phi_max) phi = phi_max;
	else if (phi < phi_min) phi = phi_min;

	//Pitch and yaw are re-used as phi and theta
	this->pitch = phi;
	this->yaw = theta;

	updateVectors();

};

void LockedCamera::updateVectors() {
	this->pos = glm::vec3(
		locked_pos.x + distance * sin(pitch) * cos(yaw),
		locked_pos.y + distance * cos(pitch),		//Y is treated as Z since the axes are swapped in OpenGL
		locked_pos.z + distance * sin(pitch) * sin(yaw)
	);

	this->forward = locked_pos - pos;
};