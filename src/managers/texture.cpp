#include "texture.h"

#include <glad/glad.h>

Texture::Texture(unsigned int id, const std::string& name, int unit)
    : id(id), name(name), unit(unit) {}

void Texture::use() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

int Texture::getUnit() const
{
    return unit;
}
