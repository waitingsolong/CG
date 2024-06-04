#include "TextureManager.h"

#include <iostream>
#include <stb_image.h>
#include <vector>

#include "texture.h"

TextureManager::TextureManager(const std::string& defaultPathFromProjectRoot)
    : defaultTexturePath(std::string(PROJECT_ROOT) + '/' + defaultPathFromProjectRoot + '/'), currentUnit(0)
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

void TextureManager::setDefaultCubeboxPath(const std::string& defaultPathFromProjectRoot)
{
    defaultCubeboxPath = std::string(PROJECT_ROOT) + '/' + defaultPathFromProjectRoot + '/';
}

// supposed 6 pictures have names [name]_top etc.
Texture* TextureManager::createCubeboxDefault(const std::string& name)
{
    if (defaultCubeboxPath.empty()) {
        std::cout << "Error: Default texture path is not set." << std::endl;
        return nullptr;
    }

    if (currentUnit >= maxTextureUnits) {
        std::cout << "Error: The number of textures exceeds the maximum number of texture units (" << maxTextureUnits << ")." << std::endl;
        return nullptr;
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::vector<std::string> namePostfixes = { "right", "left", "top", "bottom", "front", "back" };

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data; 
    for (unsigned int i = 0; i < 6; i++)
    {
        std::string texture_path = defaultCubeboxPath + std::string(name) + '_' + namePostfixes[i] + ".png";
        data = nullptr; 
        data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Error: Failed to load cubemap texture " << texture_path << std::endl;
        }
    }
    stbi_set_flip_vertically_on_load(true);
    stbi_image_free(data);

    return new Texture(textureId, name, currentUnit++);
}
