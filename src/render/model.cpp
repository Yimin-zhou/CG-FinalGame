#include "model.h"

Model::~Model()
{

}

void Model::SetMesh(std::filesystem::path filePath)
{
	mesh = GPUMesh(filePath);
}

void Model::Render()
{
	material->Apply();
	mesh.draw();
}

Model::Model()
{

}

Model::Model(const std::shared_ptr<XMaterial>& mater, const std::filesystem::path filePath) :
	mesh(filePath),
	material(mater),
	m_shaderBuilder()
{

}
