#define SDL_MAIN_HANDLED

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "managers/texturemanager.h"
#include "managers/shadermanager.h"
#include "managers/buffermanager.h"

void handleEvents(SDL_Event& e, SDL_Window* window, bool& quit);

int main(int argc, char* argv[])
{
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    printf("OpenGL version: %s\n", version);

    TextureManager textureManager;
    ShaderManager shaderManager; 
    BufferManager bufferManager; 

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        handleEvents(e, window, quit);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float timeValue = (float)SDL_GetTicks64() / 1000.0f;
        float xValue = sin(timeValue) * 0.5f;
        float yValue = sin(timeValue) * cos(timeValue) * 0.5f;
        transform = glm::translate(transform, glm::vec3(xValue, yValue, 0.0f));
        transform = glm::rotate(transform, timeValue, glm::vec3(1.0f, 1.0f, 0.0f));
        
        unsigned int transformLoc = shaderManager.getUniform("transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        bufferManager.updateBuffers();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void handleEvents(SDL_Event& e, SDL_Window* window, bool& quit) {
    while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
        }
}
