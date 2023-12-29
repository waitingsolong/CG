#pragma once

#include "managers/texture.h"
#include "managers/shader.h"

class GLObject {
public:
    Shader* shader;
    Texture* texture;

    GLObject(Shader* shader, Texture* texture) : shader(shader), texture(texture) {}

    virtual ~GLObject() {};

    virtual void createBufferObject() = 0;
    virtual void draw() = 0;
};