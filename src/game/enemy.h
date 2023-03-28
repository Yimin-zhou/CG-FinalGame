#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "render/model.h"

class Enemy 
{
public:
	std::shared_ptr<Model> model;
	glm::vec3 position;
	float speed;
	float health;
	bool isDead = false;

	Enemy(const glm::vec3& position, float speed, float health);

	void Update(float deltaTime, const glm::vec3& playerPosition);

	void TakeDamage(float damage);

	bool IsAlive() const;

	glm::vec3 GetPosition() const;

private:

};
