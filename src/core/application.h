#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glad/glad.h>
// Include glad before glfw3
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
DISABLE_WARNINGS_POP()
#include <framework/shader.h>
#include <framework/window.h>

#include <functional>
#include <iostream>
#include <vector>
#include <memory>

#pragma once

#include "render/environment.h"
#include "render/light/directionalLight.h"
#include "render/light/pointLight.h"
#include "render/light/spotLight.h"
#include "game/player.h"
#include "game/camera.h"
#include "game/enemy.h"
#include "game/projectile.h"
#include "game/animatedModel.h"
#include "game/ntree.h"
#include "game/boss.h"

class Application
{
public:
	float deltaTime = 0;
	float lastFrameTime;
	float snakeJointAngle = 0;

	void Init();
	void OnUpdate();
	void ShadowRender();
	void MainRender();
	void ProcessContinousInput();

	void onKeyPressed(int key, int mods);
	void onKeyReleased(int key, int mods);
	void onMouseMove(const glm::dvec2& cursorPos);
	void onMouseScroll(const glm::dvec2& cursorPos);
	void onMouseClicked(int button, int mods);
	void onMouseReleased(int button, int mods);
	const std::vector<std::string> loadFramePaths(const std::string& folderPath);

	void DebugWindows();

	Application();

private:
	bool m_trailerPlaying = false;
	float m_trailerStartTime = -1.0f;
	Window m_window;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Camera> m_playerCam;
	
	std::shared_ptr<AnimatedModel> m_animatedModel;

	std::vector<std::shared_ptr<Boss>> m_bosses;
	std::shared_ptr<Model> m_bossHeadModel;
	std::shared_ptr<Model> m_bossBodyModel_1;
	std::shared_ptr<Model> m_bossBodyModel_2;
	std::shared_ptr<Model> m_bossBodyModel_3;

	std::vector<std::shared_ptr<Projectile>> m_projectiles;
	std::shared_ptr<Model> m_projectileModel;
	std::shared_ptr<Model> m_enemyModel;
	std::vector<std::shared_ptr<Enemy>> m_enemies;

	// std::shared_ptr<Level> m_level;
	std::shared_ptr<Environment> m_environment;
	
	std::vector<glm::vec3> m_points;

	std::shared_ptr<DirectionalLight> m_directionalLight;
	std::vector<std::shared_ptr<PointLight>> m_pointLights;
	std::vector<std::shared_ptr<SpotLight>> m_spotLights;

	// shadow mapping
	GLuint m_shadowTex;
	GLuint m_shadowMapFBO;
	std::shared_ptr<Camera> m_shadowCam;

	Shader m_shadowShader;
	Shader m_mainShader;
};
