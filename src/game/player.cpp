#include "player.h"
#include <iostream>

Player::Player(glm::vec3 startPosition, float playerSpeed) :
	position(startPosition),
	speed(playerSpeed),
	health(100),
	shootingInterval(0.2f),
	shootingTimer(0.0f),
	abilityTimer(0.0f),
	abilityInterval(10.0f),
	m_front(glm::vec3(0.0f, 0.0f, 1.0f)),
	m_left(glm::vec3(1.0f, 0.0f, 0.0f)),
	m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_yaw(0.0f),
	modelMatrix(glm::mat4(1.0f)),
	collider(startPosition, 1.0f)
{

}

Player::Player()
{

}

void Player::Update(float deltaTime)
{
	SetShootingTimer(deltaTime);

	collider.SetPosition(position);
	
	// Apply knockback
	if (glm::length(knockback) > 0) {
		position.x += knockback.x;
		position.z += knockback.z;
		knockback *= 0.96;
		if (glm::length(knockback) < 0.01f) {
			knockback = glm::vec3(0.0f);
		}
	}

	SetAbilityTimer(deltaTime);

}

void Player::ApplyKnockback(const glm::vec3& knockbackDirection, float knockbackForce)
{
	glm::vec3 normalizedDirection = glm::normalize(knockbackDirection);
	knockback = normalizedDirection * knockbackForce;
}


void Player::MoveForward(float deltaTime)
{
	position.x += sin(glm::radians(m_yaw)) * speed * deltaTime;
	position.z += cos(glm::radians(m_yaw)) * speed * deltaTime;
}

void Player::MoveBackward(float deltaTime)
{
	position.x -= sin(glm::radians(m_yaw)) * speed * deltaTime;
	position.z -= cos(glm::radians(m_yaw)) * speed * deltaTime;
}

void Player::MoveLeft(float deltaTime)
{
	position.x -= -cos(glm::radians(m_yaw)) * speed * deltaTime;
	position.z += -sin(glm::radians(m_yaw)) * speed * deltaTime;
}

void Player::MoveRight(float deltaTime)
{
	position.x += -cos(glm::radians(m_yaw)) * speed * deltaTime;
	position.z -= -sin(glm::radians(m_yaw)) * speed * deltaTime;
}

void Player::RotateLeft(float deltaTime)
{
	m_yaw -= speed * deltaTime;
}

void Player::RotateRight(float deltaTime)
{
	m_yaw += speed * deltaTime;
}

float Player::GetYaw()
{
	return m_yaw;
}

uint32_t Player::GetHealth()
{
	return health;
}

void Player::SetYaw(float yaw)
{
	m_yaw = yaw;
	updateFront();
	updateLeft();
	updateUp();
}

void Player::SetShootingTimer(float deltaTime)
{
	shootingTimer -= deltaTime;
}

void Player::SetAbilityTimer(float deltaTime) 
{
	abilityTimer -= deltaTime;
	if (abilityTimer <= 0) {
		is_abilityOn = false;
	}
}

glm::vec3 Player::GetPlayerUp()
{
	return m_up;
}

glm::vec3 Player::GetPlayerFront()
{
	return m_front;
}

void Player::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
	float m_mouseSensitivity = 0.4f;
	m_yaw -= xOffset * m_mouseSensitivity;
}

glm::vec3 Player::GetPlayerLeft()
{
	return m_left;
}

void Player::ProcessKeyboardInput(int key, float deltaTime)
{
	if (key == 87) 
	{
		MoveForward(deltaTime);
	}
	else if (key == 83) 
	{
		MoveBackward(deltaTime);
	}
	else if (key == 65) 
	{
		MoveLeft(deltaTime);
	}
	else if (key == 68) 
	{
		MoveRight(deltaTime);
	}
}

glm::vec3 Player::GetPosition()
{
	return position;
}

void Player::updateFront()
{
	// move front with position
	m_front = glm::vec3(0.0f, 0.0f, 1.0f);
	m_front.x = sin(glm::radians(m_yaw));
	m_front.z = cos(glm::radians(m_yaw));
	m_front = glm::normalize(m_front);
}

void Player::updateLeft()
{
	m_left = glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_front));
}

void Player::updateUp()
{
	m_up = glm::normalize(glm::cross(m_front, m_left));
}

glm::vec3 Player::GetMatrixPosition() {
	return glm::vec3(modelMatrix[3]);
}

glm::vec3 Player::GetMatrixUp() {
	return glm::normalize(glm::vec3(modelMatrix[1]));
}

glm::vec3 Player::GetMatrixFront() {
	return glm::normalize(-glm::vec3(modelMatrix[2]));
}

glm::vec3 Player::GetMatrixLeft() {
	return glm::normalize(glm::vec3(modelMatrix[0]));
}

void Player::MatrixMoveForward(float deltaTime) {
	glm::vec3 movement = -GetMatrixFront() * speed * deltaTime;
	glm::mat4 model = glm::translate(modelMatrix, movement);
	position = glm::vec3(model * glm::vec4(position, 1.0));
}

void Player::MatrixMoveBackward(float deltaTime) {
	glm::vec3 movement = GetMatrixFront() * speed * deltaTime;
	glm::mat4 model = glm::translate(modelMatrix, movement);

	position = glm::vec3(model * glm::vec4(position, 1.0));
}

void Player::MatrixMoveLeft(float deltaTime) {
	glm::vec3 movement = GetMatrixLeft() * speed * deltaTime;
	glm::mat4 model = glm::translate(modelMatrix, movement);

	position = glm::vec3(model * glm::vec4(position, 1.0));
}

void Player::MatrixMoveRight(float deltaTime) {
	glm::vec3 movement = -GetMatrixLeft() * speed * deltaTime;
	glm::mat4 model = glm::translate(modelMatrix, movement);

	position = glm::vec3(model * glm::vec4(position, 1.0));
}

void Player::MatrixRotateLeft(float deltaTime) {
	float rotationAngle = speed * deltaTime;
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), GetPlayerUp());
	m_yaw += rotationAngle;
}

void Player::MatrixRotateRight(float deltaTime) {
	float rotationAngle = -speed * deltaTime;
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), GetPlayerUp());
	m_yaw += rotationAngle;
}
