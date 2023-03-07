#include "LightManager.h"



LightManager::LightManager() {
	point_lights = std::vector<PointLight>();
	spot_lights = std::vector<SpotLight>();
	dir_lights = std::vector<DirectionalLight>();
};



void LightManager::uploadLights(Shader& shader) {

	//Upload point lights
	for (unsigned int i = 0; i < shader.num_point_lights && i < point_lights.size(); i++) {

		std::string id = "point_lights[" + std::to_string(i) + "]";

		std::cout << "Upload point light " << id << std::endl;
		std::cout << point_lights[i].diffuse.x << point_lights[i].diffuse.y << point_lights[i].diffuse.z << std::endl;

		shader.setVec3(id + ".ambient", point_lights[i].ambient);
		shader.setVec3(id + ".diffuse", point_lights[i].diffuse);
		shader.setVec3(id + ".specular", point_lights[i].specular);
		shader.setVec3(id + ".position", point_lights[i].position);

		shader.setFloat(id + ".constant", point_lights[i].constant);
		shader.setFloat(id + ".linear", point_lights[i].linear);
		shader.setFloat(id + ".quadratic", point_lights[i].quadratic);

	}

	//Upload spot lights
	for (unsigned int i = 0; i < shader.num_spot_lights && i < spot_lights.size(); i++) {

		std::string id = "spot_lights[" + std::to_string(i) + "]";
		shader.setVec3(id + ".ambient", spot_lights[i].ambient);
		shader.setVec3(id + ".diffuse", spot_lights[i].diffuse);
		shader.setVec3(id + ".specular", spot_lights[i].specular);
		shader.setVec3(id + ".position", spot_lights[i].position);
		shader.setVec3(id + ".direction", spot_lights[i].direction);

		shader.setFloat(id + ".constant", spot_lights[i].constant);
		shader.setFloat(id + ".linear", spot_lights[i].linear);
		shader.setFloat(id + ".quadratic", spot_lights[i].quadratic);

		shader.setFloat(id + ".inner_cutoff", spot_lights[i].inner_cutoff);
		shader.setFloat(id + ".outer_cutoff", spot_lights[i].outer_cutoff);

	}

	//Upload directional light
	if (dir_lights.size() > 0) {

		std::string id = "directional_light";
		shader.setVec3(id + ".ambient", dir_lights[0].ambient);
		shader.setVec3(id + ".diffuse", dir_lights[0].diffuse);
		shader.setVec3(id + ".specular", dir_lights[0].specular);
		shader.setVec3(id + ".direction", dir_lights[0].direction);

	}

}


void LightManager::addPointLight(PointLight light) {
	point_lights.push_back(light);
}

void LightManager::addSpotLight(SpotLight light) {
	spot_lights.push_back(light);
}

void LightManager::addDirLight(DirectionalLight light) {
	dir_lights.push_back(light);
}