#pragma once

#include "../managers/texture.h"
#include "../managers/shader.h"

class GLObject {
public:
    Shader* shader;
    Texture* texture;

    GLObject(Shader* shader, Texture* texture) : shader(shader), texture(texture) {}
    virtual ~GLObject() {};

    virtual void draw() = 0;

private:
    virtual void createBufferObject() = 0;

};