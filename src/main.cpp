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
#include "movingcamera.h"

void handleEvents(SDL_Event& e, SDL_Window* window, bool& quit, MovingCamera& movingCamera);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main(int argc, char* argv[])
{
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    
    const GLubyte* version = glGetString(GL_VERSION);
    printf("OpenGL version: %s\n", version);

    // main part 
    // ---------

    TextureManager textureManager;
    ShaderManager shaderManager; 
    BufferManager bufferManager; 
    MovingCamera movingCamera;

    SDL_Event e;
    bool quit = false;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    glm::mat4 projection = glm::mat4(1.0f);
    GLuint projectionLocation = shaderManager.getUniform("projection");
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    GLuint modelLocation = shaderManager.getUniform("model");
    GLuint viewLocation = shaderManager.getUniform("view");

    while (!quit) {
        frameStart = SDL_GetTicks();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handleEvents(e, window, quit, movingCamera);

        // transformations
        // ---------------
        view = movingCamera.getViewMatrix();
        
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        bufferManager.updateBuffers();
        SDL_GL_SwapWindow(window);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void handleEvents(SDL_Event& e, SDL_Window* window, bool& quit, MovingCamera& movingCamera) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    movingCamera.handleKeys(state);

    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);
    movingCamera.handleMouseDirection(xrel, yrel);
    
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            glViewport(0, 0, e.window.data1, e.window.data2);
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
        }
    }
}
