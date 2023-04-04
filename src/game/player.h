#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "render/model.h"
#include "collider.h"


class Player 
{
public:
	std::shared_ptr<Model> model;
	glm::vec3 position;
	float speed;
	uint32_t health;
	Collider collider;
	float shootingInterval;
	float shootingTimer;
	glm::vec3 knockback = glm::vec3(0.0f);

	Player(glm::vec3 startPosition, float playerSpeed);
	Player();

	void Update(float deltaTime);

	void ApplyKnockback(const glm::vec3& knockbackDirection, float knockbackForce);

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

	glm::vec3 GetMatrixPosition();
	glm::vec3 GetMatrixUp();
	glm::vec3 GetMatrixFront();
	glm::vec3 GetMatrixLeft();
	void MatrixMoveForward(float deltaTime);
	void MatrixMoveBackward(float deltaTime);
	void MatrixMoveLeft(float deltaTime);
	void MatrixMoveRight(float deltaTime);
	void MatrixRotateRight(float deltaTime);
	void MatrixRotateLeft(float deltaTime);

	void ProcessKeyboardInput(int key, float deltaTime);

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);

	glm::vec3 GetPosition();

private:
	double m_yaw = 0.0f;

	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::vec3 m_left;

	glm::mat4 modelMatrix;

	void updateUp();
	void updateFront();
	void updateLeft();
};