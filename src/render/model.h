﻿#pragma once

#include "mesh.h"
#include "xmaterial.h"

#include <memory>

class Model
{
public:
	std::shared_ptr<XMaterial> material;

	void SetMesh(std::filesystem::path filePath);

	void Render();

	Model(const std::shared_ptr<XMaterial>& mater, const std::filesystem::path filePath);

	Model();
	~Model();

private:
	GPUMesh m_mesh;
	Texture m_texture;
	ShaderBuilder m_shaderBuilder;
};