#include "SceneObject.h"

SceneObject::SceneObject(Model& modell, glm::vec3 pos) : model(modell), pos(pos) {

}

void SceneObject::setScale(float scale) {
	this->scale = scale;
	this->shouldUpload = true;
}

void SceneObject::setPos(glm::vec3 pos) {
	this->pos = pos;
	this->shouldUpload = true;
}

void SceneObject::draw(Shader& shader) {

	if (shouldUpload) {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		shader.setMat4("model", modelMatrix);

		glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
		shader.setMat3("normal_matrix", normalMatrix);

		shouldUpload = false;
	}
	model.draw(shader);

}