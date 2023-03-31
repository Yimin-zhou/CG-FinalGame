#include "particle.h"

Particle::Particle(std::shared_ptr<ParticleMesh> mesh,
	std::shared_ptr<ParticleMaterial> material, float scale,
	const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, float life) :
	m_mesh(mesh),
	m_material(material),
	m_scale(scale),
	m_position(position),
	m_velocity(velocity),
	m_color(color),
	m_life (life)
{
	// Initialize the model matrix based on the initial position and scale
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position) 
		* glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
}

void Particle::update(float deltaTime)
{
	if (m_life > 0.0f) 
	{
		m_position += m_velocity * deltaTime;
		m_life -= deltaTime;
	}

	// Update the model matrix based on the updated position and scale
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
}

bool Particle::isAlive() const
{
	return m_life > 0.0f;
}

const glm::mat4& Particle::modelMatrix() const
{
	return m_modelMatrix;
}

const glm::vec4& Particle::color() const
{
	return m_color;
}
