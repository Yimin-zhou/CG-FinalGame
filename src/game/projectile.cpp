#include "projectile.h"

Projectile::Projectile(const glm::vec3& position, const glm::vec3& direction, float speed, uint32_t damage, std::shared_ptr<Model> mod) :
	position(position),
	direction(direction),
	speed(speed),
	damage(damage),
	m_timeToLive(5.0f)
{
	model = mod;
}

Projectile::Projectile()
{

}

bool Projectile::Update(float deltaTime)
{
	position += direction * speed * deltaTime;
	m_timeToLive -= deltaTime;
	return (m_timeToLive <= 0.0f);
}

void Projectile::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

void Projectile::SetDirection(const glm::vec3& dir)
{
	direction = dir;
}

void Projectile::SetSpeed(float sp)
{
	speed = sp;
}

void Projectile::SetDamage(uint32_t da)
{
	damage = da;
}

glm::vec3 Projectile::GetPosition() const
{
	return position;
}

float Projectile::GetDamage() const
{
	return damage;
}

