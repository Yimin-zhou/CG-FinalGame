#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "render/model.h"

class Boss
{
public:
	std::shared_ptr<Model> model;
	glm::vec3 position;
	float speed;
	uint32_t health = 100;
	bool isDead = false;
	//std::vector<std::shared_ptr<ObjectNode>> tempObjects;

	Boss(const glm::vec3& position, float speed, uint32_t health);

	void Update(float deltaTime, const glm::vec3& playerPosition);

	void TakeDamage(float damage);

	bool IsAlive() const;

	bool CheckCollision(glm::vec3 projectilePos);

	uint32_t GetHealth() const;

	glm::vec3 GetPosition() const;

private:

};
