#include "spotLight.h"

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, 
	const glm::vec3& color, float intensity, 
	float constant, float linear, float quadratic, float innerCutoff, float outerCutoff) : 
	PointLight(position, color, intensity, constant, linear, quadratic), 
	direction(direction), innerCutoff(innerCutoff), outerCutoff(outerCutoff)
{

}

const glm::vec3& SpotLight::getDirection() const
{
	return direction;
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	this->direction = this->direction = direction;
}

float SpotLight::getInnerCutoff() const
{
	return innerCutoff;
}

void SpotLight::setInnerCutoff(float innerCutoff)
{
	this->innerCutoff = innerCutoff;
}

float SpotLight::getOuterCutoff() const
{
	return outerCutoff;
}

void SpotLight::setOuterCutoff(float outerCutoff)
{
	this->outerCutoff = outerCutoff;
}

