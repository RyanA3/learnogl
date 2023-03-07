#pragma once
#include <glm/glm.hpp>

struct PointLight {

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

struct SpotLight {

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

struct DirectionalLight {

	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);

	glm::vec3
		ambient,
		diffuse,
		specular,
		direction;
};