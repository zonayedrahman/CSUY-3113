#include "Level2.h"
#define LEVEL2_WIDTH 30
#define LEVEL2_HEIGHT 8
#define LEVEL2_ENEMY_COUNT 3


unsigned int level2_data[] =
{
   3,   295,  291,    0,    0,    0,    0,     0,    0,     0,     0,     0,    0,    0,    0,    0,     0,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,     3,
   3,     0,    0,    0,    0,  291,    0,     0,    0,     0,     0,     0,    0,    0,    0,    0,     0,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,    23,
   3,   291,    0,    0,    0,    0,    0,     0,    0,     0,    91,    91,    0,    0,  201,    0,     0,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,     3,
   3,     0,    0,    0,    0,    0,    0,   190,  191,     0,     0,     0,    0,    0,    0,    0,   115,  116,     116,   117,     0,     0,     0,     0,     0,     0,     0,     0,    0,    23,
   3,     0,    0,    0,    0,    0,    0,     0,    0,     0,     0,     0,   10,    0,    0,    0,     0,    0,       0,     0,     0,    84,    85,    85,    86,     0,     0,     0,    0,     3,
  57,     0,    0,    0,    0,    0,    0,     0,    0,     0,   190,   191,  190,    0,    0,    0,     0,    0,       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    0,     3,
 168,   169,  168,   167,  187,   0,    0,   189,  188,   189,     0,     0,    0,    0,    0,    0,   227,  228,     228,   228,   228,   229,    83,   116,   116,   116,   116,   190,  191,   192,
   3,   291,  291,   291,  291,   0,    0,  291,  291,   291,      0,     0,    0,    0,    0,    0,   247,    0,       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,  291,     0,
};

void Level2::Initialize() {

    state.nextScene = -1;


    GLuint mapTextureID = Util::LoadTexture("monochrome_tilemap_packed.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 20, 20);


    // Move over all of the player and enemy code from initialization.
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2.0f, -5.0f, 0.0f);
    state.player->originalPosition = glm::vec3(2.0f, -5.0f, 0.0f);
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

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
    }

    state.player->height = 0.8f;
    state.player->width = 0.5f;
    state.player->jumpPower = 6.0f;

    state.key = new Entity();
    state.key->entityType = KEY;
    state.key->position = glm::vec3(10.0f, -0.8f, 0.0f);

    state.key->textureID = Util::LoadTexture("key-blue.png");
    state.key->animRight = new int[7]{ 0, 1, 2, 3, 4, 5 , 6 };// 7, 8, 9, 10, 11};
    state.key->animIndices = state.key->animRight;
    state.key->animFrames = 7;
    state.key->animIndex = 0;
    state.key->animTime = 0;
    state.key->animCols = 12;
    state.key->animRows = 1;


    state.door = new Entity();
    state.door->entityType = DOOR;
    state.door->position = glm::vec3(19.0f, -4.95f, 0.0f);

    state.door->textureID = Util::LoadTexture("door.png");
    state.door->animRight = new int[7]{ 0, 1 };
    state.door->animIndices = state.door->animRight;
    state.door->animFrames = 7;
    state.door->animIndex = 0;
    state.door->animTime = 0;
    state.door->animCols = 2;
    state.door->animRows = 1;


    GLuint enemyTextureID = Util::LoadTexture("B_witch_run.png");
    state.enemies[0].textureID = enemyTextureID;

    state.enemies[0].animRight = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[0].animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[0].animFrames = 8;
    state.enemies[0].animIndex = 7;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 1;
    state.enemies[0].animRows = 8;

    state.enemies[0].position = glm::vec3(11.0f, 0.0f, 0.0f);
    state.enemies[0].accelaration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.enemies[0].aiType = PATROL;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].speed = 1.2f;




    GLuint enemy2TextureID = Util::LoadTexture("B_witch_run.png");
    state.enemies[1].textureID = enemyTextureID;

    state.enemies[1].animRight = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[1].animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[1].animIndices = state.enemies[1].animRight;
    state.enemies[1].animFrames = 8;
    state.enemies[1].animIndex = 7;
    state.enemies[1].animTime = 0;
    state.enemies[1].animCols = 1;
    state.enemies[1].animRows = 8;
    state.enemies[1].position = glm::vec3(23.0f, -3.0f, 0.0f);
    state.enemies[1].speed = 1.2f;
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = WALKING;



    GLuint enemy3TextureID = Util::LoadTexture("spr_Tentacle_strip.png");
    state.enemies[2].textureID = enemy3TextureID;

    state.enemies[2].animRight = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[2].animLeft = new int[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };
    state.enemies[2].animIndices = state.enemies[2].animRight;
    state.enemies[2].animFrames = 8;
    state.enemies[2].animIndex = 7;
    state.enemies[2].animTime = 0;
    state.enemies[2].animCols = 8;
    state.enemies[2].animRows = 1;
    state.enemies[2].position = glm::vec3(18.0f, -2.0f, 0.0f);
    state.enemies[2].speed = 1.0f;
    state.enemies[2].aiType = PATROL;
    state.enemies[2].aiState = WALKING;

}




void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.key, state.door, state.map);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.key, state.door, state.map);
    }

    if (state.player->keyAquired) {
        if (state.player->CheckCollision(state.door)) {
            state.nextScene = 3;
        }
    }

    //if (state.player->position.y < -8) {
    //    state.player->lives -= 1;
    //    if (state.player->lives > 0) {
    //        state.player->position = state.player->originalPosition;
    //        for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
    //            state.enemies[i].isActive = true;
    //        }
    //    }
    //    else if (state.player->lives <= 0) {
    //        state.player->isActive = false;
    //        state.player->gameover = true;

    //    }

    //}

    if (state.player->position.y < -8) {
        state.player->reset = true;
        state.player->decrease_life = true;
    }

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.key, state.door, state.map);
    }

    if (state.player->reset) {
        state.player->position = state.player->originalPosition;
        for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
            state.enemies[i].isActive = true;
        }
        state.key->isActive = true;
        state.player->keyAquired = false;
        state.player->reset = false;
    }

    state.key->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.key, state.door ,state.map);
    state.door->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.key, state.door, state.map);
}
void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    state.key->Render(program);
    state.door->Render(program);

    state.player->Render(program);
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    //Util::DrawText(program, fontTextureID, "Level 2", .25, 0.02f, glm::vec3(1.4, -3.75f, 0));

    //Util::DrawText(program, fontTextureID, "Lives: ", .25, 0.02f, glm::vec3(0.5, -0.8f, 0));

    //std::string lives = std::to_string(state.player->lives);
    //Util::DrawText(program, fontTextureID, lives, .25, 0.02f, glm::vec3(2.2, -0.8f, 0));
}