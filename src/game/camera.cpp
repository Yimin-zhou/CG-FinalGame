#include "camera.h"
#include<iostream>

Camera::Camera()
{

}

Camera::Camera(const glm::vec3& initialPosition) :
	m_position(initialPosition), 
	m_worldUp(0.0f, 1.0f, 0.0f),
	m_yaw(0.0f), 
	m_pitch(0.0f), 
	m_distanceFromPlayer(4.0f)
{
	updateCameraVectors();
}

void Camera::FollowPlayer(std::shared_ptr<Player> player)
{
	glm::vec3 followPosition = player->GetPosition();

	if (m_isTopDown) {
		//Top-down camera angle
		m_pitch = -90.0f;
	}

	float offsetX = -m_distanceFromPlayer * sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	float offsetZ = -m_distanceFromPlayer * cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	float offsetY = -m_distanceFromPlayer * sin(glm::radians(m_pitch));
	m_position.x = followPosition.x + offsetX;
	m_position.z = followPosition.z + offsetZ;
	m_position.y = followPosition.y + offsetY + m_distanceFromPlayer; // yoffset

	// Constrain the camera's position to the ground
	if (m_position.y <= 0.2f) m_position.y = 0.2f;
	

	// Update the camera's orientation to look at the player's position
	m_front = glm::normalize(followPosition - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::FollowPlayerAlongBezierCurve(std::shared_ptr<Player> player, const BezierCurve& cameraPath, float t)
{
    // Evaluate the position on the Bézier curve
    glm::vec3 curvePosition = cameraPath.evaluate(t);

    // Calculate the offset vector based on the player's position and the curve position
    glm::vec3 offset = curvePosition - player->GetPosition();

    // Apply the offset to the camera's position
    m_position = player->GetPosition() + offset;

    // Update the camera's orientation to look at the player's position
    m_front = glm::normalize(player->GetPosition() - m_position);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::FollowPlayerAlongCompositeBezierCurve(std::shared_ptr<Player> player, const CompositeBezierCurve& cameraPath, float t)
{
	// Evaluate the position on the composite Bézier curve
	glm::vec3 curvePosition = cameraPath.GetPoint(t);

	// Calculate the offset vector based on the player's position and the curve position
	glm::vec3 offset = curvePosition - player->GetPosition();

	// Apply the offset to the camera's position
	m_position = player->GetPosition() + offset;

	// Update the camera's orientation to look at the player's position
	m_front = glm::normalize(player->GetPosition() - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::FollowBossAlongBezierCurveConstanly(std::shared_ptr<Boss> boss, const BezierCurve& cameraPath, float t)
{
	// Evaluate the position on the Bézier curve
	glm::vec3 curvePosition = cameraPath.evaluate(t);

	// Calculate the offset vector based on the player's position and the curve position
	glm::vec3 offset = curvePosition - boss->GetPosition();

	// Apply the offset to the camera's position
	m_position = boss->GetPosition() + offset;

	// Update the camera's orientation to look at the player's position
	m_front = glm::normalize(boss->GetPosition() - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::Zoom(float offset)
{
	// zoom
	m_distanceFromPlayer -= offset;
	if (m_distanceFromPlayer < 2.0f)
	{
		m_distanceFromPlayer = 2.0f;
	}
}

glm::mat4 Camera::GetViewMatrix() 
{
	return glm::lookAt(m_position, m_position + m_front + m_up * 0.5f, m_up);
}

glm::mat4 Camera::GetPerspectiveMatrix(Window& window)
{
	return glm::perspective(glm::radians(80.0f), (float)window.getWindowSize().x / (float)window.getWindowSize().y, m_near, m_far);
}

glm::mat4 Camera::GetOthoProjMatrix()
{
	return glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::GetOthoViewMatrix()
{
	return glm::lookAt(m_position, glm::vec3(0), m_up);
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

float Camera::GetYaw()
{
	return m_yaw;
}

float Camera::GetPitch()
{
	return m_pitch;
}

glm::vec3 Camera::GetFront()
{
	return m_front;
}

glm::vec3 Camera::GetUp()
{
	return m_up;
}

glm::vec3 Camera::GetRight()
{
	return m_right;
}

void Camera::ProcessKeyboardInput(GLFWwindow* window) 
{
	// Implement keyboard input handling to move the camera
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	// Update the yaw and pitch angles
	m_yaw -= xOffset * m_mouseSensitivity;
	m_pitch += yOffset * m_mouseSensitivity * 0.5f;

	// Constrain the pitch angle to avoid gimbal lock
	if (constrainPitch) 
	{
		if (m_pitch > 89.0f) 
		{
			m_pitch = 89.0f;
		}
		if (m_pitch < -89.0f) 
		{
			m_pitch = -89.0f;
		}
	}

	// Update the camera's front, right, and up vectors based on the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) 
{
	Zoom(yOffset);
}

void Camera::updateCameraVectors() 
{
	// Calculate the new front vector
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)); 
	newFront.y = sin(glm::radians(m_pitch));
	newFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(newFront);
	 
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}