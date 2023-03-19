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

	// create material
	std::shared_ptr<XMaterial> defaultMaterial = std::make_shared<XMaterial>();
	// create models
	std::shared_ptr<Model> model_1 = std::make_shared<Model>(defaultMaterial);
	std::shared_ptr<Model> model_2 = std::make_shared<Model>(defaultMaterial);
	model_1->SetMesh("resources/dragon.obj");
	// create environment
	std::vector<std::shared_ptr<Model>> models;
	models.push_back(model_1);
	models.push_back(model_2);
	m_environment = std::make_shared<Environment>(models);

	// create player
	m_player = std::make_shared<Player>(glm::vec3(0, 0, 0), 5.0f);
	m_player->model = std::make_shared<Model>(defaultMaterial);
	m_player->model->SetMesh("resources/player.obj");

	// // create level
	// std::vector<std::shared_ptr<Environment>> environments;
	// environments.push_back(std::make_shared<Environment>(defaultEnvironment));

	// m_level = std::make_shared<Level>(environments);
}

void Application::OnUpdate() 
{
	lastFrameTime = static_cast<float>(glfwGetTime());
	while (!m_window.shouldClose()) 
	{
		// set delta time
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		// process input
		m_window.updateInput();
		ProcessContinousInput();

		// Player
		// Update the camera's position and orientation based on the player's position
		m_playerCam->FollowPlayer(m_player);

		// render scene
		Render();
		lastFrameTime = currentTime;
	}
}

void Application::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
	glScissor(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);

	glm::mat4 view = m_playerCam->GetViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(80.0f), (float)m_window.getWindowSize().x / (float)m_window.getWindowSize().y, 0.1f, 1000.0f);
	
	//std::cout << m_player->GetPosition().x << " " << m_player->GetPosition().y << " " << m_player->GetPosition().z << std::endl;

	// set player matrix & render
	m_player->SetYaw(m_playerCam->GetYaw());
	glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(m_player->GetPosition()));
	modelMat = glm::rotate(modelMat, glm::radians(m_player->GetYaw()), { 0, 1, 0 });
	m_player->model->material->SetMatrix(modelMat, view, proj);
	m_player->model->Render();

	// render objects'
	int i = 0;
	for (auto& m : m_environment->models)
	{
		m->material->SetMatrix(glm::translate(glm::mat4(1), glm::vec3(i)), view, proj);
		m->Render();
		i++;
	}

	// imgui debug window
	ImGui::Begin("Debug Info");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Camera Position: %.1f %.1f %.1f", m_playerCam->GetPosition().x, m_playerCam->GetPosition().y, m_playerCam->GetPosition().z);
	ImGui::Text("Player Position: %.1f %.1f %.1f", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);
	ImGui::Text("Player Yaw: %.1f", m_player->GetYaw());
	ImGui::Text("Player Up: %.1f %.1f %.1f", m_player->GetPlayerUp().x, m_player->GetPlayerUp().y, m_player->GetPlayerUp().z);
	ImGui::Text("Player Front: %.1f %.1f %.1f", m_player->GetPlayerFront().x, m_player->GetPlayerFront().y, m_player->GetPlayerFront().z);
	ImGui::Text("Player Left: %.1f %.1f %.1f", m_player->GetPlayerLeft().x, m_player->GetPlayerLeft().y, m_player->GetPlayerLeft().z);
	ImGui::End();

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
}

// In here you can handle key presses
// key - Integer that corresponds to numbers in
// https://www.glfw.org/docs/latest/group__keys.html mods - Any modifier
// keys pressed, like shift or control
void Application::onKeyPressed(int key, int mods) 
{
	std::cout << "Key pressed: " << key << std::endl;
	//m_player->ProcessKeyboardInput(key, deltaTime);

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(m_window.getWindowHandle(), true);
		case GLFW_KEY_F1:
			m_window.setMouseCapture(true);
		case GLFW_KEY_F2:
			m_window.setMouseCapture(false);
	}
		
}

// In here you can handle key releases
// key - Integer that corresponds to numbers in
// https://www.glfw.org/docs/latest/group__keys.html mods - Any modifier
// keys pressed, like shift or control
void Application::onKeyReleased(int key, int mods) 
{
	std::cout << "Key released: " << key << std::endl;
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
	std::cout << "onMouseScroll" << std::endl;
}

// If one of the mouse buttons is pressed this function will be called
// button - Integer that corresponds to numbers in
// https://www.glfw.org/docs/latest/group__buttons.html mods - Any modifier
// buttons pressed
void Application::onMouseClicked(int button, int mods) 
{
	std::cout << "Pressed mouse button: " << button << std::endl;
}

// If one of the mouse buttons is released this function will be called
// button - Integer that corresponds to numbers in
// https://www.glfw.org/docs/latest/group__buttons.html mods - Any modifier
// buttons pressed
void Application::onMouseReleased(int button, int mods) 
{
	std::cout << "Released mouse button: " << button << std::endl;
}




