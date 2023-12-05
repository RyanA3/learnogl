#pragma once

#include "camera.h"
#include "CoordinateCross.h"

class LockedCamera : public Camera {

private:
	float cursor_last_x = 400.0f;
	float cursor_last_y = 300.0f;

public:
	glm::vec3 locked_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotate_sensitivity = 0.01f;
	float base_move_speed = 1.0f;
	float base_zoom_speed = 5.0f;
	float zoom_speed = 0.0f;
	float move_speed = 0.0f;
	float distance = 2.0f;

	CoordinateCross cross;

	LockedCamera();

	LockedCamera(glm::vec3 locked_pos, float distance);

	LockedCamera(glm::vec3 pos, glm::vec3 forward, float pitch, float yaw);

	/*
	* Updates the location of the camera based off of
	* mouse movement
	*/
	void processMouseInput(GLFWwindow* window, double xpos, double ypos);

	/*
	* Updates the distance of the camera based off of
	* key inputs
	*/
	void processKeyInput(GLFWwindow* window, float delta_time);

	/*
	* Updates the phi and theta coordinates of the camera
	* phi & theta in radians
	*/
	void updatePhiTheta(float phi, float theta);

	/*
	* Overrides updateVectors to update the pos and forward
	* vectors of the camera based off the spherical coordinate
	* system
	*/
	void updateVectors();

};

