#include "Light.h"

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->uid = last_light_id++;
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, 
	float constant, float linear, float quadratic, float inner_cutoff, float outer_cutoff) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->position = position;
	this->direction = direction;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->inner_cutoff = inner_cutoff;
	this->outer_cutoff = outer_cutoff;
	this->uid = last_light_id++;
}

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->direction = direction;
	this->uid = last_light_id++;
}