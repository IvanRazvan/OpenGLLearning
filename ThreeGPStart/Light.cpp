#include "Light.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 colour, float intensity)
{
	m_direction = direction;
	m_colour = colour;
	m_intensity = intensity;
}

void DirectionalLight::Set(glm::vec3 direction, glm::vec3 colour, float intensity)
{
	m_direction = direction;
	m_colour = colour;
	m_intensity = intensity;
}

PointLight::PointLight(glm::vec3 position, glm::vec3 colour, float intensity, float range)
{
	m_position = position;
	m_colour = colour;
	m_intensity = intensity;
	m_range = range;
}

void PointLight::Set(glm::vec3 position, glm::vec3 colour, float intensity, float range)
{
	m_position = position;
	m_colour = colour;
	m_intensity = intensity;
	m_range = range;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 colour, float intensity, glm::vec3 direction, float angle, float range)
{
	m_position = position;
	m_colour = colour;
	m_intensity = intensity;
	m_direction = direction;
	m_angle = angle;
	m_cos = cos(glm::radians(angle));
	m_range = range;
}

void SpotLight::Set(glm::vec3 position, glm::vec3 colour, float intensity, glm::vec3 direction, float angle, float range)
{
	m_position = position;
	m_colour = colour;
	m_intensity = intensity;
	m_direction = direction;
	m_angle = angle;
	m_cos = cos(glm::radians(angle));
	m_range = range;
}
