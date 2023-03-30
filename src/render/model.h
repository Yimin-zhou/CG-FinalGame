#pragma once

#include "mesh.h"
#include "xmaterial.h"

#include <memory>

class Model
{
public:
	std::shared_ptr<XMaterial> material;
	GPUMesh mesh;

	void InitTextures();
	void SetMesh(std::filesystem::path filePath);

	void Render(glm::vec3 lightPos, glm::vec3 camPos);

	Model(const std::shared_ptr<XMaterial>& mater, const std::filesystem::path filePath);

	Model();
	~Model();

private:
	Texture m_texture;
	ShaderBuilder m_shaderBuilder;
};