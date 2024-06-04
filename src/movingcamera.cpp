#include "movingcamera.h"

#include <glm/gtc/matrix_transform.hpp>

void MovingCamera::handleMouseDirection(int xrel, int yrel) {
    yaw += xrel * mouseSensitivity;
    pitch -= yrel * mouseSensitivity;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void MovingCamera::handleKeys(const Uint8* state) {
    if (state[SDL_SCANCODE_W]) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (state[SDL_SCANCODE_S]) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (state[SDL_SCANCODE_A]) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
    }
    if (state[SDL_SCANCODE_D]) {
        cameraPos += glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
    }
}

glm::mat4 MovingCamera::getViewMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 MovingCamera::getCameraPos() const
{
    return cameraPos;
}
