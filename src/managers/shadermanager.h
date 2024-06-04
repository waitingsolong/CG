#pragma once

#include <glad/glad.h>
#include <string>

#include "shader.h"

class ShaderManager {
private:
    const std::string defaultShaderPath;
    unsigned int compileMode;

public:
    ShaderManager(const std::string& defaultPathFromProjectRoot);

    Shader* createShaderDefault(const std::string& name);
};
