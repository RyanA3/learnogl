#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glfw-3.3.8/include/GLFW/glfw3.h>

class Camera {

public:
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 pos;

	float pitch;
	float yaw;
	

	Camera() {
		this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
		this->pitch = 0.0f;
		this->yaw = -90.0f;
	}

	Camera(glm::vec3 pos, float pitch, float yaw) {
		this->pos = pos;
		this->pitch = pitch;
		this->yaw = yaw;
		updateVectors();
	}

	

	/*
	* Re-calculates the forward vector of the camera
	* Should be called whenever the camera's pitch or yaw is modified
	*/
	void updateVectors() {
		forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		forward.y = sin(glm::radians(pitch));
		forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	}

	/*
	* Sets the pitch and yaw of the camera
	*/
	void setPitchYaw(float pitch, float yaw) {

		//Limit pitch to prevent funkyness at pitch=90
		if (pitch > 89.0f) pitch = 89.0f;
		else if (pitch < -89.0f) pitch = -89.0f;

		this->pitch = pitch;
		this->yaw = yaw;
		updateVectors();
	}

};



class SpectatorCamera : public Camera {

private:
	float cursor_last_x = 400;
	float cursor_last_y = 300;
	float speed = 0.0f;

public:
	float sensitivity;
	float base_speed;

	SpectatorCamera() {
		sensitivity = 0.1f;
		base_speed = 2.5f;
	}

	SpectatorCamera(float sensitivity, float base_speed, glm::vec3 pos, float pitch, float yaw)
	: Camera(pos, pitch, yaw) {
		this->sensitivity = sensitivity;
		this->base_speed = base_speed;
	}



	/*
	* Updates the direction the camera is facing based off of
	* mouse movement
	*/
	void processMouseInput(GLFWwindow* window, double xpos, double ypos) {
		
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
	}

	/*
	* Updates the location of the camera based off of
	* key inputs
	*/
	void processKeyInput(GLFWwindow* window, float delta_time) {
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
	}

};


#endif