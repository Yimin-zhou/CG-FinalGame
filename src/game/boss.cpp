#include "boss.h"

Boss::Boss(const glm::vec3& pos, float sp, uint32_t hp) :
	position(pos),
	speed(sp),
	health(hp),
	isDead(false),
	model()
{

}

void Boss::Update(float deltaTime, const glm::vec3& playerPosition)
{
	// Add AI logic here, e.g. follow the player, patrol, attack, etc.
	// Simple follow player logic:
	glm::vec3 direction = playerPosition - position;
	direction = direction == glm::vec3(0) ? direction : glm::normalize(direction);
	
	float distance = glm::distance(playerPosition, position);
	if (distance > 2.0f) {
		position += direction * speed * deltaTime;
	}
	FacePlayer(playerPosition);
}

void Boss::FacePlayer(const glm::vec3& playerPosition) {
	glm::vec3 direction = glm::normalize(playerPosition - position);
	float newYaw = glm::degrees(atan2(direction.x, direction.z));
	yaw = newYaw;
	UpdateBossVectors();
}

void Boss::UpdateBossVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw));
	newFront.y = 0.0f;
	newFront.z = sin(glm::radians(yaw));
}


void Boss::TakeDamage(float damage)
{
	health -= damage;
	if (health <= 0)
	{
		isDead = true;
	}
}

bool Boss::IsAlive() const
{
	return !isDead;
}

bool Boss::CheckCollision(glm::vec3 projectilePos)
{
	glm::vec3 delta = projectilePos - position;
	float distance = glm::length(delta);
	float radiusSum = 2.0f + 1.0f;

	return distance < radiusSum;
}

uint32_t Boss::GetHealth() const
{
	return health;
}

glm::vec3 Boss::GetPosition() const
{
	return position;
}

float Boss::GetYaw() const
{
	return yaw;
}

glm::mat4 Boss::GetModelMatrix() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	return model;
}


