#pragma once
#include <glad/glad.h>
#include <framework/shader.h>
#include "render/texture.h"

#include <memory>

class ParticleMaterial
{
public:
	ParticleMaterial(std::shared_ptr<Texture> texture);

	std::shared_ptr<Texture> getTexture() const;
	GLenum getSrcBlendFactor() const;
	GLenum getDstBlendFactor() const;

private:
	std::shared_ptr<Texture> m_texture;
	GLenum m_srcBlendFactor;
	GLenum m_dstBlendFactor;
};
