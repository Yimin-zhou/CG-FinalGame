#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render/xmaterial.h"

#include <memory>

#include "render/model.h"

class AnimatedModel
{
public:
	std::vector<std::shared_ptr<Model>> m_frames;
	std::shared_ptr<XMaterial> material;

	void Update(float deltaTime);
	void Render();

	AnimatedModel(const std::shared_ptr<XMaterial>& material, const std::vector<std::string>& framePaths);

	AnimatedModel();
	~AnimatedModel();
	
private:
	
	int currentFrame;
	float frameDuration;
	float elapsedTime;
};
