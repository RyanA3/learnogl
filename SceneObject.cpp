#include "SceneObject.h"

SceneObject::SceneObject(Model& model) : model(model) {};
SceneObject::SceneObject(Model& model, glm::vec3 pos) : model(model), pos(pos) {};
SceneObject::SceneObject(Model& model, glm::vec3 pos, glm::vec3 velocity) : model(model), pos(pos), velocity(velocity) {};

void SceneObject::setScale(float scale) {
	this->scale = scale;
	this->shouldUpload = true;
}

void SceneObject::setMass(float mass) {
	this->inverseMass = 1 / mass;
}

void SceneObject::setPos(glm::vec3 pos) {
	this->pos = pos;
	this->shouldUpload = true;
}

void SceneObject::addPos(glm::vec3 pos) {
	this->pos += pos;
	this->shouldUpload = true;
}

void SceneObject::setVelocity(glm::vec3 velocity) {
	this->velocity = velocity;
}

void SceneObject::addVelocity(glm::vec3 velocity) {
	this->velocity += velocity;
}

void SceneObject::applyForce(glm::vec3 force) {
	netForce += force;
	acceleration += force * inverseMass;
}

void SceneObject::update(float delta_time) {

	velocity += acceleration * delta_time;
	
	//Don't update anything on the gpu if we aren't moving
	if (velocity.x == 0 && velocity.y == 0 && velocity.z == 0) return;
	pos += velocity * delta_time;
	shouldUpload = true;

}

void SceneObject::draw(Shader& shader) {

	if (shouldUpload) {
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		shader.setMat4("model", modelMatrix);

		normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
		shader.setMat3("normal_matrix", normalMatrix);

		shouldUpload = false;
	}
	model.draw(shader);

}