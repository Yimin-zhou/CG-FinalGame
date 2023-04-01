#include "application.h"

Application::Application()
	: m_window("Final Project", glm::ivec2(1920, 1080),OpenGLVersion::GL45)
{
	m_window.registerKeyCallback(
		[this](int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS)
				onKeyPressed(key, mods);
			else if (action == GLFW_RELEASE)
				onKeyReleased(key, mods);
		});
	m_window.registerMouseMoveCallback(
		std::bind(&Application::onMouseMove, this, std::placeholders::_1));
	m_window.registerMouseButtonCallback(
		[this](int button, int action, int mods) {
			if (action == GLFW_PRESS)
				onMouseClicked(button, mods);
			else if (action == GLFW_RELEASE)
				onMouseReleased(button, mods);
		});
	m_window.registerScrollCallback(
		std::bind(&Application::onMouseScroll, this, std::placeholders::_1));

	// init shadow frambuffer and texture
	{
		// === Create Shadow Texture ===
		const int SHADOWTEX_WIDTH = 2048;
		const int SHADOWTEX_HEIGHT = 2048;
		glGenTextures(1, &m_shadowTex);
		glBindTexture(GL_TEXTURE_2D, m_shadowTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		// Set behaviour for when texture coordinates are outside the [0, 1] range.
		glTextureParameteri(m_shadowTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_shadowTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set interpolation for texture sampling (GL_NEAREST for no interpolation).
		glTextureParameteri(m_shadowTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_shadowTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// === Create framebuffer for extra texture ===
		glCreateFramebuffers(1, &m_shadowMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	m_playerCam = std::make_shared<Camera>();
}

void Application::Init()
{
	// set window as full screen but not borderless
	m_window.setBorderedFullScreen();
	// set mouse mode
	m_window.setMouseCapture(true);

	// create a camera
	glm::vec3 initialCameraPosition(0.0f, 0.0f, -1.0f);
	m_playerCam = std::make_shared<Camera>(initialCameraPosition);

	// build shaders
	ShaderBuilder mainBuilder;
	mainBuilder.addStage(GL_VERTEX_SHADER, "shaders/pbr_vert.glsl");
	mainBuilder.addStage(GL_FRAGMENT_SHADER, "shaders/pbr_frag.glsl");
	m_mainShader = mainBuilder.build();

	ShaderBuilder shadowBuilder;
	shadowBuilder.addStage(GL_VERTEX_SHADER, "shaders/shadow_vert.glsl");
	m_shadowShader = shadowBuilder.build();

	ShaderBuilder projectileBuilder;
	projectileBuilder.addStage(GL_VERTEX_SHADER, "shaders/projectile_vert.glsl");
	projectileBuilder.addStage(GL_FRAGMENT_SHADER, "shaders/projectile_frag.glsl");
	m_projectileShader = projectileBuilder.build();

	ShaderBuilder particleBuilder;
	particleBuilder.addStage(GL_VERTEX_SHADER, "shaders/particle_vert.glsl");
	particleBuilder.addStage(GL_FRAGMENT_SHADER, "shaders/particle_frag.glsl");
	m_particleShader = particleBuilder.build();

	// setup lights
	m_directionalLight = std::make_shared<DirectionalLight>(glm::vec3(10.0f, 30.0f, 30.0f), glm::vec3(1.0f), 1.0f);
	// point light
	std::shared_ptr<PointLight> pointLight_1 = std::make_shared<PointLight>(glm::vec3(10.0f, 5.0f, -10.0f),
		glm::vec3(0.95f, 0.2f, 0.9f), 100.0f, 1.0f, 0.2f, 0.2f);
	std::shared_ptr<PointLight> pointLight_2 = std::make_shared<PointLight>(glm::vec3(-10.0f, 5.0f, 10.0f),
		glm::vec3(0.1f, 0.8f, 0.9f), 100.0f, 1.0f, 0.2f, 0.2f);
	std::shared_ptr<PointLight> pointLight_3 = std::make_shared<PointLight>(glm::vec3(-10.0f, 5.0f, -10.0f),
		glm::vec3(0.95f, 0.8f, 0.1f), 100.0f, 1.0f, 0.2f, 0.2f);
	std::shared_ptr<PointLight> pointLight_4 = std::make_shared<PointLight>(glm::vec3(10.0f, 5.0f, 10.0f),
		glm::vec3(0.1f, 0.8f, 0.2f), 100.0f, 1.0f, 0.2f, 0.2f);
	m_pointLights.push_back(pointLight_1);
	m_pointLights.push_back(pointLight_2);
	m_pointLights.push_back(pointLight_3);
	m_pointLights.push_back(pointLight_4);

	// spot light
	std::shared_ptr<SpotLight> spotLight_1 = std::make_shared<SpotLight>(glm::vec3(20.0f, 15.0f, -20.0f),
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.9f), 100.0f, 1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(45.0f)));
	std::shared_ptr<SpotLight> spotLight_2 = std::make_shared<SpotLight>(glm::vec3(-20.0f, 15.0f, 20.0f),
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.95f, 0.2f, 0.2f), 100.0f, 1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(45.0f)));
	std::shared_ptr<SpotLight> spotLight_3 = std::make_shared<SpotLight>(glm::vec3(-20.0f, 15.0f, -20.0f),
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.3f, 0.8f, 0.8f), 100.0f, 1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(45.0f)));
	std::shared_ptr<SpotLight> spotLight_4 = std::make_shared<SpotLight>(glm::vec3(20.0f, 15.0f, 20.0f),
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.8f, 0.9f, 0.2f), 100.0f, 1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(45.0f)));
	m_spotLights.push_back(spotLight_1);
	m_spotLights.push_back(spotLight_2);
	m_spotLights.push_back(spotLight_3);
	m_spotLights.push_back(spotLight_4);

	// create a camera for shadow mapping
	m_shadowCam = std::make_shared<Camera>(m_directionalLight->getPosition());

	// create materials
	std::shared_ptr<XMaterial> statuePbrMaterial = std::make_shared<XMaterial>();
	statuePbrMaterial->SetShader(m_mainShader);

	std::shared_ptr<XMaterial> playerPbrMaterial = std::make_shared<XMaterial>();
	playerPbrMaterial->SetShader(m_mainShader);

	std::shared_ptr<XMaterial> enemyPbrMaterial = std::make_shared<XMaterial>();
	enemyPbrMaterial->SetShader(m_mainShader);

	std::shared_ptr<XMaterial> projectileMaterial = std::make_shared<XMaterial>();
	projectileMaterial->SetShader(m_projectileShader);

	// create models
	std::shared_ptr<Model> model_room = std::make_shared<Model>(statuePbrMaterial, "resources/room.obj");
	std::shared_ptr<Model> model_statue = std::make_shared<Model>(statuePbrMaterial, "resources/statue/statue.obj");

	// create environment, contains static objects
	std::vector<std::shared_ptr<Model>> models;
	models.push_back(model_room);
	models.push_back(model_statue);
	m_environment = std::make_shared<Environment>(models);

	// create player
	m_player = std::make_shared<Player>(glm::vec3(0.0f, 0.0f, 0.0f), 7.0f);
	m_player->model = std::make_shared<Model>(playerPbrMaterial, "resources/player/player.obj");

	// create enemies
	m_enemyModel = std::make_shared<Model>(enemyPbrMaterial, "resources/enemy/enemy.obj");
	std::shared_ptr<Enemy> enemy_1 = std::make_shared<Enemy>(glm::vec3(0, 0, 0), 1.0f, 4);
	enemy_1->model = m_enemyModel;
	m_enemies.push_back(enemy_1);

	// init projectile model
	m_projectileModel = std::make_shared<Model>(projectileMaterial, "resources/projectile.obj");

	// init animated model
	const std::vector<std::string> framePaths = loadFramePaths("resources/animatedModels");
	m_animatedModel = std::make_shared<AnimatedModel>(statuePbrMaterial, framePaths);

	// init particle system
	m_particleSystem = std::make_shared<ParticleSystem>();
	m_particleProps.colorBegin = glm::vec4(0.9f, 0.9f, 0.7f, 1.0f);
	m_particleProps.colorEnd = glm::vec4(0.1f, 0.8f, 0.1f, 0.0f);
	m_particleProps.sizeBegin = 0.5f, m_particleProps.sizeVariation = 0.3f, m_particleProps.sizeEnd = 0.0f;
	m_particleProps.lifeTime = 5.0f;
	m_particleProps.velocity = glm::vec3(0.0f, 1.0f, 0.0f);
	m_particleProps.velocityVariation = glm::vec3(2.0f, 2.0f, 2.0f);
	m_particleProps.position = glm::vec3(10.0f, 1.0f, 10.0f);
}

void Application::OnUpdate() 
{
	lastFrameTime = static_cast<float>(glfwGetTime());
	while (!m_window.shouldClose()) 
	{
		// set delta time
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrameTime;

		// process input
		m_window.updateInput();
		ProcessContinousInput();

		// Player
		// Update the camera's position and orientation based on the player's position
		m_player->Update(deltaTime);
		m_playerCam->FollowPlayer(m_player);

		// update enemies, delete dead enemies
		for (auto& enemy : m_enemies)
		{
			enemy->Update(deltaTime, m_player->GetPosition());
		}	
		m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
			[](const std::shared_ptr<Enemy>& enemy) {
				return !enemy->IsAlive();
			}), m_enemies.end());


		// update projectile, delete dead projectiles
		m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(),
			[&](const std::shared_ptr<Projectile>& projectile) 
			{
				bool shouldDestroy = projectile->Update(deltaTime);

				for (auto& enemy : m_enemies) 
				{
					if (!enemy->IsAlive()) continue;
					if(enemy->CheckCollision(projectile->GetPosition())) 
					{
						enemy->TakeDamage(projectile->GetDamage());
						shouldDestroy = true;
					}
				}

				return shouldDestroy;
			}), m_projectiles.end());
		
		// update animated Model
		m_animatedModel->Update(deltaTime);

		// render scene
		ShadowRender();
		MainRender();
		lastFrameTime = currentTime;
	}
}

void Application::ShadowRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
	// Clear the shadow map and set needed options
	glClearDepth(1.0f); // Set the clear value for the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT); // Clear the depth buffer
	glEnable(GL_DEPTH_TEST);
	// Set viewport size
	glViewport(0, 0, 2048, 2048);

	{
		glm::mat4 view = m_shadowCam->GetOthoViewMatrix();
		glm::mat4 proj = m_shadowCam->GetOthoProjMatrix();

		// set player matrix & render
		m_shadowShader.bind();
		m_player->SetYaw(m_playerCam->GetYaw());
		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(m_player->GetPosition()));
		modelMat = glm::rotate(modelMat, glm::radians(m_player->GetYaw()), { 0, 1, 0 }); // rotate player with camera
		m_player->model->material->SetShader(m_shadowShader);
		m_player->model->material->SetMatrix(modelMat, view, proj);
		m_player->model->material->Apply();
		m_player->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());

		// render animated model
		m_animatedModel->material->SetShader(m_shadowShader);
		m_animatedModel->material->SetMatrix(glm::mat4(1), view, proj);
		m_animatedModel->material->Apply();
		m_animatedModel->Render(m_directionalLight, m_pointLights,
			m_spotLights, m_playerCam->GetPosition());

		// render objects
		for (auto& m : m_environment->models)
		{
			m->material->SetShader(m_shadowShader);
			m->material->SetMatrix(glm::mat4(1), view, proj);
			m->material->Apply();
			m->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		}

		// render enemies
		for (auto& e : m_enemies)
		{
			if (!e->IsAlive()) continue;
			e->model->material->SetShader(m_shadowShader);
			glm::mat4 modelMat_enemy = glm::translate(glm::mat4(1), glm::vec3(e->GetPosition()));
			e->model->material->SetMatrix(modelMat_enemy, view, proj);
			e->model->material->Apply();
			e->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		}
	}
	// Unbind the off-screen framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::MainRender()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
	glScissor(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
	{
		glm::mat4 view = m_playerCam->GetViewMatrix();
		glm::mat4 proj = m_playerCam->GetPerspectiveMatrix(m_window);

		// set player matrix & render
		m_player->SetYaw(m_playerCam->GetYaw());
		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(m_player->GetPosition()));
		modelMat = glm::rotate(modelMat, glm::radians(m_player->GetYaw()), { 0, 1, 0 }); // rotate player with camera
		m_player->model->material->SetShader(m_mainShader);
		m_player->model->material->SetMatrix(modelMat, view, proj);
		m_player->model->material->Apply();
		m_player->model->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_shadowTex);
			m_player->model->material->SetUniform("shadowMap", 3);
		}
		m_player->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());

		// render animated model
		m_animatedModel->material->SetShader(m_mainShader);
		m_animatedModel->material->SetMatrix(glm::mat4(1), view, proj);
		m_animatedModel->material->Apply();
		m_animatedModel->Render(m_directionalLight, m_pointLights, 
			m_spotLights, m_playerCam->GetPosition());

		// render objects
		for (auto& m : m_environment->models)
		{
			m->material->SetShader(m_mainShader);
			m->material->SetMatrix(glm::mat4(1), view, proj);
			m->material->Apply();
			m_player->model->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, m_shadowTex);
				m->material->SetUniform("shadowMap", 3);
			}
			m->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		}

		// render enemies
		for (auto& e : m_enemies)
		{
			if (!e->IsAlive()) continue;
			e->model->material->SetShader(m_mainShader);
			glm::mat4 modelMat_enemy = glm::translate(glm::mat4(1), glm::vec3(e->GetPosition()));
			e->model->material->SetMatrix(modelMat_enemy, view, proj);
			e->model->material->Apply();
			m_player->model->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, m_shadowTex);
				e->model->material->SetUniform("shadowMap", 3);
			}
			e->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		}

		// render projectiles
		for (auto& p : m_projectiles)
		{
			// render projectile
			glm::mat4 modelMat_projectile = glm::translate(glm::mat4(1), glm::vec3(p->GetPosition()));
			p->model->material->SetMatrix(modelMat_projectile, view, proj);
			p->model->material->Apply();
			p->model->material->SetUniform("emissiveColor", glm::vec3(0.1, 0.8, 0.95));
			p->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		}

		// update particles
		m_particleSystem->Update(deltaTime);
		// render particles
		m_particleSystem->SetShader(m_particleShader);
		m_particleSystem->SetModelMatrix(modelMat);
		m_particleSystem->SetViewMatrix(view);
		m_particleSystem->SetProjectionMatrix(proj);
		m_particleSystem->Render();
	}
	

#if _DEBUG
	DebugWindows();
#else
	ImGui::Begin("Debug Info");
	ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
	// frame time in ms
	ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
	ImGui::End();
#endif

	m_window.swapBuffers();
}

void Application::ProcessContinousInput()
{
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
	{
		m_player->MoveForward(deltaTime);
	}
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
	{
		m_player->MoveBackward(deltaTime);
	}
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
	{
		m_player->MoveLeft(deltaTime);
	}
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		m_player->MoveRight(deltaTime);
	}

	if (glfwGetMouseButton(m_window.getWindowHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_player->shootingTimer <= 0.0f)
	{
		std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(glm::vec3(m_player->GetPosition().x , 
			m_player->GetPosition().y + 1.5f, m_player->GetPosition().z),
			m_player->GetPlayerFront(),
			15.0f, 2, m_projectileModel); // 1.5f is the player height(offset)

		m_projectiles.push_back(projectile);

		m_player->shootingTimer = m_player->shootingInterval;

		//std::cout << "Shoot" << std::endl;
	}
}


void Application::onKeyPressed(int key, int mods) 
{
	switch (key)
	{
		case GLFW_KEY_C: 
			m_playerCam->SwitchCameraMode();
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(m_window.getWindowHandle(), true);
		case GLFW_KEY_F1:
			m_window.setMouseCapture(false);
			break;
		case GLFW_KEY_Q:
			// emit particles
			for (uint32_t i = 0; i < 5; i++)
			{
				m_particleSystem->Emit(m_particleProps);
			}
			break;
		case GLFW_KEY_F2:
			// spawn new enemy at random position, random position between -20 and 20
			std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(glm::vec3(rand() % 40 - 20, 0, rand() % 40 - 20), 3.0f, 10);
			enemy->model = m_enemyModel;
			m_enemies.push_back(enemy);
			break;
	}
}


void Application::onKeyReleased(int key, int mods) 
{

}

// If the mouse is moved this function will be called with the x, y
// screen-coordinates of the mouse
void Application::onMouseMove(const glm::dvec2& cursorPos) 
{
    static float lastX = 0.0f, lastY = 0.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = static_cast<float>(cursorPos.x);
        lastY = static_cast<float>(cursorPos.y);
        firstMouse = false;
    }
	
    float xoffset = static_cast<float>(cursorPos.x) - lastX;
    float yoffset = lastY - static_cast<float>(cursorPos.y); // Reversed since y-coordinates go from bottom to top
    lastX = static_cast<float>(cursorPos.x);
    lastY = static_cast<float>(cursorPos.y);

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

	m_playerCam->ProcessMouseMovement(xoffset, yoffset,true);
}

void Application::onMouseScroll(const glm::dvec2& cursorPos)
{
	m_playerCam->ProcessMouseScroll(static_cast<float>(cursorPos.y));
}

void Application::onMouseClicked(int button, int mods) 
{

}

void Application::onMouseReleased(int button, int mods) 
{

}

const std::vector<std::string> Application::loadFramePaths(const std::string& folderPath) {
	std::vector<std::string> framePaths;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".obj") {
			framePaths.push_back(entry.path().string());
		}
	}

	// Sort the frame paths to ensure they are in the correct order
	std::sort(framePaths.begin(), framePaths.end());

	return framePaths;
}

void Application::DebugWindows()
{
	// disable ImGui interaction while mouse is captured
	if (m_window.isMouseCaptured)
	{
		ImGui::GetIO().MouseDrawCursor = false;
		ImGui::GetIO().WantCaptureMouse = false;
		ImGui::GetIO().WantCaptureKeyboard = false;
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::GetIO().WantCaptureMouse = true;
		ImGui::GetIO().WantCaptureKeyboard = true;
	}

	
	// imgui debug windows
	ImGui::Begin("Debug Info");
	ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
	ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);
	ImGui::End();

	// player info
	ImGui::Begin("Player Info");
	ImGui::Text("Player Health: %d", m_player->GetHealth());
	ImGui::Text("Player Position: %.1f %.1f %.1f", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
	ImGui::Text("Player Yaw: %.1f", m_player->GetYaw());
	ImGui::Text("Player Up: %.1f %.1f %.1f", m_player->GetPlayerUp().x, m_player->GetPlayerUp().y, m_player->GetPlayerUp().z);
	ImGui::Text("Player Front: %.1f %.1f %.1f", m_player->GetPlayerFront().x, m_player->GetPlayerFront().y, m_player->GetPlayerFront().z);
	ImGui::Text("Player Left: %.1f %.1f %.1f", m_player->GetPlayerLeft().x, m_player->GetPlayerLeft().y, m_player->GetPlayerLeft().z);
	ImGui::End();

	// camera info
	ImGui::Begin("Camera Info");
	ImGui::Text("Camera Position: %.1f %.1f %.1f", m_playerCam->GetPosition().x, m_playerCam->GetPosition().y, m_playerCam->GetPosition().z);
	ImGui::Text("Camera Yaw: %.1f", m_playerCam->GetYaw());
	ImGui::Text("Camera Pitch: %.1f", m_playerCam->GetPitch());
	ImGui::Text("Camera Up: %.1f %.1f %.1f", m_playerCam->GetUp().x, m_playerCam->GetUp().y, m_playerCam->GetUp().z);
	ImGui::Text("Camera Front: %.1f %.1f %.1f", m_playerCam->GetFront().x, m_playerCam->GetFront().y, m_playerCam->GetFront().z);
	ImGui::End();

	// projectile info
	ImGui::Begin("Projectile Info");
	// loop through all projectiles
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		ImGui::Text("Projectile %d Position: %.1f %.1f %.1f", i, m_projectiles[i]->GetPosition().x, m_projectiles[i]->GetPosition().y, m_projectiles[i]->GetPosition().z);
	}
	ImGui::End();

	// enemy info
	ImGui::Begin("Enemy Info");
	// loop through all enemies
	for (int i = 0; i < m_enemies.size(); i++)
	{
		ImGui::Text("Enemy %d Position: %.1f %.1f %.1f", i, m_enemies[i]->GetPosition().x, m_enemies[i]->GetPosition().y, m_enemies[i]->GetPosition().z);
		ImGui::Text("Enemy %d Health: %d", i, m_enemies[i]->GetHealth());
	}
	ImGui::End();
}



