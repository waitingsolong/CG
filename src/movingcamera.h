#pragma once 

#include <glm/glm.hpp>
#include <SDL.h>

class MovingCamera {
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    float cameraSpeed;
    float yaw;
    float pitch;
    float mouseSensitivity;

    MovingCamera() : cameraPos(0.0f, 0.0f, 0.0f), cameraFront(0.0f, 0.0f, -1.0f),
        cameraSpeed(0.05f), yaw(0.0f), pitch(0.0f), mouseSensitivity(0.1f) {}

    void handleMouseDirection(int xrel, int yrel);

    void handleKeys(const Uint8* state);

    glm::mat4 getViewMatrix();
    glm::vec3 getCameraPos() const;
};
