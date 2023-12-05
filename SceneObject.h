#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "model.h"

class SceneObject {
private:
	Model& model;
	glm::vec3 pos;
	float scale = 1.0f;
	bool shouldUpload = true;

public:
	SceneObject(Model& model, glm::vec3 pos);

	void setScale(float scale);
	void setPos(glm::vec3 pos);
	void draw(Shader& shader);


};