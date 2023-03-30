#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

class Light 
{
public:
	Light(const glm::vec3& position, const glm::vec3& color, float intensity);

	virtual ~Light() = default;

	const glm::vec3& getPosition() const;

	void setPosition(const glm::vec3& position);

	const glm::vec3& getColor() const;

	void setColor(const glm::vec3& color);

	float getIntensity() const;

	void setIntensity(float intensity);

private:
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};
