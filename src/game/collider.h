#pragma once

#include <glm/glm.hpp>

class Collider
{
public:
    Collider(const glm::vec3& pos, float radius);
    Collider();

    bool CheckCollision(const Collider& other) const;

    void SetPosition(const glm::vec3& pos);
    const glm::vec3& GetPosition() const;

    void SetRadius(float radius);
    float GetRadius() const;

private:
    glm::vec3 position;
    float radius;
};
