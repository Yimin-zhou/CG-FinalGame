#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render/xmaterial.h"

#include <memory>

#include "render/model.h"

class AnimatedModel
{
public:
	std::vector<std::shared_ptr<Model>> frames;
	std::shared_ptr<XMaterial> material;

	void Update(float deltaTime);
	void Render(std::shared_ptr<DirectionalLight> dirLight,
		std::vector<std::shared_ptr<PointLight>>& pointLights,
		std::vector<std::shared_ptr<SpotLight>>& spotLights,
		const glm::vec3& camPos);

	AnimatedModel(const std::shared_ptr<XMaterial>& material, const std::vector<std::string>& framePaths);

	AnimatedModel();
	~AnimatedModel();
	
private:
	
	int m_currentFrame;
	float m_frameDuration;
	float m_elapsedTime;
};
