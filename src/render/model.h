#pragma once

#include "mesh.h"
#include "xmaterial.h"

#include <memory>

class Model
{
public:
	std::shared_ptr<XMaterial> material;

	void SetMesh(std::filesystem::path filePath);

	void Render();

	Model(const std::shared_ptr<XMaterial>& material, const std::filesystem::path filePath);

	Model();
	~Model();

private:
	GPUMesh m_mesh;
};