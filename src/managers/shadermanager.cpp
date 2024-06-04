#include "shadermanager.h"

#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>

#include "shader.h"

ShaderManager::ShaderManager(const std::string& defaultPathFromProjectRoot)
    : defaultShaderPath(std::string(PROJECT_ROOT) + '/' + defaultPathFromProjectRoot + '/'), compileMode(0b11) {}

Shader* ShaderManager::createShaderDefault(const std::string& name) {
    std::string shaderExtensions[3] = { ".vs", ".fs", ".gs" };
    unsigned int shaderTypes[3] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
    unsigned int shaders[3] = { 0, 0, 0 };

    unsigned int pid = glCreateProgram();

    for (int i = 0; i < 3; i++) {
        if (!(compileMode & (1 << i))) {
            continue; 
        }
            
        std::string shaderPath = defaultShaderPath + name + shaderExtensions[i];
        std::ifstream shaderStream(shaderPath, std::ios::in);

        if (!shaderStream.is_open()) {
            std::cout << "Error: " << shaderPath << " was not found" << std::endl;
            continue;
        }

        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        std::string shaderCode = sstr.str();
        shaderStream.close();

        shaders[i] = glCreateShader(shaderTypes[i]);
        char const* shaderSourcePointer = shaderCode.c_str();
        glShaderSource(shaders[i], 1, &shaderSourcePointer, NULL);
        glCompileShader(shaders[i]);

        int success;
        char infoLog[512];
        glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaders[i], 512, NULL, infoLog);
            std::cout << "Error: " << name << shaderExtensions[i] << " has not compiled\n" << infoLog << std::endl;
        }

        glAttachShader(pid, shaders[i]);
    }

    glLinkProgram(pid);

    int success;
    char infoLog[512];
    glGetProgramiv(pid, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pid, 512, NULL, infoLog);
        std::cout << "Error: Shader program has not linked\n" << infoLog << std::endl;
    }

    for (int i = 0; i < 3; i++) {
        if (compileMode & (1 << i)) {
            glDeleteShader(shaders[i]);
        }
    }

    return new Shader(pid);
}
