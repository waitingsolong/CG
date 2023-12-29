#include "shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

const char* Shader::defaultTextureSamplerName = "ourTexture";

Shader::Shader(unsigned int pid) : pid(pid) {}

Shader::~Shader() {
    glDeleteProgram(pid);
}

void Shader::use() {
    glUseProgram(pid);
}

int Shader::getUniformLocation(const std::string& name) const {
    if (uniformLocations.contains(name)) {
        return uniformLocations[name];
    }
    else {
        int location = glGetUniformLocation(pid, name.c_str());
        uniformLocations[name] = location;
        return location;
    }
}

void Shader::setTextureSampler(int textureUnit) const
{
    glUniform1i(glGetUniformLocation(pid, defaultTextureSamplerName), textureUnit);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}