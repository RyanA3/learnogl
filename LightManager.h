#pragma once
#include <vector>
#include <iostream>
#include "shader.h"
#include "Light.h"




class LightManager {

private:
	std::vector<PointLight> point_lights;
	std::vector<SpotLight> spot_lights;
	std::vector<DirectionalLight> dir_lights;

public:
	LightManager();
	
	void uploadLights(Shader& shader);
	
	void addPointLight(PointLight light);
	void addSpotLight(SpotLight light);
	void addDirLight(DirectionalLight light);

};