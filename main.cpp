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


SDL_Window* displayWindow;
bool gameIsRunning = true;

struct GameState {
    Entity* player;
    Entity* goal;
    Entity* obstacles;
};

GameState state;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;


GLuint font_textureID, death_star_textureID;
GLuint obstacle_textureID;
//GLuint darth_vader_textureID, luke_textureID;

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


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}



#define OBSTACLE_COUNT 28

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 3- Lunar Ladder!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // player entity
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0.3f, 3.3f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0.0f, -1.5f, 0.0f);
    state.player->speed = 2.0f;
    state.player->textureID = LoadTexture("player.png");

    // goal_platform entity
    state.goal = new Entity();
    state.goal->entityType = GOAL;
    state.goal->position = glm::vec3(1.5f, -3.3f, 0);
    state.goal->textureID = LoadTexture("tileset.png");


    GLuint fontTextureID = LoadTexture("font2.png");
    // obstacles
    state.obstacles = new Entity[OBSTACLE_COUNT];
    state.obstacles->entityType = OBSTACLE;


    GLuint obstacleTextureID = LoadTexture("tileset.png");


    state.obstacles[0].textureID = obstacleTextureID;
    state.obstacles[0].position = glm::vec3(-4.8f, 3.3f, 0);

    state.obstacles[1].textureID = obstacleTextureID;
    state.obstacles[1].position = glm::vec3(-4.8f, 2.3f, 0);

    state.obstacles[2].textureID = obstacleTextureID;
    state.obstacles[2].position = glm::vec3(-4.8f, 1.3f, 0);

    state.obstacles[3].textureID = obstacleTextureID;
    state.obstacles[3].position = glm::vec3(-4.8f, 0.3f, 0);

    state.obstacles[4].textureID = obstacleTextureID;
    state.obstacles[4].position = glm::vec3(-4.8f, -0.3f, 0);

    state.obstacles[5].textureID = obstacleTextureID;
    state.obstacles[5].position = glm::vec3(-4.8f, -1.3f, 0);

    state.obstacles[6].textureID = obstacleTextureID;
    state.obstacles[6].position = glm::vec3(-4.8f, -2.3f, 0);

    state.obstacles[7].textureID = obstacleTextureID;
    state.obstacles[7].position = glm::vec3(-4.8f, -3.3f, 0);

    state.obstacles[8].textureID = obstacleTextureID;
    state.obstacles[8].position = glm::vec3(-3.8f, -3.3f, 0);

    state.obstacles[9].textureID = obstacleTextureID;
    state.obstacles[9].position = glm::vec3(-2.8f, -3.3f, 0);

    state.obstacles[10].textureID = obstacleTextureID;
    state.obstacles[10].position = glm::vec3(-1.8f, -3.3f, 0);

    state.obstacles[11].textureID = obstacleTextureID;
    state.obstacles[11].position = glm::vec3(-0.8f, -3.3f, 0);

    state.obstacles[12].textureID = obstacleTextureID;
    state.obstacles[12].position = glm::vec3(-0.5f, -3.3f, 0);


    // right side

    state.obstacles[13].textureID = obstacleTextureID;
    state.obstacles[13].position = glm::vec3(4.8f, 3.3f, 0);

    state.obstacles[14].textureID = obstacleTextureID;
    state.obstacles[14].position = glm::vec3(4.8f, 2.3f, 0);

    state.obstacles[15].textureID = obstacleTextureID;
    state.obstacles[15].position = glm::vec3(4.8f, 1.3f, 0);

    state.obstacles[16].textureID = obstacleTextureID;
    state.obstacles[16].position = glm::vec3(4.8f, 0.3f, 0);

    state.obstacles[17].textureID = obstacleTextureID;
    state.obstacles[17].position = glm::vec3(4.8f, -0.3f, 0);

    state.obstacles[18].textureID = obstacleTextureID;
    state.obstacles[18].position = glm::vec3(4.8f, -1.3f, 0);

    state.obstacles[19].textureID = obstacleTextureID;
    state.obstacles[19].position = glm::vec3(4.8f, -2.3f, 0);

    state.obstacles[20].textureID = obstacleTextureID;
    state.obstacles[20].position = glm::vec3(4.8f, -3.3f, 0);

    state.obstacles[21].textureID = obstacleTextureID;
    state.obstacles[21].position = glm::vec3(3.8f, -3.3f, 0);



    // unique obstacles
    state.obstacles[22].textureID = obstacleTextureID;
    state.obstacles[22].position = glm::vec3(-4.0f, 1.3f, 0);

    state.obstacles[23].textureID = obstacleTextureID;
    state.obstacles[23].position = glm::vec3(-3.0f, 1.3f, 0);


    state.obstacles[24].textureID = obstacleTextureID;
    state.obstacles[24].position = glm::vec3(-1.7f, 0.3f, 0);

    state.obstacles[25].textureID = obstacleTextureID;
    state.obstacles[25].position = glm::vec3(0.9f, 1.4f, 0);

    state.obstacles[26].textureID = obstacleTextureID;
    state.obstacles[26].position = glm::vec3(2.8f, -1.3f, 0);

    state.obstacles[27].textureID = obstacleTextureID;
    state.obstacles[27].position = glm::vec3(3.8f, -1.3f, 0);



    for (int i = 0; i < OBSTACLE_COUNT; i++)
    {
        state.obstacles[i].Update(0, NULL, 0);
        //state.obstacles[i].entityType = OBSTACLE;
    }

    state.goal->Update(0, NULL, 0);

    font_textureID = LoadTexture("font2.png");

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
                // move left
                break;
            case SDLK_RIGHT:
                // move right
                break;

                // starts game
            case SDLK_SPACE:
                break;
            }
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);


    // accounts for player's vertical movement
    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0;


bool success = false;
bool failure = false;

void Update()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP, state.obstacles, OBSTACLE_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    if (state.player->CheckCollision(state.goal))
    {
        success = true;
        state.player->speed = 0.0f;
        state.player->acceleration = glm::vec3(0);
        state.player->velocity = glm::vec3(0);
        state.player->movement = glm::vec3(0);
    }
    
    for (int i = 0; i <= OBSTACLE_COUNT; i++)
    {
        if (state.player->CheckCollision(&(state.obstacles[i])))
        {
            failure = true;
            state.player->speed = 0.0f;
            state.player->acceleration = glm::vec3(0);
            state.player->velocity = glm::vec3(0);
            state.player->movement = glm::vec3(0);
            /*break;*/
        }
    }

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



    for (int i = 0; i < OBSTACLE_COUNT; i++)
    {
        state.obstacles[i].Render(&program, 3, 1, 4);
    }
    
    
    state.player->Render(&program, 0, 1 , 1);
    state.goal->Render(&program, 1, 1, 4);

    if (failure)
    {
        DrawText(&program, font_textureID, "MISSION FAILURE!", .4, 0.02f, glm::vec3(-3.0f, 0, 0));
    }
    else if (success)
    {
        DrawText(&program, font_textureID, "MISSION SUCCESS!", .4, 0.02f, glm::vec3(-3.0f, 0 , 0));
    }

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