#pragma once

#include "light.h"

class DirectionalLight : public Light 
{
public:
	DirectionalLight(const glm::vec3& location, const glm::vec3& color, float intensity);

	const glm::vec3& getDirection() const;
	const glm::vec3& getPsistion() const;

	void setDirection(const glm::vec3& direction);

private:
	glm::vec3 direction;
};
