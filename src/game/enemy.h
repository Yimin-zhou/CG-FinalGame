#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "render/model.h"
#include "collider.h"

class Enemy 
{
public:
	std::shared_ptr<Model> model;
	glm::vec3 position;
	float speed;
	uint32_t health = 100;
	bool isDead = false;
	float yaw;
	Collider collider;

	Enemy(const glm::vec3& position, float speed, uint32_t health);

	void Update(float deltaTime, const glm::vec3& playerPosition);

	void FacePlayer(const glm::vec3& playerPosition);

	void TakeDamage(float damage);

	bool IsAlive() const;

	bool CheckCollision(const Collider& other) const;

	Collider GetCollider() const;

	uint32_t GetHealth() const;

	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& pos);

	float GetYaw() const;

	glm::mat4 GetModelMatrix() const;
private:

};
