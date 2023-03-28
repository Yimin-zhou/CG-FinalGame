#include "enemy.h"

Enemy::Enemy(const glm::vec3& pos, float sp, uint32_t hp) :
	position(pos), 
	speed(sp), 
	health(hp),
	isDead(false),
	model()
{

}

void Enemy::Update(float deltaTime, const glm::vec3& playerPosition)
{
	// Add AI logic here, e.g. follow the player, patrol, attack, etc.
	// Simple follow player logic:
	glm::vec3 direction = playerPosition - position;
	direction = direction == glm::vec3(0)? direction:glm::normalize(direction);
	position += direction * speed * deltaTime;
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

bool Enemy::CheckCollision(glm::vec3 projectilePos)
{
	glm::vec3 delta = projectilePos - position;
	float distance = glm::length(delta);
	float radiusSum = 2.0f + 1.0f;

	return distance < radiusSum;
}

uint32_t Enemy::GetHealth() const
{
	return health;
}

glm::vec3 Enemy::GetPosition() const
{
	return position;
}

