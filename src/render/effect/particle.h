#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include "particleMesh.h"
#include "particleMaterial.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Particle 
{
public:
	Particle(std::shared_ptr<ParticleMesh> mesh,
		std::shared_ptr<ParticleMaterial> material, float scale,
		const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, float life);

	void update(float deltaTime);

	bool isAlive() const;
	const glm::mat4& modelMatrix() const;
	const glm::vec4& color() const;

private:
	std::shared_ptr<ParticleMesh>  m_mesh;
	std::shared_ptr<ParticleMaterial>  m_material;
	float m_scale;
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec4 m_color;
	float m_life;
	glm::mat4 m_modelMatrix;
};


