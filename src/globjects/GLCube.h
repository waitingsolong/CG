#pragma once

#include <glad/glad.h>

#include "GLObject.h"

class GLCube : public GLObject {
private:
    unsigned int VAO, VBO;

    void createBufferObject() override;

public:
    GLCube(Shader* shader, Texture* texture);
    ~GLCube() override;

    void draw() override;
};