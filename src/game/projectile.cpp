#include "projectile.h"

Projectile::Projectile(const glm::vec3& position, const glm::vec3& direction, float speed, float damage) : 
	position(position), 
	direction(direction), 
	speed(speed), 
	damage(damage),
	model()
{

}

Projectile::Projectile() :
	position(glm::vec3(0)),
	direction(glm::vec3(0)),
	speed(0),
	damage(0),
	model()
{

}

void Projectile::Update(float deltaTime)
{
	position += direction * speed * deltaTime;
}

void Projectile::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

void Projectile::SetDirection(const glm::vec3& dir)
{
	direction = dir;
}

glm::vec3 Projectile::GetPosition() const
{
	return position;
}

float Projectile::GetDamage() const
{
	return damage;
}

void Projectile::Activate()
{
	isActive = true;
}

