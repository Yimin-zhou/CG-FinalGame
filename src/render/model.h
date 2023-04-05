#pragma once

#include "mesh.h"
#include "xmaterial.h"
#include "light/light.h"
#include "light/directionalLight.h"
#include "light/pointLight.h"
#include "light/spotLight.h"


#include <memory>

class Model
{
public:
	std::shared_ptr<XMaterial> material;
	GPUMesh mesh;

	void InitTextures();
	void SetMesh(std::filesystem::path filePath);

	void Render(std::shared_ptr<DirectionalLight> dirLight, 
		std::vector<std::shared_ptr<PointLight>>& pointLights,
		std::vector<std::shared_ptr<SpotLight>>& spotLights,
		const glm::vec3& camPos);

	void RnderUI(int health);

	Model(const std::shared_ptr<XMaterial>& mater, const std::filesystem::path filePath);

	Model();
	~Model();

private:
	Texture m_texture;
	ShaderBuilder m_shaderBuilder;
};