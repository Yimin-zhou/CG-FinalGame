#include "model.h"

Model::~Model()
{

}

void Model::InitTextures()
{
	// set up textures for pbr shader
	if (mesh.kdTexture.has_value() && mesh.rmaTexture.has_value()
		&& mesh.normalEmTexture.has_value())
	{
		Texture* albedo = new Texture(std::move(mesh.kdTexture.value()));
		Texture* rma = new Texture(std::move(mesh.rmaTexture.value())); // roughness, metalness, ambient occlusion
		Texture* normalEm = new Texture(std::move(mesh.normalEmTexture.value())); // normal, emissive

		material->SetAlbedo(albedo);
		material->SetRma(rma);
		material->SetNormalEm(normalEm);
	}
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
	InitTextures();
}
