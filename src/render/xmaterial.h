#pragma once

#include <framework/shader.h>
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class XMaterial
{
public:
	glm::mat4 GetModelMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjMatrix();

	void SetTexture(const Texture& texture);
	void SetShader(const Shader& shader);
	void SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
	void SetMatrix(const glm::mat4& view, const glm::mat4& proj);

	void Apply();

	XMaterial(glm::mat4 m_modelMat, glm::mat4 m_viewMat, glm::mat4 m_projMat);
	XMaterial();
	~XMaterial();

private:
	std::shared_ptr<Texture> m_texture;
	Shader m_shader;

	glm::mat4 m_modelMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;
};
