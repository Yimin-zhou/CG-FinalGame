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

#include "game/level.h"
#include "game/player.h"
#include "game/camera.h"

class Application
{
public:
	float deltaTime = 0;
	float lastFrameTime;

	void Init();
	void OnUpdate();
	void Render();
	void ProcessContinousInput();

	void onKeyPressed(int key, int mods);
	void onKeyReleased(int key, int mods);
	void onMouseMove(const glm::dvec2& cursorPos);
	void onMouseScroll(const glm::dvec2& cursorPos);
	void onMouseClicked(int button, int mods);
	void onMouseReleased(int button, int mods);

	Application();

private:
	Window m_window;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Camera> m_playerCam;

	// std::shared_ptr<Level> m_level;
	std::shared_ptr<Environment> m_environment;
};
