#pragma once
#include <glm/glm.hpp>
#include "DataUtils.h"



//Used to keep track of how many lights there are to give each new one a unique id
static int last_light_id = 0;



class PointLight : public BinarySearchable {

public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic);

	glm::vec3
		ambient,
		diffuse,
		specular,
		position;

	float
		constant,
		linear,
		quadratic;

};

class SpotLight : public BinarySearchable {

public:
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic, float inner_cutoff, float outer_cutoff);

	glm::vec3
		ambient,
		diffuse,
		specular,
		position,
		direction;

	float
		constant,
		linear,
		quadratic,

		inner_cutoff,
		outer_cutoff;

};

class DirectionalLight : public BinarySearchable {

public:
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);

	glm::vec3
		ambient,
		diffuse,
		specular,
		direction;

};