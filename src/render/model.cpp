#include "model.h"

Model::~Model()
{

}

void Model::SetMesh(std::filesystem::path filePath)
{
	m_mesh = GPUMesh(filePath);
}

void Model::Render()
{
	material->Apply();
	m_mesh.draw();
}

Model::Model()
{

}

Model::Model(const std::shared_ptr<XMaterial>& mater, const std::filesystem::path filePath) :
	m_mesh(filePath),
	material(mater),
	m_shaderBuilder()
{

}
