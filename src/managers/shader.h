#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// supposed name of texture sampler is "ourTexture"
class Shader {
private:
    unsigned int pid;
    mutable std::unordered_map<std::string, int> uniformLocations;
    static const char* defaultTextureSamplerName;

    int getUniformLocation(const std::string& name) const;

public:
    Shader(unsigned int pid);
    ~Shader();

    void use();

    void setTextureSampler(int textureUnit) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

};