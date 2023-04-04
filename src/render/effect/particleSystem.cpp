#include "particleSystem.h"
#include "helper/random.h"

ParticleSystem::ParticleSystem()
{
	m_particlePool.resize(1000); // instantiate particles
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Update(float deltaTime)
{
	for (auto& particle : m_particlePool)
	{
		if (!particle.active)
			continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= deltaTime;
		particle.position += particle.velocity * deltaTime;
		particle.rotation += 0.01f * deltaTime;
	}
}

void ParticleSystem::Render()
{
	// Enable blending
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!m_quadVA)
	{
		float vertices[] = 
		{
			// Positions        // UVs
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		glCreateVertexArrays(1, &m_quadVA);
		glBindVertexArray(m_quadVA);

		GLuint quadVB, quadIB;
		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	m_shader.bind();
	m_shader.SetUniform("view", m_viewMatrix);
	m_shader.SetUniform("projection", m_projectionMatrix);

	for (auto& particle : m_particlePool)
	{
		if (!particle.active)
			continue;

		// Fade away particles
		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = glm::mix(particle.colorEnd, particle.colorBegin, life);
		color.a = color.a * life;

		float size = glm::mix(particle.sizeEnd, particle.sizeBegin, life);

		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		m_shader.SetUniform("model", transform);
		m_shader.SetUniform("particalColor", color);
		glBindVertexArray(m_quadVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	// Disable blending
	glDisable(GL_BLEND);
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_particlePool[m_poolIndex];
	particle.active = true;
	particle.position = particleProps.position + Random::Float() * 2;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocityVariation.x * (Random::Float() - 0.5f);
	particle.velocity.y += particleProps.velocityVariation.y * (Random::Float() - 0.5f);
	particle.velocity.z += particleProps.velocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.colorBegin = particleProps.colorBegin;
	particle.colorEnd = particleProps.colorEnd;

	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;
	particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() - 0.5f);
	particle.sizeEnd = particleProps.sizeEnd;

	m_poolIndex = --m_poolIndex % m_particlePool.size();
}

void ParticleSystem::SetShader(Shader& shader)
{
	m_shader = shader;
}

void ParticleSystem::SetViewMatrix(glm::mat4& viewMatrix)
{
	m_viewMatrix = viewMatrix;
}

void ParticleSystem::SetProjectionMatrix(glm::mat4& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
}
