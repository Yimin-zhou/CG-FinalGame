#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity) :
	Light(glm::vec3(0.0f), color, intensity), // Directional light has no position
	direction(direction)
{

}

const glm::vec3& DirectionalLight::getDirection() const
{
	return direction;
}

void DirectionalLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}
