#include "collider.h"
#include <glm/gtx/norm.hpp>

Collider::Collider(const glm::vec3& pos, float radius) : position(pos), radius(radius)
{
}

Collider::Collider()
{
}

bool Collider::CheckCollision(const Collider& other) const
{
    glm::vec3 delta = other.position - position;
    float distanceSquared = glm::length2(delta);
    float radiusSum = radius + other.radius;

    return distanceSquared < radiusSum * radiusSum;
}

void Collider::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

const glm::vec3& Collider::GetPosition() const
{
    return position;
}

void Collider::SetRadius(float radius)
{
    this->radius = radius;
}

float Collider::GetRadius() const
{
    return radius;
}
