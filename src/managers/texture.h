#pragma once

#include <string>

class Texture {
public:
    Texture(unsigned int id, const std::string&, int unit);

    void use();

    int getUnit() const;

private:
    const unsigned int id;
    const std::string name;
    const int unit;
};