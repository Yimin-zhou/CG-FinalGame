#include "AnimatedModel.h"
#include <iostream>

AnimatedModel::~AnimatedModel()
{

}

void AnimatedModel::Update(float deltaTime)
{
	elapsedTime += deltaTime;
	if (elapsedTime >= frameDuration) {
		elapsedTime -= frameDuration;
		currentFrame = (currentFrame + 1) % m_frames.size();
	}

}

void AnimatedModel::Render(std::shared_ptr<DirectionalLight> dirLight,
	std::vector<std::shared_ptr<PointLight>>& pointLights,
	std::vector<std::shared_ptr<SpotLight>>& spotLights,
	const glm::vec3& camPos)
{
	m_frames[currentFrame]->Render(dirLight, pointLights,spotLights, camPos);
}

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::AnimatedModel(const std::shared_ptr<XMaterial>& mater, const std::vector<std::string>& framePaths) :
	material(mater), currentFrame(0), frameDuration(1.0f / 60.0f), elapsedTime(0.0f)
{
	for (const auto& path : framePaths) {
		auto frame = std::make_shared<Model>(material, path);
		m_frames.push_back(frame);
	}
}
