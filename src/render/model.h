#pragma once

#include "mesh.h"
#include "xmaterial.h"

#include <memory>

class Model
{
public:
	std::shared_ptr<XMaterial> material;

	void SetMesh(std::string filePath);

	void Render();

	Model(const std::shared_ptr<XMaterial>& material);

	Model();
	~Model();

private:
	GPUMesh m_mesh;
};