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
	// TODO: read material frame obj
	//m_mesh = GPUMesh("resources/cube-textured.obj");
	//material = std::make_shared<XMaterial>();
}

Model::Model(const std::shared_ptr<XMaterial>& mater, const std::filesystem::path filePath) :
	m_mesh(filePath),
	material(mater)
{
	// TODO: read material frame obj
}
