#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "render/model.h"


class Player 
{
public:
	std::shared_ptr<Model> model;
	glm::vec3 position;
	float speed;
	uint32_t health;

	float shootingInterval;
	float shootingTimer;

	Player(glm::vec3 startPosition, float playerSpeed);
	Player();

	void Update(float deltaTime);

	void MoveForward(float deltaTime);

	void MoveBackward(float deltaTime);

	void MoveLeft(float deltaTime);

	void MoveRight(float deltaTime);

	void RotateLeft(float deltaTime);

	void RotateRight(float deltaTime);

	float GetYaw();

	uint32_t GetHealth();

	void SetYaw(float yaw);
	void SetShootingTimer(float deltaTime);

	glm::vec3 GetPlayerUp();
	glm::vec3 GetPlayerFront();
	glm::vec3 GetPlayerLeft();

	void ProcessKeyboardInput(int key, float deltaTime);

	glm::vec3 GetPosition();

private:
	double m_yaw = 0.0f;

	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::vec3 m_left;

	void updateUp();
	void updateFront();
	void updateLeft();
};