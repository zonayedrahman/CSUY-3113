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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include <vector>


#define ENEMY_COUNT 3
#define PLATFORM_COUNT 50



struct GameState {
    Entity* player;
    Entity* enemies;
    Entity* platforms;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("PROJECT 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4.5f, 0.0f, 0.0f);
    state.player->movement = glm::vec3(0);
    state.player->accelaration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.player->speed = 2.0f;
    state.player->textureID = LoadTexture("player.png");

    state.player->animRight = new int[4]{ 143, 144, 145, 146  };
    state.player->animLeft = new int[4]{ 117, 118, 119, 120 };
    state.player->animUp = new int[1]{ 0 };
    state.player->animDown = new int[1]{ 26 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 13;
    state.player->animRows = 21;

    state.player->height = 0.8f;
    state.player->width = 0.5f;
    state.player->jumpPower = 7.5f;



    // initialize platforms
    state.platforms = new Entity[PLATFORM_COUNT];
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
    }
    
    GLuint platformTextureID = LoadTexture("sprite_0.png");


    // bottom 
    for (int i = 0; i < 9; i++) {
        state.platforms[i].position = glm::vec3(-4.5 + i, -3.5f, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    state.platforms[9].position = glm::vec3(4.5f, -3.5f, 0.0f);
    state.platforms[9].textureID = platformTextureID;
    

    // left edge
    for (int i = 10; i < 16; i++) {
        state.platforms[i].position = glm::vec3(-5.3, -12.5 + i, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    // right edge
    state.platforms[16].position = glm::vec3(4.8f, -2.5f, 0.0f);
    state.platforms[17].position = glm::vec3(4.8f, -1.5f, 0.0f);
    state.platforms[18].position = glm::vec3(4.8f, -0.5f, 0.0f);

    for (int i = 16; i < 18; i++) {
        state.platforms[i].textureID = platformTextureID;
    }

    state.platforms[15].textureID = platformTextureID;
    state.platforms[18].textureID = platformTextureID;


    // specifics
    // 
    // bottom right
    for (int i = 19; i < 22; i++) {
        state.platforms[i].position = glm::vec3(-18.3 + i, -1.2f, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    // middle left
    for (int i = 22; i < 25; i++) {
        state.platforms[i].position = glm::vec3(-25.6 + i, -0.5f, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    state.platforms[22].textureID = platformTextureID;
    state.platforms[25].position = glm::vec3(-3.6f, 0.5f, 0.0f);
    state.platforms[25].textureID = platformTextureID;



    // middle top
    state.platforms[26].position = glm::vec3(-1.5f, 2.35f, 0.0f);
    state.platforms[27].position = glm::vec3(-0.5f, 2.35f, 0.0f);
    // later added
    state.platforms[48].position = glm::vec3(0.5f, 2.35f, 0.0f);
    state.platforms[48].textureID = platformTextureID;

    state.platforms[49].position = glm::vec3(1.0, 1.35f, 0.0f);
    state.platforms[49].textureID = platformTextureID;

    //state.platforms[50].position = glm::vec3(-0.5, 1.5f, 0.0f);
    //state.platforms[50].textureID = platform2TextureID;

    //state.platforms[51].position = glm::vec3(-1.5, 1.5f, 0.0f);
    //state.platforms[51].textureID = platform2TextureID;

    for (int i = 28; i < 31; i++) {
        state.platforms[i].position = glm::vec3(-26.0 + i, 1.35f, 0.0f);
    }

    for (int i = 26; i < 31; i++) {
        state.platforms[i].textureID = platformTextureID;
    }

    state.platforms[30].isActive = false;


    // blocking off top boundary 
    for (int i = 31; i < 43; i++) {
        state.platforms[i].position = glm::vec3(-36.3 + i, 4.25f, 0.0f);
        state.platforms[i].textureID = platformTextureID;
        //state.platforms[i].height = 0.7;
    }


    // blocking off right boundary
    for (int i = 43; i < 48; i++) {
        state.platforms[i].position = glm::vec3(5.5, -43.25 + i, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    // updates platforms once
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
    }


    // initialize enemies
    state.enemies = new Entity[ENEMY_COUNT];

    state.enemies[0].accelaration = glm::vec3(0.0f, -9.81f, 0.0f);
    //state.enemies[2].accelaration = glm::vec3(0.0f, -9.81f, 0.0f);

    GLuint enemyTextureID = LoadTexture("golem-walk.png");

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        //state.enemies[i].animRight = new int[7]{ 21,22,23,24,25,26,27 };
        //state.enemies[i].animLeft = new int[7]{7,8,9,10,11, 12, 13};
        //state.enemies[i].animUp = new int[7]{0, 1,2,3,4,5,6};
        state.enemies[i].animDown = new int[7]{14,15,16,17,18,19,20};


        state.enemies[i].animIndices = state.enemies[i].animDown;
        state.enemies[i].animFrames = 7;
        state.enemies[i].animIndex = 7;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 7;
        state.enemies[i].animRows = 4;

    }
    //state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(0.5f, -2.0f, 0.0f);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.5f;


    //state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(-3.63f, 1.5f, 0.0f);
    state.enemies[1].speed = 1.5f;
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = WALKING;



    //state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(4.5f, 2.6f, 0.0f);
    state.enemies[2].speed = 1.5f;
    state.enemies[2].aiType = PATROL;
    state.enemies[2].aiState = WALKING;
    

}


void ProcessInput() {

    state.player->movement = glm::vec3(0);

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

            case SDLK_SPACE:
                if(state.player->collidedBottom)  { 
                    state.player->jump = true;
                }
                break;
            case SDLK_r:
                if (state.player->isActive == false) {
                    state.player->isActive = true;
                    state.player->gameover = false;
                    //state.player->position = glm::vec3(-4.5f, 0.0f, 0.0f);
                }
                state.player->position = glm::vec3(-4.5f, 0.0f, 0.0f);
                for (int i = 0; i < ENEMY_COUNT; i++) {
                    state.enemies[i].isActive = true;
                }
                state.enemies[0].position = glm::vec3(0.5f, -2.0f, 0.0f);
                state.enemies[1].position = glm::vec3(-3.63f, 1.5f, 0.0f);
                state.enemies[2].position = glm::vec3(4.5f, 2.6f, 0.0f);

            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }


    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
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
        state.player->Update(FIXED_TIMESTEP, NULL, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);

        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, NULL, ENEMY_COUNT);
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    //for (int i = 0; i < ENEMY_COUNT; i++)
    //{
    //    state.enemies[i].Update(deltaTime);
    //}


}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position)
{
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (unsigned i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });

    } // end of for loop
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);

}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);



    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++)   {
        state.enemies[i].Render(&program);
    }


    state.player->Render(&program);


    GLuint font_textureID = LoadTexture("font2.png");

    //DrawText(&program, font_textureID, "Defeat the Enemies and Find the Exit!", .25, 0.02f, glm::vec3(-4.7f, 0.0f, 0.0f));
    if (state.player->gameover) {
        DrawText(&program, font_textureID, "Game Over! Press R to Play Again!", .25, 0.02f, glm::vec3(-4.2f, 0.0f, 0.0f));
    }

    bool noEnemy = true;
    bool allEnemy = true;

    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive) {
            noEnemy = false;
            break;
        }
    }

    if (noEnemy) {
        state.player->isActive = false;
        DrawText(&program, font_textureID, "Game Won! Press R to Play Again!", .25, 0.02f, glm::vec3(-4.2f, 0.0f, 0.0f));
    }
    //DrawText(&program, font_textureID, "Game Won! Press R to Play Again", .25, 0.02f, glm::vec3(-4.2f, 0.0f, 0.0f));

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
        Render();
    }

    Shutdown();
    return 0;
}