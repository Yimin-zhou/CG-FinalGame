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

void XMaterial::SetTexture(const Texture& texture)
{

}

void XMaterial::SetShader(const Shader& shader)
{

}

void XMaterial::SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj)
{
	m_modelMat = model;
	m_viewMat = view;
	m_projMat = proj;
}

void XMaterial::SetMatrix(const glm::mat4& view, const glm::mat4& proj)
{
	m_viewMat = view;
	m_projMat = proj;
}

void XMaterial::Apply() 
{
	m_shader.bind();

	const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMat));

	m_shader.SetUniform("model", m_modelMat);
	m_shader.SetUniform("view", m_viewMat);
	m_shader.SetUniform("projection", m_projMat);
	m_texture->bind(GL_TEXTURE0);
	m_shader.SetUniform("diffuseMap", 0);
}

XMaterial::XMaterial() :
	m_shader(),
	m_texture(),
	m_modelMat({ 1.0f }),
	m_viewMat(glm::lookAt(glm::vec3(-1, 1, -1), glm::vec3(0), glm::vec3(0, 1, 0))),
	m_projMat(glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 3000.0f))
{
	// TODO: replace default shader & texture
	ShaderBuilder defaultBuilder;
	defaultBuilder.addStage(GL_VERTEX_SHADER,"shaders/shader_vert.glsl");
	defaultBuilder.addStage(GL_FRAGMENT_SHADER,"shaders/shader_frag.glsl");
	m_shader = defaultBuilder.build();

	m_texture = std::make_shared<Texture>("resources/default.png");
}

XMaterial::XMaterial(glm::mat4 m_modelMat, glm::mat4 m_viewMat, glm::mat4 m_projMat) :
	m_modelMat(m_modelMat),
	m_viewMat(m_viewMat),
	m_projMat(m_projMat)
{
	// TODO: replace default shader & texture
	ShaderBuilder defaultBuilder;
	defaultBuilder.addStage(GL_VERTEX_SHADER,"shaders/shader_vert.glsl");
	defaultBuilder.addStage(GL_FRAGMENT_SHADER,"shaders/shader_frag.glsl");
	m_shader = defaultBuilder.build();

	m_texture = std::make_shared<Texture>("resources/default.png");
}
