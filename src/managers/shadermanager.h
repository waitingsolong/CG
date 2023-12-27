#pragma once

#include <glad/glad.h>

class ShaderManager {
private:
    unsigned int shaderProgram;

public:
    ShaderManager();
    ~ShaderManager();

    unsigned int getUniform(const GLchar* name);
};