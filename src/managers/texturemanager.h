#pragma once 

#include <string>
#include <glad/glad.h>

#include "texture.h"

// creates textures. Transfers their control to external
// one texture per texture unit 
class TextureManager {
public:
    TextureManager(const std::string& defaultPath);

    Texture* createTextureDefault(const std::string& name);

private:
    const std::string defaultTexturePath;
    int currentUnit;
    GLint maxTextureUnits;
};
