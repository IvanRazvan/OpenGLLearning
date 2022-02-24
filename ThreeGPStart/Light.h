#pragma once

#include "Helper.h"

// DirectionalLight holds data use to compute a directional light
class DirectionalLight {
public:
	glm::vec3 m_direction;
	glm::vec3 m_colour;
	float m_intensity{ 0 };

	DirectionalLight() = default;
	DirectionalLight(glm::vec3 direction, glm::vec3 colour, float intensity);
	void Set(glm::vec3 direction, glm::vec3 colour, float intensity);
};

// Light holds data use to create other types of light
class Light
{
public:
	glm::vec3 m_position;
	glm::vec3 m_colour;
	float m_intensity{ 0 };

	Light() = default;
};

// PointLight holds data use to compute point light
class PointLight
	: public Light
{
public:
	float m_range{ 0 };

	PointLight() = default;
	PointLight(glm::vec3 position, glm::vec3 colour, float intensity, float range);
	void Set(glm::vec3 position, glm::vec3 colour, float intensity, float range);
};

// SpotLight holds data use to compute spot light
class SpotLight
	: public PointLight
{
public:
	glm::vec3 m_direction;
	float m_angle{ 0 };
	float m_cos{ 0 };

	SpotLight() = default;
	SpotLight(glm::vec3 position, glm::vec3 colour, float intensity, glm::vec3 direction, float angle, float range);
	void Set(glm::vec3 position, glm::vec3 colour, float intensity, glm::vec3 direction, float angle, float range);
};
