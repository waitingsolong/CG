#pragma once 

#include <string>
#include <glad/glad.h>

#include "texture.h"

// creates textures. Transfers their control to external
// one texture per texture unit 
class TextureManager {
public:
    TextureManager(const std::string& defaultPathFromProjectRoot);

    Texture* createTextureDefault(const std::string& name);

    void setDefaultCubeboxPath(const std::string& defaultPathFromProjectRoot);
    Texture* createCubeboxDefault(const std::string& name);

private:
    const std::string defaultTexturePath;
    std::string defaultCubeboxPath;
    int currentUnit;
    GLint maxTextureUnits;
};
