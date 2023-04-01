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

void Model::Render(std::shared_ptr<DirectionalLight> dirLight, 
	std::vector<std::shared_ptr<PointLight>>& pointLights,
	std::vector<std::shared_ptr<SpotLight>>& spotLights,
	const glm::vec3& camPos)
{
	material->SetUniform("roughnessMultiplier", 1.0f);
	material->SetUniform("baseColor", glm::vec3(1));
	material->SetUniform("lightColor", glm::vec3(0.2, 0.2, 1.0));
	//material->SetUniform("emissiveColor", glm::vec3(1.0, 0.2, 0.2));
	material->SetUniform("cameraPos", camPos);

	// set up directional light
	material->SetUniform("directionalLight.direction", dirLight->getDirection());
	material->SetUniform("directionalLight.color", dirLight->getColor());
	material->SetUniform("directionalLight.intensity", dirLight->getIntensity());

	// set up point light
	uint16_t index = 0;
	for(auto& light : pointLights)
	{
		std::string prefix = "pointLights[" + std::to_string(index) + "].";
		material->SetUniform(prefix + "position", light->getPosition());
		material->SetUniform(prefix + "color", light->getColor());
		material->SetUniform(prefix + "intensity", light->getIntensity());
		material->SetUniform(prefix + "constant", light->getConstant());
		material->SetUniform(prefix + "linear", light->getLinear());
		material->SetUniform(prefix + "quadratic", light->getQuadratic());
		++index;
	}

	// set up spot light
	index = 0;
	for (auto& light : spotLights)
	{
		std::string prefix = "spotLights[" + std::to_string(index) + "].";
		material->SetUniform(prefix + "position", light->getPosition());
		material->SetUniform(prefix + "direction", light->getDirection());
		material->SetUniform(prefix + "color", light->getColor());
		material->SetUniform(prefix + "intensity", light->getIntensity());
		material->SetUniform(prefix + "constant", light->getConstant());
		material->SetUniform(prefix + "linear", light->getLinear());
		material->SetUniform(prefix + "quadratic", light->getQuadratic());
		material->SetUniform(prefix + "innerCutoff", light->getInnerCutoff());
		material->SetUniform(prefix + "outerCutoff", light->getOuterCutoff());
		++index;
	}

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
