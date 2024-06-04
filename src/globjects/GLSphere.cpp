#include "GLSphere.h"

#include <SDL_stdinc.h>

GLSphere::GLSphere(Shader* shader, Texture* texture)
    : GLObject(shader, texture)
{
    createBufferObject();
    shader->useSetTextureSampler(texture->getUnit());
}

GLSphere::~GLSphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &sphereEBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &texcoordVBO);
}

void GLSphere::createBufferObject() {
    createSphereBuffer(2, 100, 100);
}

struct SphereData {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
};

void createSphere(float radius, int rings, int sectors, SphereData& sphereData)
{
    std::vector<GLfloat>& vertices = sphereData.vertices;
    std::vector<GLfloat>& normals = sphereData.normals;
    std::vector<GLfloat>& texcoords = sphereData.texcoords;
    std::vector<GLushort>& indices = sphereData.indices;

    const float R = 1.0f / (float)(rings - 1);
    const float S = 1.0f / (float)(sectors - 1);

    for (unsigned int r = 0; r < rings; ++r) {
        for (unsigned int s = 0; s < sectors; ++s) {
            const float z = sin(-M_PI_2 + M_PI * r * R);
            const float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            const float y = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            texcoords.push_back(s * S);
            texcoords.push_back(r * R);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
    }

    for (unsigned int r = 0; r < rings - 1; ++r) {
        for (unsigned int s = 0; s < sectors - 1; ++s) {
            indices.push_back(r * sectors + s);
            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + s);

            indices.push_back((r + 1) * sectors + s);
            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + (s + 1));

        }
    }
}

void GLSphere::createSphereBuffer(float radius, int rings, int sectors)
{
    SphereData sphereData;
    createSphere(radius, rings, sectors, sphereData);
    std::vector<GLfloat>& vertices = sphereData.vertices;
    std::vector<GLfloat>& normals = sphereData.normals;
    std::vector<GLfloat>& texcoords = sphereData.texcoords;
    std::vector<GLushort>& indices = sphereData.indices;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GLfloat), &texcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

    indexCount = indices.size();
}

void GLSphere::draw() {
    shader->use();
    texture->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, 0);
}

