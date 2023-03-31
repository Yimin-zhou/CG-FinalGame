#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

#include "particle.h"

#include <vector>
#include <glm/glm.hpp>

class ParticleSystem 
{
public:
	struct InstanceData 
	{
		glm::mat4 modelMatrix;
		glm::vec4 color;
	};

	ParticleSystem(std::shared_ptr<ParticleMesh> mesh,
		std::shared_ptr<ParticleMaterial> material, uint32_t maxParticless);

	void bind();

	// get material
	std::shared_ptr<ParticleMaterial> getMaterial() const;

	void update(float deltaTime);

	void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Shader shader);

private:
	std::shared_ptr<ParticleMesh> m_mesh;
	std::shared_ptr<ParticleMaterial> m_material;
	unsigned int m_maxParticles;
	std::vector<std::shared_ptr<Particle>> m_particles;

	// instancing
	std::vector<InstanceData> m_instanceData;
	GLuint m_instanceBuffer;
};

