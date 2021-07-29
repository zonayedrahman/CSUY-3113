#include "MainMenu.h"
#define MAINMENU_WIDTH 30
#define MAINMENU_HEIGHT 8
#define MAINMENU_ENEMY_COUNT 1


unsigned int mainMenu_data[] =
{
 3,    0,  291,    0,    0,    0,    0,    0,    0,    0,     0,     0,    0,    0,    0,    0,     6,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,     3,
 3,    0,    0,    0,    0,  291,    0,    0,    0,  291,     0,     0,    0,    0,    0,    0,    14,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,    23,
 3,  291,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,    0,    0,    0,    0,    34,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,     3,
 3,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,    0,    0,    0,    0,   115,  116,     116,   117,     0,     0,     0,     0,     0,     0,     0,     0,    0,    23,
 3,    0,    0,    0,    0,    0,    0,    0,    0,   83,    83,    83,    0,    0,    0,    0,     0,    0,       0,     0,     0,    84,    85,    85,    86,     0,     0,     0,    0,     3,
 3,    0,    0,    0,    0,    0,    0,    17,    0,   0,     0,     0,    0,   12,    0,    0,     0,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,     0,
 3,   83,   83,   83,   83,   83,    0,   83,   83,   83,    83,    83,   83,   83,    0,    0,   115,   84,      85,    86,   116,   116,    83,   116,   116,   116,   116,   190,  191,   192,
 3,  291,  291,  291,  291,  291,  291,  291,  291,  291,   291,   291,  291,  291,    0,    0,   291,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,  291,    23,
};

void MainMenu::Initialize() {

    state.nextScene = -1;


    GLuint mapTextureID = Util::LoadTexture("monochrome_tilemap_packed.png");
    state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainMenu_data, mapTextureID, 1.0f, 20, 20);


    // Move over all of the player and enemy code from initialization.
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5.0f, 0.0f, 0.0f);
    state.player->movement = glm::vec3(0);
    state.player->accelaration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("Fullmain.png");

    state.player->animRight = new int[7]{ 0, 10, 11, 12, 13, 14, 15 };
    state.player->animLeft = new int[7]{ 0, 10, 11, 12, 13, 14, 15 };
    //state.player->animUp = new int[1]{ 0 };
    //state.player->animDown = new int[1]{ 26 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 7;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 10;
    state.player->animRows = 12;

    state.player->height = 0.8f;
    state.player->width = 0.5f;
    state.player->jumpPower = 6.0f;
}




void MainMenu::Update(float deltaTime) {
    //state.player->Update(deltaTime, state.player, state.enemies, MAINMENU_ENEMY_COUNT, state.map);

    //if (state.player->position.x >= 12) {
    //    state.nextScene = 1;
    //}
}
void MainMenu::Render(ShaderProgram* program) {
    state.map->Render(program);
    //state.player->Render(program);
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    Util::DrawText(program, fontTextureID, "The Haunted", .35, 0.02f, glm::vec3(2.8, -2.75f, 0));
    Util::DrawText(program, fontTextureID, "Press ENTER to Start!", .25, 0.02f, glm::vec3(2.0, -3.75f, 0));
}