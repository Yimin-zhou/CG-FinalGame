#include "TopDownCamera.h"
#include <iostream>


TopDownCamera::TopDownCamera(const glm::vec3& initialPosition)
    : m_position(initialPosition), 
    m_worldUp(0.0f, 1.0f, 0.0f),
    m_up(0.0f, 0.0f, 1.0f),
    m_right(-1.0f, 0.0f, 0.0f),
    m_front(0.0f, -1.0f, 0.0f),
    m_yaw(0.0f),
    m_pitch(0.0f), 
    m_distanceFromPlayer(2.5f)
{
}

TopDownCamera::TopDownCamera() : Camera()
{
}

void TopDownCamera::FollowPlayer(std::shared_ptr<Player> player) {
    glm::vec3 followPosition = player->GetPosition();

    // Calculate X and Z offsets based on the yaw angle.
    float offsetX = -m_distanceFromPlayer * sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    float offsetZ = -m_distanceFromPlayer * cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    // update the camera's position.
    m_position.x = followPosition.x + offsetX;
    m_position.z = followPosition.z + offsetZ;

    // Update the camera's orientation to look at the player's position.
    m_front = glm::normalize(followPosition - m_position);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 TopDownCamera::GetViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 TopDownCamera::GetPerspectiveMatrix(Window& window) {
    return glm::perspective(glm::radians(m_zoom), window.getAspectRatio(), m_near, m_far );
}

void TopDownCamera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    //m_yaw -= xOffset * m_mouseSensitivity;
}


void TopDownCamera::updateCameraVectors() 
{
}


glm::vec3 TopDownCamera::GetPosition()
{
    return m_position;
}

float TopDownCamera::GetYaw()
{
    return m_yaw;
}

float TopDownCamera::GetPitch()
{
    return m_pitch;
}

glm::vec3 TopDownCamera::GetFront()
{
    return m_front;
}

glm::vec3 TopDownCamera::GetUp()
{
    return m_up;
}

glm::vec3 TopDownCamera::GetRight()
{
    return m_right;
}


glm::mat4 TopDownCamera::GetOthoProjMatrix()
{
    return glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 100.0f);
}

glm::mat4 TopDownCamera::GetOthoViewMatrix()
{
    return glm::lookAt(m_position, glm::vec3(0), m_up);
}