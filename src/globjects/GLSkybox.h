#pragma once

#include "GLObject.h"

class GLSkybox : public GLObject {
private:
    unsigned int VAO, VBO;

    void createBufferObject() override;

public:
    GLSkybox(Shader* shader, Texture* texture);
    ~GLSkybox() override;

    void draw() override;
};
