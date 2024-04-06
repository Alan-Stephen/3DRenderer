#pragma once
#include "glm/glm.hpp"
#include <string>
#include "Shader.h"

class PointLight {
public:
	PointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float constant, float linear, float quadratic) :
		position(position), diffuse(diffuse), specular(specular), ambient(ambient),
		constant(constant), linear(linear), quadratic(quadratic) {}
	PointLight() {}
	void bind_at(unsigned int slot, std::string array_name, const Shader &shader) const;

private:
	glm::vec3 position;   
	glm::vec3 diffuse;    
	glm::vec3 specular;   
	glm::vec3 ambient;   

	float constant;
	float linear;
	float quadratic;
};

class DirectionalLight {
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {}

    void bind(const Shader& shader) const;
private:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class SpotLight {
public:
    SpotLight() {};
    SpotLight(const glm::vec3& _position, const glm::vec3& _direction, 
              const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, 
              float _cutoff, float _outer_cutoff)
        : position(_position), direction(_direction), ambient(_ambient), diffuse(_diffuse),
          specular(_specular), cutoff(_cutoff), outer_cutoff(_outer_cutoff) {}

    void bind_at(unsigned int slot, const std::string& array_name, const Shader& shader) const;
private:
    glm::vec3 position;
    glm::vec3 direction;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float cutoff;
    float outer_cutoff;
};


