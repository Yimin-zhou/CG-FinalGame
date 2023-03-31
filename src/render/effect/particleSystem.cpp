#include "particleSystem.h"

#include <random>

ParticleSystem::ParticleSystem(std::shared_ptr<ParticleMesh> mesh,
	std::shared_ptr<ParticleMaterial> material, uint32_t maxParticles) :
	m_mesh(mesh), 
	m_material(material), 
	m_maxParticles(maxParticles), 
	m_particles(maxParticles),
	m_instanceData(maxParticles)
{
	glGenBuffers(1, &m_instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::mt19937 randomEngine(std::random_device{}());
	std::uniform_real_distribution<float> positionDistribution(-10.0f, 10.0f);
	std::uniform_real_distribution<float> velocityDistribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> colorDistribution(0.1f, 1.0f);
	std::uniform_real_distribution<float> lifeDistribution(1.0f, 5.0f);
	std::uniform_real_distribution<float> scaleDistribution(0.5f, 2.0f);
	// Initialize the particles
	for (unsigned int i = 0; i < m_maxParticles; ++i) 
	{
		glm::vec3 position(positionDistribution(randomEngine), positionDistribution(randomEngine), positionDistribution(randomEngine));
		glm::vec3 velocity(velocityDistribution(randomEngine), velocityDistribution(randomEngine), velocityDistribution(randomEngine));
		glm::vec4 color(colorDistribution(randomEngine), colorDistribution(randomEngine), colorDistribution(randomEngine), 1.0f);
		float life = lifeDistribution(randomEngine);
		float scale = scaleDistribution(randomEngine);

		m_particles.push_back(std::make_shared<Particle>(m_mesh, m_material, scale, position, velocity, color, life));
	}

}

void ParticleSystem::bind()
{
	// Bind the particle mesh
	glBindVertexArray(m_mesh->getVAO());
}

std::shared_ptr<ParticleMaterial> ParticleSystem::getMaterial() const
{
	return m_material;
}

void ParticleSystem::update(float deltaTime)
{
	for (auto& particle : m_particles) {
		particle->update(deltaTime);
	}
}

void ParticleSystem::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Shader shader)
{
	shader.bind();

	shader.SetUniform("view", viewMatrix);
	shader.SetUniform("projection", projectionMatrix);

	// Bind the particle mesh
	ParticleSystem::bind();

	// Bind the particle material properties
	m_material->getTexture()->bind(0);
	glBlendFunc(m_material->getSrcBlendFactor(), m_material->getDstBlendFactor());

	// Update instance data
	unsigned int instanceCount = 0;
	for (unsigned int i = 0; i < m_maxParticles; ++i) 
	{
		auto& particle = m_particles[i];
		if (particle->isAlive()) {
			m_instanceData[instanceCount].modelMatrix = particle->modelMatrix();
			m_instanceData[instanceCount].color = particle->color();
			++instanceCount;
		}
	}

	// Update the instance buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(InstanceData), m_instanceData.data());

	// Set up instance attributes
	GLsizei vec4Size = sizeof(glm::vec4);
	GLsizei mat4Size = 4 * vec4Size;
	for (int i = 0; i < 4; ++i) {
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)(i * vec4Size));
		glEnableVertexAttribArray(3 + i);
		glVertexAttribDivisor(3 + i, 1);
	}

	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(4 * vec4Size));
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Draw instanced particles
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh->getIndicesCount(), GL_UNSIGNED_INT, 0, instanceCount);

	// Reset the instance buffer
	for (int i = 3; i < 8; ++i) {
		glVertexAttribDivisor(i, 0);
	}
}
