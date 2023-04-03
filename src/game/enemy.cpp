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
	float distance = glm::distance(playerPosition, position);
	if (distance > 1.0f) {
		position += direction * speed * deltaTime;
	}
	FacePlayer(playerPosition);
}

void Enemy::FacePlayer(const glm::vec3& playerPosition)
{
	glm::vec3 direction = glm::normalize(playerPosition - position);
	float newYaw = glm::degrees(atan2(direction.x, direction.z));
	yaw = newYaw;
	UpdateEnemyVectors();
}

void Enemy::UpdateEnemyVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw));
	newFront.y = 0.0f;
	newFront.z = sin(glm::radians(yaw));
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

float Enemy::GetYaw() const
{
	return yaw;
}

glm::mat4 Enemy::GetModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	return model;
}


