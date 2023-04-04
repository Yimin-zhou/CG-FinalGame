#include "enemy.h"

Enemy::Enemy(const glm::vec3& pos, float sp, uint32_t hp) :
	position(pos), 
	speed(sp), 
	health(hp),
	isDead(false),
	model(),
	yaw(0.0f),
	collider(pos, 0.9f)
{

}

void Enemy::Update(float deltaTime, const glm::vec3& playerPosition)
{
	// Add AI logic here, e.g. follow the player, patrol, attack, etc.
	// Simple follow player logic:
	glm::vec3 direction = playerPosition - position;
	direction = direction == glm::vec3(0)? direction:glm::normalize(direction);
	position += direction * speed * deltaTime;
	
	collider.SetPosition(position);
	FacePlayer(playerPosition);
}

void Enemy::FacePlayer(const glm::vec3& playerPosition) 
{
	glm::vec3 direction = glm::normalize(playerPosition - position);
	float newYaw = glm::degrees(atan2(direction.x, direction.z));
	yaw = newYaw;
}

void Enemy::TakeDamage(float damage)
{
	health -= damage;
	if (health <= 0) 
	{
		isDead = true;
	}
}

bool Enemy::IsAlive() const
{
	return !isDead;
}

bool Enemy::CheckCollision(const Collider& other) const
{
	return collider.CheckCollision(other);
}

Collider Enemy::GetCollider() const
{
	return collider;
}

uint32_t Enemy::GetHealth() const
{
	return health;
}

glm::vec3 Enemy::GetPosition() const
{
	return position;
}

void Enemy::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

float Enemy::GetYaw() const
{
	return yaw;
}

glm::mat4 Enemy::GetModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	return model;
}


