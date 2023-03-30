
#include "pointLight.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float intensity, float constant, float linear, float quadratic) : 
	Light(position, color, intensity), 
	constant(constant), linear(linear), 
	quadratic(quadratic)
{

}

float PointLight::getConstant() const
{
	return constant;
}

void PointLight::setConstant(float constant)
{
	this->constant = constant;
}

float PointLight::getLinear() const
{
	return linear;
}

void PointLight::setLinear(float linear)
{
	this->linear = linear;
}

float PointLight::getQuadratic() const
{
	return quadratic;
}

void PointLight::setQuadratic(float quadratic)
{
	this->quadratic = quadratic;
}

