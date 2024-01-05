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
#include "movingcamera.h"
#include "globjects/GLCube.h"
#include "globjects/GLSphere.h"
#include "globjects/GLSkybox.h"

void handleEvents(SDL_Event& e, SDL_Window* window, bool& quit, MovingCamera& movingCamera);

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

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

    // setups 
    // ------

    TextureManager textureManager("assets/textures");
    textureManager.setDefaultCubeboxPath("assets/cubeboxes");
    ShaderManager shaderManager("assets/shaders");
    MovingCamera movingCamera;

    // objects 
    // -------

    std::vector<GLObject*> glObjects;

    GLCube* cube = new GLCube(
        shaderManager.createShaderDefault("cube"),
        textureManager.createTextureDefault("xfile")
    ); 
    glObjects.push_back(cube);

    GLSphere* sphere = new GLSphere(
        shaderManager.createShaderDefault("sphere"),
        textureManager.createTextureDefault("sun")
    );
    glObjects.push_back(sphere);

    // transformations 
    // ---------------

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    for (auto& object : glObjects) {
        object->shader->useSetMat4("projection", projection);
    }

    glm::vec3 lightPos(0.0f);
    
    glm::mat4 model(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.5f));
    cube->shader->useSetMat4("model", model);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 15.0f, 15.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    sphere->shader->useSetMat4("model", model);
    
    sphere->shader->setVec3("lightPos", lightPos);

    // main loop 
    // ---------

    SDL_Event e;
    bool quit = false;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    glm::mat4 view;

    float speed = 0.0015f; 
    float amplitude = 5.0f; 

    while (!quit) {
        frameStart = SDL_GetTicks();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handleEvents(e, window, quit, movingCamera);

        /////////////////////////////////////

        sphere->shader->setVec3("viewPos", movingCamera.getCameraPos());

        view = movingCamera.getViewMatrix();

        // sphere movement 
        float offset = amplitude * sin(speed * frameStart);
        glm::vec3 spherePos(offset, 0.0f, 0.0f);
        model = glm::translate(glm::mat4(1.0f), spherePos);
        sphere->shader->useSetMat4("model", model);

        for (auto& object : glObjects) {
            object->shader->useSetMat4("view", view);
            object->draw();
        }

        ////////////////////////////////////

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
            else if (e.key.keysym.sym == SDLK_q) {
                quit = true;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
        }
    }
}
