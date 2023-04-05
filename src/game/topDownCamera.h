#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <framework/window.h>
#include "camera.h"
#include "player.h"


class TopDownCamera : public Camera {
public:
    TopDownCamera(const glm::vec3& initialPosition);
	TopDownCamera();
	
	void Zoom(float offset);
	glm::mat4 GetViewMatrix();

    // Update the camera's position and orientation based on the player's position for the top-down angle
    void FollowPlayer(std::shared_ptr<Player> player);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);

	glm::vec3 GetPosition();
	glm::vec3 GetFront();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
	float GetYaw();
	float GetPitch();

	glm::mat4 GetOrthoMatrix(Window& window);
	glm::mat4 GetOthoViewMatrix();

private:
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
	float m_far = 200.0f;

	//Top-down options
	bool m_isTopDown = false;

	// Calculates the front vector from the camera's updated Euler angles
	void updateCameraVectors();
};