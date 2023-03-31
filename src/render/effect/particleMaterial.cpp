#include "particleMaterial.h"

ParticleMaterial::ParticleMaterial(std::shared_ptr<Texture> texture)
{
	m_texture = texture;
	m_srcBlendFactor = GL_SRC_ALPHA;
	m_dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
}

std::shared_ptr<Texture>  ParticleMaterial::getTexture() const
{
	return m_texture;
}

GLenum ParticleMaterial::getSrcBlendFactor() const
{
	return m_srcBlendFactor;
}

GLenum ParticleMaterial::getDstBlendFactor() const
{
	return m_dstBlendFactor;
}
