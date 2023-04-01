#include "light.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity) :
	position(position),
	color(color),
	intensity(intensity)
{

}

const glm::vec3& Light::getPosition() const
{
	return position;
}

void Light::setPosition(const glm::vec3& position)
{
	this->position = position;
}

const glm::vec3& Light::getColor() const
{
	return color;
}

void Light::setColor(const glm::vec3& color)
{
	this->color = color;
}

float Light::getIntensity() const
{
	return intensity;
}

void Light::setIntensity(float intensity)
{
	this->intensity = intensity;
}
