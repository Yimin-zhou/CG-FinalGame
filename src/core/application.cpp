#include "application.h"

Application::Application()
	: m_window("Final Project", glm::ivec2(1920, 1080),OpenGLVersion::GL45),
	m_playerCam(std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, -1.0f))),
	m_topDownCam(std::make_shared<TopDownCamera>(glm::vec3(0.0f, 16.0f, 0.0f))),
	is_topDown(false)
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

		// Set X-Toon Shading Texture
		m_toonTexture = std::make_shared<Texture>("resources/toon_map.png", false); //TODO 

		// === Create framebuffer for extra texture ===
		glCreateFramebuffers(1, &m_shadowMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Application::InitShader()
{
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

	ShaderBuilder xToonShader;
	xToonShader.addStage(GL_VERTEX_SHADER, "shaders/xtoon_vert.glsl");
	xToonShader.addStage(GL_FRAGMENT_SHADER, "shaders/xtoon_frag.glsl");
	m_xToonShader = xToonShader.build();

	ShaderBuilder bloomShader;
	bloomShader.addStage(GL_VERTEX_SHADER, "shaders/bloom_vert.glsl");
	bloomShader.addStage(GL_FRAGMENT_SHADER, "shaders/bloom_frag.glsl");
	m_bloomShader = bloomShader.build();

	ShaderBuilder bloomBlurShader;
	bloomBlurShader.addStage(GL_VERTEX_SHADER, "shaders/bloom_blur_vert.glsl");
	bloomBlurShader.addStage(GL_FRAGMENT_SHADER, "shaders/bloom_blur_frag.glsl");
	m_bloomBlurShader = bloomBlurShader.build();
}

void Application::InitLight()
{
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
}

std::shared_ptr<XMaterial> Application::InitMaterial(Shader& shader)
{
	std::shared_ptr<XMaterial> material = std::make_shared<XMaterial>();
	material->SetShader(shader);
	return material;
}

void Application::InitModel()
{
	// create models for environment
	std::shared_ptr<Model> model_floor = std::make_shared<Model>(InitMaterial(m_mainShader), "resources/env/floor/floor.obj");

	std::vector<std::shared_ptr<Model>> models;
	models.push_back(model_floor);

	// Add room models
	uint32_t roomPropCount = 13;
	for (int i = 1; i <= roomPropCount; ++i)
	{
		auto model_room = std::make_shared<Model>(InitMaterial(m_mainShader), "resources/env/" + std::to_string(i) + "/room_" + std::to_string(i) + ".obj");
		models.push_back(model_room);
	}

	m_environment = std::make_shared<Environment>(models);
}

void Application::InitEnemies(std::shared_ptr<XMaterial> material, uint32_t enemyCount)
{
	// create enemies
	m_enemyModel = std::make_shared<Model>(material, "resources/enemy/enemy.obj");

	// Create and store enemies
	for (int i = 0; i < enemyCount; ++i) 
	{
		// random position between -40 and 40
		float x = (rand() % 80) - 40;
		float z = (rand() % 80) - 40;
		auto enemy = std::make_shared<Enemy>(glm::vec3(x, 1, z), 2.0f, 4);
		enemy->model = m_enemyModel;
		m_enemies.push_back(enemy);
	}
}

void Application::Init()
{
	// set window as full screen but not borderless
	m_window.setBorderedFullScreen();
	// set mouse mode
	m_window.setMouseCapture(true);

	// init shader
	InitShader();

	// init lights
	InitLight();

	// create a camera for shadow mapping
	m_shadowCam = std::make_shared<Camera>(m_directionalLight->getPosition());

	// 1 material -> multiple objects
	std::shared_ptr<XMaterial> playerPbrMaterial = InitMaterial(m_mainShader);
	std::shared_ptr<XMaterial> enemyPbrMaterial = InitMaterial(m_mainShader);
	std::shared_ptr<XMaterial> bossBodyPbrMaterial = InitMaterial(m_mainShader);
	std::shared_ptr<XMaterial> projectilePbrMaterial = InitMaterial(m_projectileShader);


	// init models with unique materials
	InitModel();

	// create player
	m_player = std::make_shared<Player>(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
	// init animated model
	const std::vector<std::string> framePaths = loadFramePaths("resources/animatedModels");
	m_player->animatedModel = std::make_shared<AnimatedModel>(playerPbrMaterial, framePaths);

	// create enemies
	InitEnemies(enemyPbrMaterial, 10);

	CollisionManager collisionManager;
	collisionManager.SetPlayer(m_player);

	// create boss
	std::shared_ptr<Boss> m_bossHead = std::make_shared<Boss>(glm::vec3(0, 14, 0), 4.0f, 31);
	std::shared_ptr<Boss> m_bossBody_1 = std::make_shared<Boss>(glm::vec3(0, 14, 0), 4.0f, 31);
	std::shared_ptr<Boss> m_bossBody_2 = std::make_shared<Boss>(glm::vec3(0, 14, 0), 4.0f, 31);
	std::shared_ptr<Boss> m_bossBody_3 = std::make_shared<Boss>(glm::vec3(0, 14, 0), 4.0f, 31);

	m_bossHeadModel = std::make_shared<Model>(InitMaterial(m_mainShader), "resources/boss/Head.obj");
	m_bossBodyModel_1 = std::make_shared<Model>(bossBodyPbrMaterial, "resources/boss/Body.obj");
	m_bossBodyModel_2 = std::make_shared<Model>(bossBodyPbrMaterial, "resources/boss/Body2.obj");
	m_bossBodyModel_3 = std::make_shared<Model>(bossBodyPbrMaterial, "resources/boss/Body3.obj");

	m_bossHead->model = m_bossHeadModel;
	m_bossBody_1->model = m_bossBodyModel_1;
	m_bossBody_2->model = m_bossBodyModel_2;
	m_bossBody_3->model = m_bossBodyModel_3;

	m_bosses.push_back(m_bossHead);
	m_bosses.push_back(m_bossBody_1);
	m_bosses.push_back(m_bossBody_2);
	m_bosses.push_back(m_bossBody_3);
	
	// init projectile model
	m_projectileModel = std::make_shared<Model>(projectilePbrMaterial, "resources/projectile.obj");

	{
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

	{
		// init postprocessing
		m_postProcessing = std::make_shared<PostProcessing>(m_window.getWindowSize().x, m_window.getWindowSize().y);
		m_postProcessing->SetShader(m_bloomShader, m_bloomBlurShader);
	}

	// startTrailer
	m_trailerPlaying = true;
	m_trailerStartTime = static_cast<float>(glfwGetTime());

	// set curve points for plotting the trace of the camera
	std::vector<glm::vec3> m_points;
}

void createCameraControlPoints(std::shared_ptr <Player> m_player, glm::vec3 controlPoints[4]) 
{
	
	controlPoints[0] = m_player->GetPosition() + glm::vec3(5.0f, 8.0f, 7.0f);
	controlPoints[1] = controlPoints[0] + m_player->GetPlayerFront() * 2.0f;
	controlPoints[2] = controlPoints[0] + m_player->GetPlayerFront() * 4.0f + m_player->GetPlayerLeft() * 2.0f;
	controlPoints[3] = controlPoints[0] + m_player->GetPlayerFront() * 6.0f;
	
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
		m_topDownCam->FollowPlayer(m_player);
		
		if (m_trailerPlaying)
		{
			glm::vec3 curve1[4];
			createCameraControlPoints(m_player, curve1);
			BezierCurve cameraPath1(curve1[0], curve1[1], curve1[2], curve1[3]);

			for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
				glm::vec3 point = cameraPath1.evaluate(t);
				m_points.push_back(point);
			}

			glm::vec3 curve2[4];
			// Control points for the second curve (connects smoothly to the first curve)
			curve2[0] = curve1[3];
			curve2[1] = curve2[0] + glm::vec3(-12.0f, 39.0f, 6.0f);
			curve2[2] = curve2[0] + glm::vec3(-3.0f, 17.0f, -5.0f);
			curve2[3] = curve2[0] + glm::vec3(10.0f, 9.0f, 10.0f);
			
			BezierCurve cameraPath2(curve2[0], curve2[1], curve2[2], curve2[3]);

			for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
				glm::vec3 point = cameraPath2.evaluate(t);
				m_points.push_back(point);
			}
			std::vector<BezierCurve> curves = { cameraPath1, cameraPath2 };
			CompositeBezierCurve cameraPath(curves);
			float trailerDuration = 4.0f; // Duration of the trailer in seconds	
			float elapsedTime = currentTime - m_trailerStartTime; 
			
			float t = fmod(currentTime - m_trailerStartTime, trailerDuration) / trailerDuration;
			//m_playerCam->FollowPlayerAlongBezierCurve(m_player, cameraPath1, t);
			m_playerCam->FollowPlayerAlongCompositeBezierCurve(m_player, cameraPath, t);
			if (elapsedTime >= trailerDuration)
			{
				m_trailerPlaying = false;
			}
		}
		else
		{
			// Move at cosntant speed along Bézier curve
			//glm::vec3 bezierCurve[4];
			//createCameraControlPoints(m_player, bezierCurve);

			//BezierCurve cameraPath(bezierCurve[0], bezierCurve[1], bezierCurve[2], bezierCurve[3]);
			//int numSamples = 1000;
			//std::vector<float> arcLengthTable = cameraPath.generateArcLengthTable(numSamples);

			//float trailerDuration = 11.0f; // Duration of the trailer in seconds
			//float elapsedTime = currentTime - m_trailerStartTime;

			//// Move at constant speed along Bézier curve
			//float currentarclength = elapsedTime / trailerDuration; // normalize the elapsed time to range [0, 1]
			//float t = cameraPath.findTForArcLength(arcLengthTable, currentarclength); // find the corresponding 't' value for the given arc length

			////// Set the position of the camera
			//m_playerCam->FollowPlayerAlongBezierCurve(m_player, cameraPath, t);
			
			m_playerCam->FollowPlayer(m_player);
		}

		// update bosses
		for (auto& boss : m_bosses)
		{
			boss->Update(deltaTime, m_player->GetPosition());
		}
		snakeJointAngle = glm::sin(currentTime / 0.8) * 20.0f;
		m_bosses.erase(std::remove_if(m_bosses.begin(), m_bosses.end(),
			[](const std::shared_ptr<Boss>& boss) {
				return !boss->IsAlive();
			}), m_bosses.end());

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
					if (enemy->CheckCollision(projectile->collider))
					{
						enemy->TakeDamage(projectile->GetDamage());
						shouldDestroy = true;
					}
				}

				for (auto& boss : m_bosses)
				{
					if (!boss->IsAlive()) continue;
					if (boss->CheckCollision(projectile->collider))
					{
						for (auto& bossPart : m_bosses)
						{
							bossPart->TakeDamage(projectile->GetDamage());
						}
						shouldDestroy = true;
					}
				}

				return shouldDestroy;
			}), m_projectiles.end());

		collisionManager.enemies = m_enemies;
		collisionManager.bosses = m_bosses;
		collisionManager.projectiles = m_projectiles;
		collisionManager.SetPlayer(m_player);
		collisionManager.CheckCollisions();
		
		// update animated Model
		m_player->animatedModel->Update(deltaTime);

		// render scene
		ShadowRender();
		MainRender();
		PostProcssing();
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
		if (!is_topDown) {
			m_player->SetYaw(m_playerCam->GetYaw());
		}
		else {
			m_player->SetYaw(m_player->GetYaw());
		}
		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(m_player->GetPosition()));
		modelMat = glm::rotate(modelMat, glm::radians(m_player->GetYaw()), { 0, 1, 0 }); // rotate player with camera
		m_player->animatedModel->material->SetShader(m_shadowShader);
		m_player->animatedModel->material->SetMatrix(modelMat, view, proj);
		m_player->animatedModel->material->Apply();
		if (!is_topDown) {
			m_player->animatedModel->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		}
		else {
			m_player->animatedModel->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
		}

		// render unique material objects
		for (auto& m : m_environment->models)
		{
			m->material->SetShader(m_shadowShader);
			m->material->SetMatrix(glm::scale(glm::mat4(1), glm::vec3(0.7f)), view, proj); // scale down the environment
			m->material->Apply();
			if (!is_topDown) {
				m->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
			}
			else {
				m->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
			}
		}

		// render enemies
		for (auto& e : m_enemies)
		{
			if (!e->IsAlive()) continue;
			e->model->material->SetShader(m_shadowShader);
			glm::mat4 modelMat_enemy = glm::translate(glm::mat4(1), glm::vec3(e->GetPosition()));
			e->model->material->SetMatrix(modelMat_enemy, view, proj);
			e->model->material->Apply();
			if (!is_topDown) {
				e->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
			}
			else {
				e->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
			}
		}

		//// render boss
		//int snakeLength = 4;
		//// create tree root (upperarm origin)
		//glm::mat4 bossModelMat = glm::translate(glm::mat4(1), glm::vec3(m_bosses[0]->GetPosition()));
		//std::shared_ptr<ObjectNode> root_snake = std::make_shared<ObjectNode>(bossModelMat);
		//std::vector<std::shared_ptr<ObjectNode>> tempObjects;
		//tempObjects.push_back(root_snake);

		//// body
		//for (int i = 0; i < snakeLength - 1; ++i)
		//{
		//	glm::mat4 translation_snake = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0));
		//	glm::mat4 rotation_snake = glm::rotate(glm::mat4(1.0f), glm::radians(snakeJointAngle), glm::vec3(0, 1, 0));
		//	bossModelMat = translation_snake * rotation_snake;
		//	std::shared_ptr<ObjectNode> node = std::make_shared<ObjectNode>(bossModelMat);
		//	tempObjects[i]->addChild(node);
		//	tempObjects.push_back(node);
		//}
		//traverse(root_snake);

		//for (int i = 0; i < snakeLength; ++i)
		//{
		//	if (!m_bosses[i]->IsAlive()) continue;
		//	glm::mat4 modelMat_boss = tempObjects[i]->transform;
		//	m_bosses[i]->model->material->SetShader(m_mainShader);
		//	m_bosses[i]->model->material->Apply();
		//	m_bosses[i]->model->material->SetMatrix(modelMat_boss, view, proj);
		//	m_bosses[i]->model->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
		//	{
		//		glActiveTexture(GL_TEXTURE3);
		//		glBindTexture(GL_TEXTURE_2D, m_shadowTex);
		//		m_bosses[i]->model->material->SetUniform("shadowMap", 3);
		//	}
		//	if (!is_topDown) {
		//		m_bosses[i]->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
		//	}
		//	else {
		//		m_bosses[i]->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
		//	}
		//}


	}
	// Unbind the off-screen framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::MainRender()
{
	m_postProcessing->BindFramebuffer();
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// // Create a VBO to store the points
	// GLuint vbo;
	// glGenBuffers(1, &vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec3), &m_points[0], GL_STATIC_DRAW);

	// // Create a VAO to define the vertex layout
	// GLuint vao;
	// glGenVertexArrays(1, &vao);
	// glBindVertexArray(vao);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// // Render the points
	// glPointSize(15.0f);
	// glBindVertexArray(vao);
	// glDrawArrays(GL_POINTS, 0, m_points.size());

	// // Clean up
	// glDeleteBuffers(1, &vbo);
	// glDeleteVertexArrays(1, &vao);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glViewport(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
	glScissor(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
	{
		glm::mat4 view, proj;
		if (!is_topDown) 
		{
			view = m_playerCam->GetViewMatrix();
			proj = m_playerCam->GetPerspectiveMatrix(m_window);
		}
		else 
		{
			view = m_topDownCam->GetViewMatrix();
			proj = m_topDownCam->GetPerspectiveMatrix(m_window);
		}

		// render boss
		if (m_bosses.size() > 0) 
		{
			{
				//// set transformations for boss
				int snakeLength = 4;
				// create tree root (upperarm origin) head
				glm::mat4 translation_snake = glm::translate(glm::mat4(1.0f), glm::vec3(0, 2, 0));
				glm::mat4 rotation_body = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1, 0));
				// let the head face player by GetModelMatrix()
				glm::mat4 bossModelMat = m_bosses[0]->GetModelMatrix() * translation_snake * rotation_body;
				std::shared_ptr<ObjectNode> root_snake = std::make_shared<ObjectNode>(bossModelMat);
				std::vector<std::shared_ptr<ObjectNode>> tempObjects;
				tempObjects.push_back(root_snake);
				
				// body
				for (int i = 0; i < snakeLength - 1; ++i)
				{
					glm::mat4 translation_snake = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0));
					glm::mat4 rotation_snake = glm::rotate(glm::mat4(1.0f), glm::radians(snakeJointAngle), glm::vec3(0, 1, 0));
					glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_bosses[0]->GetYaw()), glm::vec3(0, 1, 0));
					bossModelMat = translation_snake * rotation_snake;
					std::shared_ptr<ObjectNode> node = std::make_shared<ObjectNode>(bossModelMat);
					tempObjects[i]->addChild(node);
					tempObjects.push_back(node);
				}
				traverse(root_snake);
				// render head and body
				for (int i = 0; i < snakeLength; ++i)
				{
					if (m_bosses.size() <= 0 || !m_bosses[i]->IsAlive()) break;
					glm::mat4 modelMat_boss = tempObjects[i]->transform;
					m_bosses[i]->model->material->SetShader(m_mainShader);
					m_bosses[i]->model->material->Apply();
					m_bosses[i]->model->material->SetMatrix(modelMat_boss, view, proj);
					m_bosses[i]->model->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
					{
						glActiveTexture(GL_TEXTURE3);
						glBindTexture(GL_TEXTURE_2D, m_shadowTex);
						m_bosses[i]->model->material->SetUniform("shadowMap", 3);
					}
					if (!is_topDown) {
						m_bosses[i]->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
					}
					else {
						m_bosses[i]->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
					}
				}
			}
		}

		// render player
		{
			if (!is_topDown) {
				m_player->SetYaw(m_playerCam->GetYaw());
			}
			else {
				m_player->SetYaw(m_player->GetYaw());
			}
			glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(m_player->GetPosition()));
			modelMat = glm::rotate(modelMat, glm::radians(m_player->GetYaw()), { 0, 1, 0 }); // rotate player with camera
			m_player->animatedModel->material->SetShader(m_mainShader);
			m_player->animatedModel->material->SetMatrix(modelMat, view, proj);
			m_player->animatedModel->material->Apply();
			m_player->animatedModel->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, m_shadowTex);
				m_player->animatedModel->material->SetUniform("shadowMap", 3);
			}
			if (!is_topDown) 
			{
				m_player->animatedModel->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
			}
			else 
			{
				m_player->animatedModel->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
			}
		}
	

		// render unique material env props
		{
			for (auto& m : m_environment->models)
			{
				m->material->SetShader(m_mainShader);
				m->material->SetMatrix(glm::scale(glm::mat4(1), glm::vec3(0.7f)), view, proj); // scale down the environment
				m->material->Apply();
				m->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
				{
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_2D, m_shadowTex);
					m->material->SetUniform("shadowMap", 3);
				}
				if (!is_topDown) {
					m->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
				}
				else {
					m->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
				}
			}
		}
		
		// render enemies
		{
			for (auto& e : m_enemies)
			{
				if (!m_player->is_abilityOn){
					if (!e->IsAlive()) continue;
					e->model->material->SetShader(m_mainShader);
					glm::mat4 modelMat_enemy = e->GetModelMatrix();
					e->model->material->SetMatrix(modelMat_enemy, view, proj);
					e->model->material->Apply();
					e->model->material->SetUniform("lightSpaceMatrix", m_shadowCam->GetOthoProjMatrix() * m_shadowCam->GetOthoViewMatrix());
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_2D, m_shadowTex);
					e->model->material->SetUniform("shadowMap", 3);
				}
				else {
					if (!e->IsAlive()) continue;
					e->model->material->SetShader(m_xToonShader);
					glm::mat4 modelMat_enemy = e->GetModelMatrix();
					e->model->material->SetMatrix(modelMat_enemy, view, proj);
					e->model->material->Apply();
					m_toonTexture->bind(GL_TEXTURE4); // TODO TOON MAP
					e->model->material->SetUniform("toonMap", 4);
					e->model->material->SetUniform("viewPosition", m_playerCam->GetPosition());
					e->model->material->SetUniform("lightPosition", m_directionalLight->getPosition());
					//e->model->material->SetUniform("distanceOffset", );
					//e->model->material->SetUniform("distanceScale", );
				}
				if (!is_topDown) {
					e->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
				}
				else {
					e->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
				}
			}
		}

		// render projectiles
		{
			for (auto& p : m_projectiles)
			{
				// render projectile
				glm::mat4 modelMat_projectile = glm::translate(glm::mat4(1), glm::vec3(p->GetPosition()));
				p->model->material->SetMatrix(modelMat_projectile, view, proj);
				p->model->material->Apply();
				p->model->material->SetUniform("emissiveColor", glm::vec3(0.1, 0.8, 0.95));
				if (!is_topDown) {
					p->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_playerCam->GetPosition());
				}
				else {
					p->model->Render(m_directionalLight, m_pointLights, m_spotLights, m_topDownCam->GetPosition());
				}
			}
		}

		// update particles
		{
			m_particleSystem->Update(deltaTime);
			// render particles
			m_particleSystem->SetShader(m_particleShader);
			m_particleSystem->SetViewMatrix(view);
			m_particleSystem->SetProjectionMatrix(proj);
			m_particleSystem->Render();
		}

		m_postProcessing->UnbindFramebuffer();
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

}

void Application::PostProcssing()
{
	m_postProcessing->BlurScreenTex();
	m_postProcessing->RenderToScreen();

	m_window.swapBuffers();
}

void Application::ProcessContinousInput()
{
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
	{
		if (!is_topDown) {
			m_player->MoveForward(deltaTime);
		}
		else {
			m_player->MatrixMoveForward(deltaTime);
		}
	}
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
	{
		if (!is_topDown) {
			m_player->MoveBackward(deltaTime);
		}
		else {
			m_player->MatrixMoveBackward(deltaTime);
		}
	}
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
	{
		if (!is_topDown) {
			m_player->MoveLeft(deltaTime);
		}
		else {
			m_player->MatrixMoveLeft(deltaTime);
		}
	}
	if (glfwGetKey(m_window.getWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		if (!is_topDown) {
			m_player->MoveRight(deltaTime);
		}
		else {
			m_player->MatrixMoveRight(deltaTime);
		}
	}

	if (glfwGetMouseButton(m_window.getWindowHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_player->shootingTimer <= 0.0f)
	{
		std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(glm::vec3(m_player->GetPosition().x , 
			m_player->GetPosition().y + 1.5f, m_player->GetPosition().z),
			m_player->GetPlayerFront(),
			20.0f, 2, m_projectileModel); // 1.5f is the player height(offset)

		m_projectiles.push_back(projectile);

		m_player->shootingTimer = m_player->shootingInterval;

		//std::cout << "Shoot" << std::endl;
	}
}

void Application::change2XToonShader() {
	if (m_player->abilityTimer <= 0) {
		m_player->is_abilityOn = true;
		m_player->abilityTimer = m_player->abilityInterval;
	}
}

void Application::onKeyPressed(int key, int mods) 
{
	switch (key)
	{
		case GLFW_KEY_C: 
			is_topDown = !is_topDown;
			break;
		case GLFW_KEY_E:
			change2XToonShader();
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
			std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(glm::vec3(rand() % 40 - 20, 1, rand() % 40 - 20), 3.0f, 10);
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
	if (!is_topDown) {
		m_playerCam->ProcessMouseMovement(xoffset, yoffset, true);
	}
	else {
		m_player->ProcessMouseMovement(xoffset, yoffset, true);
		m_topDownCam->ProcessMouseMovement(xoffset, yoffset, true);
	}
}

void Application::onMouseScroll(const glm::dvec2& cursorPos)
{
	/*if (!is_topDown) {
		m_playerCam->ProcessMouseScroll(static_cast<float>(cursorPos.y));
	}
	else {
		m_topDownCam->ProcessMouseScroll(static_cast<float>(cursorPos.y));
	}*/
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
	ImGui::Text("Player Ability On: %s", m_player->is_abilityOn ? "true" : "false");
	ImGui::End();

	// camera info
	ImGui::Begin("Camera Info");
	ImGui::Text("Camera Position: %.1f %.1f %.1f", m_playerCam->GetPosition().x, m_playerCam->GetPosition().y, m_playerCam->GetPosition().z);
	ImGui::Text("Camera Yaw: %.1f", m_playerCam->GetYaw());
	ImGui::Text("Camera Pitch: %.1f", m_playerCam->GetPitch());
	ImGui::Text("Camera Up: %.1f %.1f %.1f", m_playerCam->GetUp().x, m_playerCam->GetUp().y, m_playerCam->GetUp().z);
	ImGui::Text("Camera Front: %.1f %.1f %.1f", m_playerCam->GetFront().x, m_playerCam->GetFront().y, m_playerCam->GetFront().z);
	ImGui::End();

	ImGui::Begin("Camera Info");
	ImGui::Text("TopDown Camera Position: %.1f %.1f %.1f", m_topDownCam->GetPosition().x, m_topDownCam->GetPosition().y, m_topDownCam->GetPosition().z);
	ImGui::Text("TopDown Camera Yaw: %.1f", m_topDownCam->GetYaw());
	ImGui::Text("TopDown Camera Pitch: %.1f", m_topDownCam->GetPitch());
	ImGui::Text("TopDown Camera Up: %.1f %.1f %.1f", m_topDownCam->GetUp().x, m_topDownCam->GetUp().y, m_topDownCam->GetUp().z);
	ImGui::Text("TopDown Camera Front: %.1f %.1f %.1f", m_topDownCam->GetFront().x, m_topDownCam->GetFront().y, m_topDownCam->GetFront().z);
	ImGui::Text("TopDown Camera Right: %.1f %.1f %.1f", m_topDownCam->GetRight().x, m_topDownCam->GetRight().y, m_topDownCam->GetRight().z);
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

	// boss info
	ImGui::Begin("Boss Info");
	// loop through all enemies
	for (int i = 0; i < m_bosses.size(); i++)
	{
		ImGui::Text("Boss %d Position: %.1f %.1f %.1f", i, m_bosses[i]->GetPosition().x, m_bosses[i]->GetPosition().y, m_bosses[i]->GetPosition().z);
		ImGui::Text("Boss %d Health: %d", i, m_bosses[i]->GetHealth());
	}
	ImGui::End();
}



