#pragma once

#include <glad/glad.h>
#include <string>

#include "shader.h"

class ShaderManager {
private:
    const std::string defaultShaderPath;
    unsigned int compileMode;

public:
    ShaderManager(const std::string& defaultPathFromSourceRoot);

    Shader* createShaderDefault(const std::string& name);
};
