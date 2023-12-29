#include "TextureManager.h"

#include <iostream>
#include <stb_image.h>

#include "texture.h"

TextureManager::TextureManager(const std::string& defaultPathFromSourceRoot)
    : defaultTexturePath(std::string(PROJECT_ROOT) + '/' + defaultPathFromSourceRoot + '/'), currentUnit(0)
{
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
}

Texture* TextureManager::createTextureDefault(const std::string& name) {
    if (defaultTexturePath.empty()) {
        std::cout << "Error: Default texture path is not set." << std::endl;
        return nullptr;
    }

    if (currentUnit >= maxTextureUnits) {
        std::cout << "Error: The number of textures exceeds the maximum number of texture units (" << maxTextureUnits << ")." << std::endl;
        return nullptr;
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string texture_path = defaultTexturePath + std::string(name) + ".jpg";
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Error: Failed to load texture " << name << ".jpg" << std::endl;
        return nullptr;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return new Texture(textureId, name, currentUnit++);
}
