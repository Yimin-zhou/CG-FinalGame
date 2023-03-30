#include "xmaterial.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

XMaterial::~XMaterial()
{

}

glm::mat4 XMaterial::GetModelMatrix()
{
	return m_modelMat;
}

glm::mat4 XMaterial::GetViewMatrix()
{
	return m_viewMat;
}

glm::mat4 XMaterial::GetProjMatrix()
{
	return m_projMat;
}

void XMaterial::SetUniform(const std::string& name, float value)
{
	m_shader.SetUniform(name, value);
}

void XMaterial::SetUniform(const std::string& name, int value)
{
	m_shader.SetUniform(name, value);
}

void XMaterial::SetUniform(const std::string& name, const glm::vec3& value)
{
	m_shader.SetUniform(name, value);
}

void XMaterial::SetUniform(const std::string& name, const glm::vec4& value)
{
	m_shader.SetUniform(name, value);
}

void XMaterial::SetUniform(const std::string& name, const glm::mat3& value)
{
	m_shader.SetUniform(name, value);
}

void XMaterial::SetUniform(const std::string& name, const glm::mat4& value)
{
	m_shader.SetUniform(name, value);
}

void XMaterial::SetAlbedo(std::filesystem::path textureFilePath, bool shouldFlip)
{
	m_albedo = std::make_shared<Texture>(textureFilePath, shouldFlip);
}

void XMaterial::SetAlbedo(Texture* map)
{
	m_albedo = std::shared_ptr<Texture>(map);
}

void XMaterial::SetRma(std::filesystem::path textureFilePath, bool shouldFlip)
{
	m_rma = std::make_shared<Texture>(textureFilePath, shouldFlip);
}

void XMaterial::SetRma(Texture* map)
{
	m_rma = std::shared_ptr<Texture>(map);
}

void XMaterial::SetShader(std::filesystem::path vertFilePath, std::filesystem::path fragFilePath)
{
	ShaderBuilder builder;
	builder.addStage(GL_VERTEX_SHADER, vertFilePath);
	builder.addStage(GL_FRAGMENT_SHADER, fragFilePath);
	m_shader = builder.build();
}

void XMaterial::SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj)
{
	m_modelMat = model;
	m_normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMat));
	m_viewMat = view;
	m_projMat = proj;
}

void XMaterial::SetMatrix(const glm::mat4& view, const glm::mat4& proj)
{
	m_viewMat = view;
	m_projMat = proj;
}

void XMaterial::SetNormalEm(std::filesystem::path textureFilePath, bool shouldFlip)
{
	m_normalEm = std::make_shared<Texture>(textureFilePath, shouldFlip);
}

void XMaterial::SetNormalEm(Texture* map)
{
	m_normalEm = std::shared_ptr<Texture>(map);
}

void XMaterial::Apply() 
{
	m_shader.bind();

	const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMat));

	SetUniform("model", m_modelMat);
	SetUniform("view", m_viewMat);
	SetUniform("projection", m_projMat);
	SetUniform("normalModelMatrix", normalModelMatrix);
	if (m_albedo != nullptr)
	{
		m_albedo->bind(GL_TEXTURE0);
		SetUniform("albedoMap", 0);
	}
	if(m_rma != nullptr) 
	{
		m_rma->bind(GL_TEXTURE1);
		SetUniform("rmaMap", 1);
	}
	if(m_normalEm != nullptr)
	{
		m_normalEm->bind(GL_TEXTURE2);
		SetUniform("normalEmMap", 2);
	}
}

XMaterial::XMaterial() :
	m_shader(),
	m_albedo(),
	m_modelMat({ 1.0f }),
	m_viewMat(glm::lookAt(glm::vec3(-1, 1, -1), glm::vec3(0), glm::vec3(0, 1, 0))),
	m_projMat(glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 3000.0f))
{
	m_normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMat));
	// default shader & texture
	ShaderBuilder defaultBuilder;
	defaultBuilder.addStage(GL_VERTEX_SHADER,"shaders/shader_vert.glsl");
	defaultBuilder.addStage(GL_FRAGMENT_SHADER,"shaders/shader_frag.glsl");
	m_shader = defaultBuilder.build();

	m_albedo = std::make_shared<Texture>("resources/default.png", true);
}

XMaterial::XMaterial(glm::mat4 m_modelMat, glm::mat4 m_viewMat, glm::mat4 m_projMat) :
	m_modelMat(m_modelMat),
	m_viewMat(m_viewMat),
	m_projMat(m_projMat)
{
	m_normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMat));
	// default shader & texture
	ShaderBuilder defaultBuilder;
	defaultBuilder.addStage(GL_VERTEX_SHADER,"shaders/shader_vert.glsl");
	defaultBuilder.addStage(GL_FRAGMENT_SHADER,"shaders/shader_frag.glsl");
	m_shader = defaultBuilder.build();

	m_albedo = std::make_shared<Texture>("resources/default.png");
}
