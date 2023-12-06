#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "shader.h"


class Camera {

public:
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 pos = glm::vec3(0.0f);

	float pitch = 0;
	float yaw = 180;

	float fov = glm::radians(45.0f),
		aspectRatio = 3.0f/2.0f, 
		nearPlane = 0.1f, 
		farPlane = 100;

	bool 
		shouldUpdateView = true,
		shouldUpdateProjection = true;
	

	Camera();

	Camera(glm::vec3 pos, float pitch, float yaw);

	

	/*
	* Re-calculates the forward vector of the camera
	* Should be called whenever the camera's pitch or yaw is modified
	*/
	void updateForwardVector();

	/*
	* Sets the pitch and yaw of the camera
	*/
	void setPitchYaw(float pitch, float yaw);

	void setPos(glm::vec3 pos);
	void addPos(glm::vec3 dir);

	/*
	* Updates projection and view matrices for the given shader (if nessecery)
	*/
	void uploadMatrices(Shader& shader);

	/*
	* Updates the shouldUpdate booleans after at the end of a full rendering schtick
	* This allows multiple shaders to have view and projection updated in one draw loop upload
	*/
	void postdrawUpdate();

};



class SpectatorCamera : public Camera {

private:
	float cursor_last_x = 400.0f;
	float cursor_last_y = 300.0f;
	float speed = 0.0f;

public:
	float sensitivity = 0.1f;
	float base_speed = 2.5f;

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