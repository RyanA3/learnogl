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



SpectatorCamera::SpectatorCamera() 
	: Camera() {
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
		pos += speed * forward * glm::vec3(1.0f, 0.0f, 1.0f);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= speed * forward * glm::vec3(1.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= (glm::normalize(glm::cross(forward, up)) * speed) * glm::vec3(1.0f, 0.0f, 1.0f);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += (glm::normalize(glm::cross(forward, up)) * speed) * glm::vec3(1.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		pos.y -= speed;
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos.y += speed;
};




CoordinateCross::CoordinateCross() {
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	generateModelData();
};

CoordinateCross::CoordinateCross(glm::vec3 pos, float scale) {
	this->pos = pos;
	this->scale = scale;
	generateModelData();
};

void CoordinateCross::generateModelData() {

	float new_vertices[] = {
		//x-axis (red)
		-(scale * neg_base), 0.0f, 0.0f,   100.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 (scale * pos_base), 0.0f, 0.0f,   254.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		//y-axis (green)
		0.0f, -(scale * neg_base), 0.0f,   0.0f, 100.0f, 0.0f,   0.0f, 0.0f,
		0.0f,  (scale * pos_base), 0.0f,   0.0f, 254.0f, 0.0f,   0.0f, 0.0f,
		//z-axis (blue)
		0.0f, 0.0f, -(scale * neg_base),   0.0f, 0.0f, 100.0f,   0.0f, 0.0f,
		0.0f, 0.0f,  (scale * pos_base),   0.0f, 0.0f, 254.0f,   0.0f, 0.0f
	};

	for (int i = 0; i < 48; i++) {
		vertices[i] = new_vertices[i];
	}

};

void CoordinateCross::generateVAO() {

	//Generate VBO
	glGenBuffers(1, &VBO);

	//Generate EBO
	glGenBuffers(1, &EBO);

	//Generate & Bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Load vertices into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Load indices into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Format the VAO
	// - location data (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// - color data (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// - texel data (x, y);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

};

unsigned int CoordinateCross::getVAO() {
	return VAO;
};

void CoordinateCross::draw(Shader& shader, glm::vec3 location) {

	shader.setBool("color_mode", true);

	//Bind VAO
	glBindVertexArray(VAO);

	//Generate translated matrix for the coordinate cross's model and update it
	glm::mat4 cross_matrix = glm::mat4(1.0f);
	cross_matrix = glm::translate(cross_matrix, location);
	shader.setMat4("model_matrix", cross_matrix);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	//Unbind VAO
	glBindVertexArray(0);

}



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
		locked_pos.y + distance * cos(pitch),		//Y is treated as Z since the axes are swapped in OpemGL
		locked_pos.z + distance * sin(pitch) * sin(yaw)
	);

	this->forward = locked_pos - pos;
};