#pragma once

#include "SceneObject.h"
#include <vector>

class Scene {

private:
	std::vector<SceneObject> objects;

public:
	Scene();

	void addObject(SceneObject& object);

	void update(float delta_time);
	void draw(Shader& shader);

};