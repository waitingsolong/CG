#pragma once

#include <glad/glad.h>

#include "GLObject.h"

class GLSphere : public GLObject {
private:
    unsigned int VAO, VBO, sphereEBO, normalVBO, texcoordVBO, indexCount;

    void createBufferObject() override;
    void createSphereBuffer(float radius, int rings, int sectors);

public:
    GLSphere(Shader* shader, Texture* texture);
    ~GLSphere() override;

    void draw() override;
};