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
	float damage;
	std::shared_ptr<Model> model;

	bool isActive = false;


	Projectile(const glm::vec3& position, const glm::vec3& direction, float speed, float damage);
	Projectile();

	void Update(float deltaTime);

	void SetPosition(const glm::vec3& position);
	void SetDirection(const glm::vec3& direction);

	glm::vec3 GetPosition() const;

	float GetDamage() const;

	void Activate();

private:
};
