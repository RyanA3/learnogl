#include "Scene.h"

Scene::Scene() {};

void Scene::addObject(SceneObject& object) {
	objects.push_back(object);
};

void Scene::update(float delta_time) {
	for (int i = 0; i < objects.size(); i++) {
		objects[i].update(delta_time);
	}
}

void Scene::draw(Shader& shader) {
	for (int i = 0; i < objects.size(); i++) {
		objects[i].draw(shader);
	}
}