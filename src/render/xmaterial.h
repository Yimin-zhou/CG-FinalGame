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

	void SetShader(std::filesystem::path vertFilePath, std::filesystem::path fragFilePath);
	void SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
	void SetMatrix(const glm::mat4& view, const glm::mat4& proj);

	//void SetUniform(const std::string& name, float value);
	//void SetUniform(const std::string& name, int value);
	//void SetUniform(const std::string& name, const glm::vec3& value);
	//void SetUniform(const std::string& name, const glm::vec4& value);
	//void SetUniform(const std::string& name, const glm::mat4& value);

	// set textures
	void SetAlbedo(std::filesystem::path textureFilePath, bool shouldFlip);

	void Apply();

	XMaterial(glm::mat4 m_modelMat, glm::mat4 m_viewMat, glm::mat4 m_projMat);
	XMaterial();
	~XMaterial();

private:
	// textures
	std::shared_ptr<Texture> m_albedo;

	Shader m_shader;
	glm::mat4 m_modelMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;
};
