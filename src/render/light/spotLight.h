#pragma once

#include "pointLight.h"

class SpotLight : public PointLight 
{
public:
	SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float intensity, float constant, float linear, float quadratic, float innerCutoff, float outerCutoff);

	const glm::vec3& getDirection() const;

	void setDirection(const glm::vec3& direction);

	float getInnerCutoff() const;

	void setInnerCutoff(float innerCutoff);

	float getOuterCutoff() const;

	void setOuterCutoff(float outerCutoff);

private:
	glm::vec3 direction;
	float innerCutoff;
	float outerCutoff;
};

