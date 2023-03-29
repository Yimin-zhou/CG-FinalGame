#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "render/model.h"

class Projectile 
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	float speed;
	uint32_t damage;
	std::shared_ptr<Model> model;

	Projectile(const glm::vec3& position, const glm::vec3& direction, float speed, uint32_t damage, std::shared_ptr<Model> mod);
	Projectile();

	bool Update(float deltaTime);

	void SetPosition(const glm::vec3& position);
	void SetDirection(const glm::vec3& direction);
	void SetSpeed(float speed);
	void SetDamage(uint32_t damage);

	glm::vec3 GetPosition() const;
	float GetDamage() const;

private:
	float m_timeToLive;
};
