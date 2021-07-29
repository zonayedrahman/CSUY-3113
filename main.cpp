#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h" 
#include <SDL_mixer.h>

#include "Entity.h"
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#include <vector>



/*
REQUIREMENTS EXPLAINATIONS FOR PROFESSOR

Menu Screen - It is almost identical to level 1 map however, it still fulfills the requirement of being a different "screen" than the levels

3 levels - can be seen above as #include "Levelxx.h"

AI - I have two of "moving" AIs. One moves left and right based on collisions and can be killed from above(aka the witches). However, the other type(AITYPE == PATROL)
is a tentacle that just slithers in place vertically and CANNOT BE KILLED(not from above or sides).


Audio- There is a beautiful, peaceful piece playing in the background, uninturrupted by any gameplay. And the the other requirement is fulfilled through the
player's jump. 


EXTRAS 

1 - I, myself, have trouble beating the first level, during the jump to acquire the key that's beside the tentacle. Therefore, I included a cheat where if you press 
the button "c", your jump power is increased (precisely from 6.0f to 6.5f), which allows you to easily avoid the tentacles. Pressing the cheat button again disables 
the cheat.

2 - If you pay attention to the transitions of the levels, you'll see hint of the previous level (with the door behind you being the most obvious).

Wishes for future changes

I wanted to treat the level 3 exit as a teleporter, meaning the player would disappear due to reaching the goal. However, due to time limit and current code, I 
cannot make that happen. I will try to put this game online once the class is over, so expect to see that change later.


I really enjoyed this project. It is already my college career's favorite. So, thank you for this experience. This probably just became a new hobby I am picking up.

*/




Scene* currentScene;
Scene* sceneList[4];


void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


int lives = 3;


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music* music;
Mix_Chunk* jumpNoise;




void Initialize() {
    //SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("PROJECT 5: The Haunted", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    music = Mix_LoadMUS("music2.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    Mix_PlayMusic(music, -1);

    jumpNoise = Mix_LoadWAV("jump2.wav");
    

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new MainMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);



}


void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_c:
                if (currentScene->state.player->jumpPower == 6.0) {
                    currentScene->state.player->jumpPower = 6.5;
                }
                else if (currentScene->state.player->jumpPower == 6.5) {
                    currentScene->state.player->jumpPower = 6.0;
                }
                break;
                //currentScene->state.player->jumpPower = 6.
            case SDLK_SPACE:
                if (currentScene->state.player->collidedBottom) {
                    currentScene->state.player->jump = true;
                    Mix_PlayChannel(-1, jumpNoise, 0);
                }
                break;
            case SDLK_RETURN:
                if (currentScene == sceneList[0]) {
                    SwitchToScene(sceneList[1]);
                }
                if (lives <= 0) {
                    SwitchToScene(sceneList[0]);
                    lives = 3;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0;


void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }


    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;


    if (currentScene->state.player->decrease_life) {
        lives -= 1;
        currentScene->state.player->decrease_life = false;
        if (lives <= 0) {
            currentScene->state.player->isActive = false;
            currentScene->state.player->gameover = true;
        }
    }


    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);



    //for (int i = 0; i < ENEMY_COUNT; i++) {
    //    state.enemies[i].Render(&program);
    //}


    currentScene->Render(&program);
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    if (currentScene != sceneList[0]) {
    Util::DrawText(&program, fontTextureID, "Lives: ", .25, 0.02f, glm::vec3(0.5, -0.8f, 0));

    //std::string lives = std::to_string(currentScene->state.player->lives);
    Util::DrawText(&program, fontTextureID, std::to_string(lives), .25, 0.02f, glm::vec3(2.2, -0.8f, 0));
    }


    if (currentScene->state.player->gameover) {
        Util::DrawText(&program, fontTextureID, "You Lose!", .35, 0.02f, glm::vec3(4.2, -2.75f, 0));
        Util::DrawText(&program, fontTextureID, "Press Enter to Play Again!", .20, 0.02f, glm::vec3(2.2, -4.55f, 0));
    }
    //Util::DrawText(&program, fontTextureID, "Press Enter to Play Again!", .20, 0.02f, glm::vec3(2.2, -4.55f, 0));
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}