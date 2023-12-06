#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "model.h"


class SceneObject {
private:
	Model& model;
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);
	glm::vec3 netForce = glm::vec3(0.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat3 normalMatrix = glm::mat3(1.0f);

	float inverseMass = 1.0f / 1.0f;
	float scale = 1.0f;
	bool shouldUpload = true;

public:
	SceneObject(Model& model);
	SceneObject(Model& model, glm::vec3 pos);
	SceneObject(Model& model, glm::vec3 pos, glm::vec3 velocity);

	void setScale(float scale);
	void setMass(float mass);

	void setPos(glm::vec3 pos);
	void addPos(glm::vec3 pos);
	void setVelocity(glm::vec3 velocity);
	void addVelocity(glm::vec3 velocity);

	void applyForce(glm::vec3 force);
	void update(float delta_time);

	void draw(Shader& shader);

};