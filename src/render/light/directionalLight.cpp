#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& location, const glm::vec3& color, float intensity) :
	Light(location, color, intensity), // Directional light has no position
	direction(location)
{

}

const glm::vec3& DirectionalLight::getDirection() const
{
	return direction;
}

const glm::vec3& DirectionalLight::getPsistion() const
{
	return direction;
}

void DirectionalLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}
