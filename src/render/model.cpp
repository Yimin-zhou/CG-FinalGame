#include "model.h"

Model::~Model()
{

}

void Model::SetMesh(std::string filePath)
{
	m_mesh = GPUMesh(filePath);
}



void Model::Render()
{
	material->Apply();
	m_mesh.draw();
}

Model::Model() :
	m_mesh(),	
	material()
{
	// TODO: read material frame obj
	m_mesh = GPUMesh("resources/cube-textured.obj");
	material = std::make_shared<XMaterial>();
}

Model::Model(const std::shared_ptr<XMaterial>& mater) :
	m_mesh(),
	material(mater)
{
	// TODO: read material frame obj
	m_mesh = GPUMesh("resources/cube-textured.obj");
}
