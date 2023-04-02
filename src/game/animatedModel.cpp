#include "AnimatedModel.h"
#include <iostream>

AnimatedModel::~AnimatedModel()
{

}

void AnimatedModel::Update(float deltaTime)
{
	m_elapsedTime += deltaTime;
	if (m_elapsedTime >= m_frameDuration) {
		m_elapsedTime -= m_frameDuration;
		m_currentFrame = (m_currentFrame + 1) % frames.size();
	}

}

void AnimatedModel::Render(std::shared_ptr<DirectionalLight> dirLight,
	std::vector<std::shared_ptr<PointLight>>& pointLights,
	std::vector<std::shared_ptr<SpotLight>>& spotLights,
	const glm::vec3& camPos)
{
	frames[m_currentFrame]->Render(dirLight, pointLights,spotLights, camPos);
}

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::AnimatedModel(const std::shared_ptr<XMaterial>& mater, const std::vector<std::string>& framePaths) :
	material(mater), m_currentFrame(0), m_frameDuration(1.0f / 60.0f), m_elapsedTime(0.0f)
{
	for (const auto& path : framePaths) {
		auto frame = std::make_shared<Model>(material, path);
		frames.push_back(frame);
	}
}
