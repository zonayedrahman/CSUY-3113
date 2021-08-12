#include "MainMenu.h"


#define MAINMENU_OBJECT_COUNT 4
#define MAINMENU_ENEMY_COUNT 1


void MainMenu::Initialize() {

    state.nextScene = -1;

    state.player = new Entity();
    state.player->position = glm::vec3(-4.9, 0.75f, 72);
    state.player->originialPosition = state.player->position;
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 0.0f;
    state.player->entityType = PLAYER;

    state.objects = new Entity[MAINMENU_OBJECT_COUNT];



    GLuint floorTextureID = Util::LoadTexture("floor2.jpg");
    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 10);

    //GLuint crateTextureID = Util::LoadTexture("floor2.png");
    //Mesh* crateMesh = new Mesh();
    //crateMesh->LoadOBJ("cube.obj", 1);


    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0, -0.25f, 0);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(40, 0.5f, 200);
    state.objects[0].entityType = FLOOR;



    state.objects[1].textureID = floorTextureID;
    state.objects[1].mesh = cubeMesh;
    state.objects[1].position = glm::vec3(-4.9, 0.75f, 71);
    state.objects[1].scale = glm::vec3(1, 1, 1);
    state.objects[1].entityType = CRATE;

    state.objects[2].textureID = floorTextureID;
    state.objects[2].mesh = cubeMesh;
    state.objects[2].position = glm::vec3(-1.0, 0.5, 50);
    state.objects[2].entityType = CRATE;

    state.objects[3].textureID = floorTextureID;
    state.objects[3].mesh = cubeMesh;
    state.objects[3].position = glm::vec3(0, 1.5, 50);
    state.objects[3].entityType = CRATE;


    GLuint wallTextureID = Util::LoadTexture("wall.jpg");

    //for (int i = 4; i < 151; i++) {
    //    state.objects[i].billboard = true;
    //    state.objects[i].textureID = wallTextureID;
    //    //state.objects[3].mesh = crateMesh;
    //    state.objects[i].position = glm::vec3(-20, .5f, 100 -(i - 4));
    //    state.objects[i].entityType = SIDEBLOCKER;
    //}

    //for (int i = 4; i < MAINMENU_OBJECT_COUNT; i++) {
    //    state.objects[i].billboard = true;
    //    state.objects[i].textureID = crateTextureID;
    //    //state.objects[3].mesh = crateMesh;
    //    state.objects[i].position = glm::vec3(-20, .25f, 100 - (i - 4));
    //    state.objects[i].entityType = SIDEBLOCKER;
    //}

    state.enemies = new Entity[MAINMENU_ENEMY_COUNT];

    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    for (int i = 0; i < MAINMENU_ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].position = glm::vec3(rand() % 24 - 20, 0.5, -(rand() % 10 - 68));
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].entityType = ENEMY;
    }

}

void MainMenu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, MAINMENU_OBJECT_COUNT, state.enemies, MAINMENU_ENEMY_COUNT);

    //if (state.player->position.x >= 29) {
    //    state.nextScene = 2;
    //}


    state.player->Update(deltaTime, state.player, state.objects, MAINMENU_OBJECT_COUNT, state.enemies, MAINMENU_ENEMY_COUNT);
    for (int i = 0; i < MAINMENU_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, MAINMENU_OBJECT_COUNT, state.enemies, MAINMENU_ENEMY_COUNT);
    }
    for (int i = 0; i < MAINMENU_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.objects, MAINMENU_OBJECT_COUNT, state.enemies, MAINMENU_ENEMY_COUNT);
    }

}


void MainMenu::Render(ShaderProgram* program) {
    //state.map->Render(program);

    for (int i = 0; i < MAINMENU_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    for (int i = 0; i < MAINMENU_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    //state.player->Render(program);
    //state.key->Render(program);
    //state.door->Render(program);


    GLuint fontTextureID = Util::LoadTexture("font2.png");
    Util::DrawText(program, fontTextureID, "Main Menu", .05, 0.02f, glm::vec3(-5.2, 0.8f, 71.5));

    Util::DrawText(program, fontTextureID, "Press ENTER to Start", .018, 0.01f, glm::vec3(-5.18, 0.74f, 71.5));


}