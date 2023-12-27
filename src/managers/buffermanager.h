#pragma once

class BufferManager {
private:
    unsigned int VBO, VAO, EBO;

public:
    BufferManager();
    ~BufferManager();

    void updateBuffers();
};