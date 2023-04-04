#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>
#include <framework/shader.h>

//Making a PARTICLE SYSTEM in ONE HOUR! (NO ENGINES): https://www.youtube.com/watch?v=GK0jHlv3e3w&ab_channel=TheCherno
struct ParticleProps
{
	glm::vec3 position;
	glm::vec3 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
};


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	void Update(float deltaTime);
	void Render();
	void Emit(const ParticleProps& particleProps);

	void SetShader(Shader& shader);
	void SetModelMatrix(glm::mat4& modelMatrix);
	void SetViewMatrix(glm::mat4& viewMatrix);
	void SetProjectionMatrix(glm::mat4& projectionMatrix);

private:
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float sizeBegin, sizeEnd;
		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;
		bool active = false;
	};

	std::vector<Particle> m_particlePool;
	uint32_t m_poolIndex = 999;

	GLuint m_quadVA = 0;
	Shader m_shader;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};