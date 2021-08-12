#include "Level2.h"


#define LEVEL2_OBJECT_COUNT 4
#define LEVEL2_ENEMY_COUNT 2


void Level2::Initialize() {

    state.nextScene = -1;

    state.player = new Entity();
    state.player->position = glm::vec3(-5, 0.75f, 75);
    state.player->originialPosition = state.player->position;
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 0.0f;
    state.player->entityType = PLAYER;

    state.objects = new Entity[LEVEL2_OBJECT_COUNT];



    GLuint floorTextureID = Util::LoadTexture("floor2.jpg");
    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 10);

    GLuint crateTextureID = Util::LoadTexture("bonewall.png");
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);


    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(-5, 0.75f, 72);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(10, 10, 10);
    state.objects[0].entityType = FLOOR;



    state.objects[1].textureID = crateTextureID;
    state.objects[1].mesh = crateMesh;
    state.objects[1].position = glm::vec3(0, 0.5, -5);
    state.objects[1].entityType = CRATE;

    state.objects[2].textureID = crateTextureID;
    state.objects[2].mesh = crateMesh;
    state.objects[2].position = glm::vec3(-1.0, 0.5, -5);
    state.objects[2].entityType = CRATE;

    state.objects[3].textureID = crateTextureID;
    state.objects[3].mesh = crateMesh;
    state.objects[3].position = glm::vec3(0, 1.5, -5);
    state.objects[3].entityType = CRATE;


    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];

    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5, -(rand() % 100 - 10));
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
    }

}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT, state.enemies, LEVEL2_ENEMY_COUNT);

    //if (state.player->position.x >= 29) {
    //    state.nextScene = 2;
    //}


    state.player->Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT, state.enemies, LEVEL2_ENEMY_COUNT);
    for (int i = 0; i < LEVEL2_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT, state.enemies, LEVEL2_ENEMY_COUNT);
    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT, state.enemies, LEVEL2_ENEMY_COUNT);
    }

    //if (state.player->reset) {
    //    state.player->position = state.player->originalPosition;
    //    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
    //        state.enemies[i].isActive = true;
    //    }
    //    state.key->isActive = true;
    //    state.player->keyAquired = false;
    //    state.player->reset = false;
    //}

}


void Level2::Render(ShaderProgram* program) {
    //state.map->Render(program);

    for (int i = 0; i < LEVEL2_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    //state.player->Render(program);
    //state.key->Render(program);
    //state.door->Render(program);


    GLuint fontTextureID = Util::LoadTexture("font2.png");
    Util::DrawText(program, fontTextureID, "GAME WON!", .25, 0.02f, glm::vec3(-6, 0.75f, 71));

    //Util::DrawText(program, fontTextureID, "Lives: ", .25, 0.02f, glm::vec3(0.5, -0.8f, 0));


}