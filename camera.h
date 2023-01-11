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
	

	Camera();

	Camera(glm::vec3 pos, float pitch, float yaw);

	

	/*
	* Re-calculates the forward vector of the camera
	* Should be called whenever the camera's pitch or yaw is modified
	*/
	void updateVectors();

	/*
	* Sets the pitch and yaw of the camera
	*/
	void setPitchYaw(float pitch, float yaw);

};



class SpectatorCamera : public Camera {

private:
	float cursor_last_x = 400;
	float cursor_last_y = 300;
	float speed = 0.0f;

public:
	float sensitivity;
	float base_speed;

	SpectatorCamera();

	SpectatorCamera(float sensitivity, float base_speed, glm::vec3 pos, float pitch, float yaw);



	/*
	* Updates the direction the camera is facing based off of
	* mouse movement
	*/
	void processMouseInput(GLFWwindow* window, double xpos, double ypos);

	/*
	* Updates the location of the camera based off of
	* key inputs
	*/
	void processKeyInput(GLFWwindow* window, float delta_time);

};


#endif