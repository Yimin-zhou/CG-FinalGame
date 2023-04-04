   #pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <framework/window.h>

#include "compositeBezier.h"
#include "player.h"

class Camera 
{
public:
	Camera(const glm::vec3& initialPosition);

	Camera();

	// Update the camera's position and orientation based on the player's position and rotation angle
	void FollowPlayer(std::shared_ptr<Player> player);
	void FollowPlayerAlongBezierCurve(std::shared_ptr<Player> player, const BezierCurve& cameraPath, float t);
	void FollowPlayerAlongCompositeBezierCurve(std::shared_ptr<Player> player, const CompositeBezierCurve& cameraPath, float t);

	void Zoom(float offset);

	// Returns the view matrix calculated using the Euler angles and the lookAt matrix
	glm::mat4 GetViewMatrix();
	glm::mat4 GetPerspectiveMatrix(Window& window);
	glm::mat4 GetOthoProjMatrix();
	glm::mat4 GetOthoViewMatrix();
	glm::vec3 GetPosition();

	float GetYaw();
	float GetPitch();
	glm::vec3 GetFront();
	glm::vec3 GetUp();
	glm::vec3 GetRight();

	// Processes input received from any keyboard-like input system
	void ProcessKeyboardInput(GLFWwindow* window);

	// Processes input received from a mouse input system
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);

	// Processes input received from a mouse scroll-wheel event
	void ProcessMouseScroll(float yOffset);

private:
	// Camera attributes
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	float m_distanceFromPlayer;

	// Euler angles
	float m_yaw;
	float m_pitch;

	// Camera options
	float m_mouseSensitivity = 0.4f;
	float m_zoom;
	float m_near = 0.1f;
	float m_far = 300.0f;

	//Top-down options
	bool m_isTopDown = false;

	// Calculates the front vector from the camera's updated Euler angles
	void updateCameraVectors();
};