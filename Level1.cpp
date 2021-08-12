#include "Level1.h"


#define LEVEL1_OBJECT_COUNT 67
#define LEVEL1_ENEMY_COUNT 5



//#include <SDL_mixer.h>

//Mix_Chunk* death;


void Level1::Initialize() {



    state.nextScene = -1;

    state.player = new Entity();
    state.player->position = glm::vec3(-18, 0.75f, 95);
    //state.player->position = glm::vec3(5, 0.65f, 35);
    state.player->originialPosition = state.player->position;
    state.player->lives = 3;
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.5f;
    state.player->entityType = PLAYER;



    //death = Mix_LoadWAV("death.wav");


    state.objects = new Entity[LEVEL1_OBJECT_COUNT];

    GLuint floorTextureID = Util::LoadTexture("floor2.jpg");
    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 10);

    //GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    //Mesh* crateMesh = new Mesh();
    //crateMesh->LoadOBJ("cube.obj", 1);




    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0, -0.25f, 0);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(40, 0.5f, 250);
    state.objects[0].entityType = FLOOR;

    //state.objects[1].textureID = crateTextureID;
    //state.objects[1].mesh = crateMesh;
    //state.objects[1].position = glm::vec3(0, 0.5, -5);
    //state.objects[1].entityType = CRATE;


    // BEGINNING OF EDGES 


    GLuint wallTextureID = Util::LoadTexture("bonewall.png");

    //leftblocker
    state.objects[1].billboard = true;
    state.objects[1].textureID = wallTextureID;
    //state.objects[1].mesh = crateMesh;
    state.objects[1].position = glm::vec3(-20, .5f, 0);
    state.objects[1].scale = glm::vec3(1, 1, 200);
    state.objects[1].depth = 200;
    state.objects[1].rotation.y = 90;
    state.objects[1].entityType = SIDEBLOCKER;

    // frontblocker
    state.objects[2].billboard = true;
    state.objects[2].textureID = wallTextureID;
    //state.objects[2].mesh = crateMesh;
    state.objects[2].position = glm::vec3(0, .5f, 10);
    state.objects[2].scale = glm::vec3(40, 1, 1);
    state.objects[2].width = 40;
    //state.objects[2].rotation.y = 90;
    state.objects[2].entityType = FRONTBLOCKER;

    ////
    state.objects[3].billboard = true;
    state.objects[3].textureID = wallTextureID;
    //state.objects[3].mesh = crateMesh;
    state.objects[3].position = glm::vec3(20, .5f, 0);
    state.objects[3].scale = glm::vec3(1, 1, 200);
    state.objects[3].depth = 200;
    state.objects[3].rotation.y = -90;
    state.objects[3].entityType = SIDEBLOCKER;

    state.objects[4].billboard = true;
    state.objects[4].textureID = wallTextureID;
    //state.objects[4].mesh = crateMesh;
    state.objects[4].position = glm::vec3(0, .5f, 100);
    state.objects[4].scale = glm::vec3(40, 1, 1);
    state.objects[4].width = 40;
    //state.objects[4].rotation.y = 90;
    state.objects[4].entityType = FRONTBLOCKER;


    // END OF EDGES



    //// first right wall
    state.objects[5].billboard = true;
    state.objects[5].textureID = wallTextureID;
    //state.objects[5].mesh = crateMesh;
    state.objects[5].position = glm::vec3(-15, .5f, 85.5);
    state.objects[5].scale = glm::vec3(1, 1, 35);
    state.objects[5].depth = 35.0f;
    state.objects[5].rotation.y = 90;
    state.objects[5].entityType = SIDEBLOCKER;


    //// first front wall 
    state.objects[6].billboard = true;
    state.objects[6].textureID = wallTextureID;
    //state.objects[6].mesh = crateMesh;
    state.objects[6].position = glm::vec3(-19.5, .5f, 82);
    state.objects[6].scale = glm::vec3(5.5, 1, 1);
    state.objects[6].width = 5.5;
    //state.objects[6].rotation.y = 90;
    state.objects[6].entityType = FRONTBLOCKER;




    GLuint doorTextureID = Util::LoadTexture("door.png");

    //// door 1 -  front when spawn
    state.objects[7].billboard = true;
    state.objects[7].textureID = doorTextureID;
    state.objects[7].position = glm::vec3(-16, .55f, 82);
    state.objects[7].scale = glm::vec3(3, 1.25, 1);
    state.objects[7].width = 3;
    state.objects[7].depth = 0.0f;
    state.objects[7].height = 1.25;
    state.objects[7].entityType = DOOR;

    //// door 2 -  first right after door 1
    state.objects[8].billboard = true;
    state.objects[8].textureID = doorTextureID;
    state.objects[8].position = glm::vec3(-15, .55f, 67.25);
    state.objects[8].scale = glm::vec3(1, 1.25, 3);
    state.objects[8].height = 1.25;
    state.objects[8].depth = 3;
    state.objects[8].width = 0.0f;
    state.objects[8].rotation.y = 90;
    state.objects[8].entityType = DOOR;

    //// rightside after door 2
    state.objects[9].billboard = true;
    state.objects[9].textureID = wallTextureID;
    //state.object92[8].mesh = crateMesh;
    state.objects[9].position = glm::vec3(-15, .5f, 59);
    state.objects[9].scale = glm::vec3(1, 1, 15);
    state.objects[9].depth = 15;
    state.objects[9].rotation.y = 90;
    state.objects[9].entityType = SIDEBLOCKER;
    //state.objects[9].isActive = false;


    // door 3 - 2ND DOOR IN MAIN HALL
    state.objects[10].billboard = true;

    state.objects[10].position = glm::vec3(-15, .55f, 50.8);
    state.objects[10].scale = glm::vec3(1, 1.25, 3);
    state.objects[10].height = 1.25;
    state.objects[10].depth = 3;
    state.objects[10].rotation.y = 90;
    state.objects[10].width = 0.0f;
    state.objects[10].textureID = doorTextureID;
    state.objects[10].entityType = DOOR;
    //state.objects[10].isActive = false;



    // rightside after door 3
    state.objects[11].billboard = true;
    //state.objects[11].textureID = wallTextureID;
    //state.object112[8].mesh = crateMesh;
    state.objects[11].position = glm::vec3(-15, .5f, 32.55);
    state.objects[11].scale = glm::vec3(1, 1, 35);
    state.objects[11].depth = 35;
    state.objects[11].rotation.y = -90;
    state.objects[11].textureID = wallTextureID;
    state.objects[11].entityType = SIDEBLOCKER;


    //// first right 
    state.objects[12].billboard = true;
    state.objects[12].textureID = wallTextureID;
    //state.object122[8].mesh = crateMesh;
    state.objects[12].position = glm::vec3(-13.0, .5f, 15.05);
    state.objects[12].scale = glm::vec3(4, 1, 1);
    //state.objects[12].rotation.y = 90;
    state.objects[12].width = 4;
    state.objects[12].entityType = FRONTBLOCKER;
    //state.objects[12].isActive = false;


    // front end RIGHT TURN (ROOM 9 FRONT) door 1
    state.objects[13].billboard = true;
    state.objects[13].position = glm::vec3(-10.25, .5f, 15.05);
    state.objects[13].scale = glm::vec3(3, 1.25, 0);
    state.objects[13].height = 1.25;
    state.objects[13].width = 3;
    state.objects[13].depth = 0.0f;
    //state.objects[13].rotation.y = 90;
    state.objects[13].textureID = doorTextureID;
    state.objects[13].entityType = DOOR;
    //state.objects[13].isActive = false;


    // right after door 1
    state.objects[14].billboard = true;
    state.objects[14].textureID = wallTextureID;
    //state.objects[14].mesh = crateMesh;
    state.objects[14].position = glm::vec3(-3.0, .5f, 15.05);
    state.objects[14].scale = glm::vec3(13, 1, 1);
    //state.objects[14].rotation.y = 90;
    state.objects[14].width = 13;
    state.objects[14].entityType = FRONTBLOCKER;

    // front end door 2
    state.objects[15].billboard = true;
    state.objects[15].position = glm::vec3(4.25, .5f, 15.05);
    state.objects[15].scale = glm::vec3(3, 1.25, 0);
    state.objects[15].height = 1.25;
    state.objects[15].width = 3;
    state.objects[15].depth = 0.0f;
    //state.objects[15].rotation.y = 90;
    state.objects[15].textureID = doorTextureID;
    state.objects[15].entityType = DOOR;

    // right after door 2
    state.objects[16].billboard = true;
    state.objects[16].textureID = wallTextureID;
    //state.objects[16].mesh = crateMesh;
    state.objects[16].position = glm::vec3(13.0, .5f, 15.05);
    state.objects[16].scale = glm::vec3(16, 1, 1);
    //state.objects[16].rotation.y = 90;
    state.objects[16].width = 16;
    state.objects[16].entityType = FRONTBLOCKER;


    // in door 1 rightblock
    state.objects[17].billboard = true;
    state.objects[17].textureID = wallTextureID;
    //state.objects[17].mesh = crateMesh;
    state.objects[17].position = glm::vec3(2.5, .5f, 72);
    state.objects[17].scale = glm::vec3(35, 1, 1);
    //state.objects[17].rotation.y = 90;
    state.objects[17].width = 35;
    state.objects[17].entityType = FRONTBLOCKER;

    // door 1 left - left block
    state.objects[18].billboard = true;
    state.objects[18].textureID = wallTextureID;
    //state.objects[18].mesh = crateMesh;
    state.objects[18].position = glm::vec3(-12, .5f, 63);
    state.objects[18].scale = glm::vec3(6, 1, 1);
    //state.objects[18].rotation.y = 90;
    state.objects[18].width = 6;
    state.objects[18].entityType = FRONTBLOCKER;



    // after door 1 first left
    state.objects[19].billboard = true;
    state.objects[19].position = glm::vec3(-8.25, .5f, 63);
    state.objects[19].scale = glm::vec3(3, 1.25, 0);
    state.objects[19].height = 1.25;
    state.objects[19].width = 3;
    state.objects[19].depth = 0.0f;
    //state.objects[19].rotation.y = 90;
    state.objects[19].textureID = doorTextureID;
    state.objects[19].entityType = DOOR;


    // after first left

    state.objects[20].billboard = true;
    state.objects[20].textureID = wallTextureID;
    //state.objects[20].mesh = crateMesh;
    state.objects[20].position = glm::vec3(1.0, .5f, 63);
    state.objects[20].scale = glm::vec3(17, 1, 1);
    //state.objects[20].rotation.y = 90;
    state.objects[20].width = 17.0f;
    state.objects[20].entityType = FRONTBLOCKER;


    // room 3 front
    state.objects[21].billboard = true;
    state.objects[21].textureID = wallTextureID;
    //state.objects[21].mesh = crateMesh;
    state.objects[21].position = glm::vec3(-5, .5f, 53);
    state.objects[21].scale = glm::vec3(20, 1, 1);
    //state.object2118].rotation.y = 90;
    state.objects[21].width = 20.0f;
    state.objects[21].entityType = FRONTBLOCKER;
    //state.objects[21].isActive = false;



    // room 5 front
    state.objects[22].billboard = true;
    state.objects[22].textureID = wallTextureID;
    //state.object2221].mesh = crateMesh;
    state.objects[22].position = glm::vec3(-5, .5f, 48.5);
    state.objects[22].scale = glm::vec3(20, 1, 1);
    //state.object2218].rotation.y = 90;
    state.objects[22].width = 20;
    state.objects[22].entityType = FRONTBLOCKER;
    //state.objects[22].isActive = false;


    //room 3 right
    state.objects[23].billboard = true;
    state.objects[23].textureID = wallTextureID;
    //state.object232[8].mesh = crateMesh;
    state.objects[23].position = glm::vec3(-3.25, .5f, 59.5);
    state.objects[23].scale = glm::vec3(1, 1, 7);
    state.objects[23].rotation.y = -90;
    state.objects[23].depth = 7;
    state.objects[23].entityType = SIDEBLOCKER;


    // room 3 right side door
    state.objects[24].billboard = true;
    state.objects[24].textureID = doorTextureID;
    state.objects[24].position = glm::vec3(-3.25, .5f, 55.25);
    state.objects[24].scale = glm::vec3(1, 1.25, 3);
    state.objects[24].depth = 3;
    state.objects[24].height = 1.25;
    state.objects[24].width = 0.0f;
    state.objects[24].rotation.y = 90;
    state.objects[24].entityType = DOOR;




    // left side of door
    state.objects[25].billboard = true;
    state.objects[25].textureID = wallTextureID;
    //state.object252[8].mesh = crateMesh;
    state.objects[25].position = glm::vec3(-3.25, .5f, 53);
    state.objects[25].scale = glm::vec3(1, 1, 3);
    state.objects[25].rotation.y = -90;
    state.objects[25].depth = 3;
    state.objects[25].entityType = SIDEBLOCKER;


    // room 5 right door
    state.objects[26].billboard = true;
    state.objects[26].textureID = doorTextureID;
    state.objects[26].position = glm::vec3(-3.25, .5f, 50.75);
    state.objects[26].scale = glm::vec3(1, 1.25, 3);
    state.objects[26].depth = 3;
    state.objects[26].width = 0.0f;
    state.objects[26].height = 1.25;
    state.objects[26].rotation.y = 90;
    state.objects[26].entityType = DOOR;


    // room 5 right - left of door
    state.objects[27].billboard = true;
    state.objects[27].textureID = wallTextureID;
    //state.object272[8].mesh = crateMesh;
    state.objects[27].position = glm::vec3(-3.25, .5f, 48.2);
    state.objects[27].scale = glm::vec3(1, 1, 3.6);
    state.objects[27].rotation.y = -90;
    state.objects[27].depth = 3.6;
    state.objects[27].entityType = SIDEBLOCKER;



    // room 8 front left

    state.objects[28].billboard = true;
    state.objects[28].textureID = wallTextureID;
    //state.object2821].mesh = crateMesh;
    state.objects[28].position = glm::vec3(-12.5, .5f, 30);
    state.objects[28].scale = glm::vec3(5, 1, 1);
    //state.object2818].rotation.y = 90;
    state.objects[28].width = 5;
    state.objects[28].entityType = FRONTBLOCKER;




    // ROOM 8 FRONT DOOR
    state.objects[29].billboard = true;
    state.objects[29].position = glm::vec3(-9.25, .5f, 30);
    state.objects[29].scale = glm::vec3(3, 1.25, 0);
    state.objects[29].height = 1.25;
    state.objects[29].width = 3;
    state.objects[29].depth = 0.0f;
    //state.object2913].rotation.y = 90;
    state.objects[29].textureID = doorTextureID;
    state.objects[29].entityType = DOOR;


    // ROOM 8 FRONT RIGHT SIDE

    state.objects[30].billboard = true;
    state.objects[30].textureID = wallTextureID;
    //state.object3021].mesh = crateMesh;
    state.objects[30].position = glm::vec3(-6, .5f, 30);
    state.objects[30].scale = glm::vec3(5.5, 1, 1);
    //state.object3018].rotation.y = 90;
    state.objects[30].width = 5.5;
    state.objects[30].entityType = FRONTBLOCKER;



    //ROOM 9 RIGHT AND ROOM 8 RIGHT LEFT OF DOOR
    state.objects[31].billboard = true;
    state.objects[31].textureID = wallTextureID;
    //state.object312[8].mesh = crateMesh;
    state.objects[31].position = glm::vec3(-3.25, .5f, 30);
    state.objects[31].scale = glm::vec3(1, 1, 29.9);
    state.objects[31].depth = 29.9;
    state.objects[31].rotation.y = 90;
    state.objects[31].entityType = SIDEBLOCKER;


    // ROOM 8 RIGHT DOOR

    state.objects[32].billboard = true;
    state.objects[32].textureID = doorTextureID;
    state.objects[32].position = glm::vec3(-3.25, .5f, 45.7);
    state.objects[32].scale = glm::vec3(1, 1.25, 3);
    state.objects[32].depth = 3;
    state.objects[32].width = 0.0f;
    state.objects[32].height = 1.25;
    state.objects[32].rotation.y = 90;
    state.objects[32].entityType = DOOR;






    // ROOM 7 RIGHT 
    state.objects[33].billboard = true;
    state.objects[33].textureID = wallTextureID;
    //state.object332[8].mesh = crateMesh;
    state.objects[33].position = glm::vec3(5, .5f, 50.75);
    state.objects[33].scale = glm::vec3(1, 1, 4.55);
    state.objects[33].rotation.y = -90;
    state.objects[33].depth = 4.55;
    state.objects[33].width = 0;
    state.objects[33].entityType = SIDEBLOCKER;



    // ROOM 6 RIGHT - RIGHT OF DOOR

    state.objects[34].billboard = true;
    state.objects[34].textureID = wallTextureID;
    //state.object342[8].mesh = crateMesh;
    state.objects[34].position = glm::vec3(2, .5f, 52.35);
    state.objects[34].scale = glm::vec3(1, 1, 1.3);
    state.objects[34].rotation.y = -90;
    state.objects[34].depth = 1.3;
    state.objects[34].entityType = SIDEBLOCKER;



    // ROOM 6 RIGHT DOOR
    state.objects[35].billboard = true;
    state.objects[35].textureID = doorTextureID;
    state.objects[35].position = glm::vec3(2, .5f, 51);
    state.objects[35].scale = glm::vec3(1, 1.25, 3);
    state.objects[35].depth = 3;
    state.objects[35].width = 0.0f;
    state.objects[35].height = 1.25;
    state.objects[35].rotation.y = 90;
    state.objects[35].entityType = DOOR;



    // ROOM 6 RIGHT - LEFT OF DOOR

    state.objects[36].billboard = true;
    state.objects[36].textureID = wallTextureID;
    //state.object362[8].mesh = crateMesh;
    state.objects[36].position = glm::vec3(2, .5f, 49.38);
    state.objects[36].scale = glm::vec3(1, 1, 1.75);
    state.objects[36].rotation.y = -90;
    state.objects[36].depth = 1.75;
    state.objects[36].entityType = SIDEBLOCKER;



    // ROOM 4 RIGHT - RIGHT SIDE OF DOOR
    state.objects[37].billboard = true;
    state.objects[37].textureID = wallTextureID;
    //state.object372[8].mesh = crateMesh;
    state.objects[37].position = glm::vec3(9.5, .5f, 60.5);
    state.objects[37].scale = glm::vec3(1, 1, 5);
    state.objects[37].rotation.y = -90;
    state.objects[37].depth = 5;
    state.objects[37].entityType = SIDEBLOCKER;



    // ROOM 4 RIGHT DOOR
    state.objects[38].billboard = true;
    state.objects[38].textureID = doorTextureID;
    state.objects[38].position = glm::vec3(9.5, .5f, 57.25);
    state.objects[38].scale = glm::vec3(1, 1.25, 3);
    state.objects[38].depth = 3;
    state.objects[38].width = 0.0f;
    state.objects[38].height = 1.25;
    state.objects[38].rotation.y = 90;
    state.objects[38].entityType = DOOR;



    // ROOM 4 RIGHT - LEFT SIDE OF DOOR
    state.objects[39].billboard = true;
    state.objects[39].textureID = wallTextureID;
    //state.object392[8].mesh = crateMesh;
    state.objects[39].position = glm::vec3(9.5, .5f, 54);
    state.objects[39].scale = glm::vec3(1, 1, 5);
    state.objects[39].rotation.y = -90;
    state.objects[39].depth = 5;
    state.objects[39].entityType = SIDEBLOCKER;



    // ROOM 4 FRONT RIGHT

    state.objects[40].billboard = true;
    state.objects[40].textureID = wallTextureID;
    //state.object4021].mesh = crateMesh;
    state.objects[40].position = glm::vec3(11, .5f, 51.5);
    state.objects[40].scale = glm::vec3(9, 1, 1);
    //state.object4018].rotation.y = 90;
    state.objects[40].width = 9.0f;
    state.objects[40].entityType = FRONTBLOCKER;
    //state.object4021].isActive = false;


    // ROOM 4 FRONT RIGHT DOOR 

    state.objects[41].billboard = true;
    state.objects[41].position = glm::vec3(5.75, .55f, 51.5);
    state.objects[41].scale = glm::vec3(3, 1.25, 0);
    state.objects[41].height = 1.25;
    state.objects[41].width = 3;
    state.objects[41].depth = 0.0f;
    //state.object4113].rotation.y = 90;
    state.objects[41].textureID = doorTextureID;
    state.objects[41].entityType = DOOR;


    // ROOM 2  FRONT 2ND DOOR (MOST RIGHT)

    state.objects[42].billboard = true;
    state.objects[42].position = glm::vec3(16.25, .55f, 51.5);
    state.objects[42].scale = glm::vec3(3, 1.25, 0);
    state.objects[42].height = 1.25;
    state.objects[42].width = 2;
    state.objects[42].depth = 0;
    //state.object4213].rotation.y = 90;
    state.objects[42].textureID = doorTextureID;
    state.objects[42].entityType = DOOR;



    // room 10 right back side

    state.objects[43].billboard = true;
    state.objects[43].textureID = wallTextureID;
    //state.object432[8].mesh = crateMesh;
    state.objects[43].position = glm::vec3(17, .5f, 38);
    state.objects[43].scale = glm::vec3(1, 1, 27);
    state.objects[43].rotation.y = -90;
    state.objects[43].depth = 27;
    //state.objects[43].width = 0;
    state.objects[43].entityType = SIDEBLOCKER;
    //state.objects[43].isActive = false;




    // ROOM 10 FRONT RIGHT DOOR

    state.objects[44].billboard = true;
    state.objects[44].position = glm::vec3(16.25, .55f, 46.5);
    state.objects[44].scale = glm::vec3(3, 1.25, 0);
    state.objects[44].height = 1.25;
    state.objects[44].width = 2;
    state.objects[44].depth = 0;
    //state.object4413].rotation.y = 90;
    state.objects[44].textureID = doorTextureID;
    state.objects[44].entityType = DOOR;



    //// ROOM 10 FRONT RIGHT SIDE
    state.objects[45].billboard = true;
    state.objects[45].textureID = wallTextureID;
    //state.object4521].mesh = crateMesh;
    state.objects[45].position = glm::vec3(13.5, .5f, 46.5);
    state.objects[45].scale = glm::vec3(4, 1, 1);
    //state.object4518].rotation.y = 90;
    state.objects[45].width = 4.0f;
    state.objects[45].entityType = FRONTBLOCKER;
    //state.object4521].isActive = false;


    // ROOM 10 RIGHT TOP SIDE

    state.objects[46].billboard = true;
    state.objects[46].textureID = wallTextureID;
    //state.object462[8].mesh = crateMesh;
    state.objects[46].position = glm::vec3(11.5, .5f, 43);
    state.objects[46].scale = glm::vec3(1, 1, 7);
    state.objects[46].rotation.y = -90;
    state.objects[46].depth = 7;
    //state.object4643].width = 0;
    state.objects[46].entityType = SIDEBLOCKER;

    // ROOM 10 FRONT LEFT 
    state.objects[47].billboard = true;
    state.objects[47].textureID = wallTextureID;
    //state.object4721].mesh = crateMesh;
    state.objects[47].position = glm::vec3(4.15, .5f, 39.5);
    state.objects[47].scale = glm::vec3(14.8, 1, 1);
    //state.object4718].rotation.y = 90;
    state.objects[47].width = 14.8f;
    state.objects[47].entityType = FRONTBLOCKER;
    //state.object4721].isActive = false;



    // ROOM 13 LEFT 


    state.objects[48].billboard = true;
    state.objects[48].textureID = wallTextureID;
    //state.object482[8].mesh = crateMesh;
    state.objects[48].position = glm::vec3(2, .5f, 16.55);
    state.objects[48].scale = glm::vec3(1, 1, 3);
    state.objects[48].rotation.y = -90;
    state.objects[48].depth = 3;
    //state.objects843].width = 0;
    state.objects[48].entityType = SIDEBLOCKER;

    // ROOM 13 RIGHT


    state.objects[49].billboard = true;
    state.objects[49].textureID = wallTextureID;
    //state.object492[8].mesh = crateMesh;
    state.objects[49].position = glm::vec3(8, .5f, 16.55);
    state.objects[49].scale = glm::vec3(1, 1, 3);
    state.objects[49].rotation.y = -90;
    state.objects[49].depth = 3;
    //state.objects943].width = 0;
    state.objects[49].entityType = SIDEBLOCKER;
    //state.objects[49].isActive = false;


    // ROOM 13 LEFT DOOR

    state.objects[50].billboard = true;
    state.objects[50].textureID = doorTextureID;
    state.objects[50].position = glm::vec3(2, .55f, 18.8);
    state.objects[50].scale = glm::vec3(1, 1.25, 3);
    state.objects[50].height = 1.25;
    state.objects[50].depth = 3;
    state.objects[50].width = 0.0f;
    state.objects[50].rotation.y = 90;
    state.objects[50].entityType = DOOR;


    // ROOM 13 RIGHT DOOR

    state.objects[51].billboard = true;
    state.objects[51].textureID = doorTextureID;
    state.objects[51].position = glm::vec3(8, .55f, 18.8);
    state.objects[51].scale = glm::vec3(1, 1.25, 3);
    state.objects[51].height = 1.25;
    state.objects[51].depth = 3;
    state.objects[51].width = 0.0f;
    state.objects[51].rotation.y = 90;
    state.objects[51].entityType = DOOR;

    // ROOM 13 LEFT BEFORE DOOR


    state.objects[52].billboard = true;
    state.objects[52].textureID = wallTextureID;
    //state.object522[8].mesh = crateMesh;
    state.objects[52].position = glm::vec3(2, .5f, 21.05);
    state.objects[52].scale = glm::vec3(1, 1, 3);
    state.objects[52].rotation.y = -90;
    state.objects[52].depth = 3;
    //state.object5243].width = 0;
    state.objects[52].entityType = SIDEBLOCKER;

    // ROOM 13 RIGHT BEFORE DOOR


    state.objects[53].billboard = true;
    state.objects[53].textureID = wallTextureID;
    //state.object532[8].mesh = crateMesh;
    state.objects[53].position = glm::vec3(8, .5f, 21.05);
    state.objects[53].scale = glm::vec3(1, 1, 3);
    state.objects[53].rotation.y = -90;
    state.objects[53].depth = 3;
    //state.object5343].width = 0;
    state.objects[53].entityType = SIDEBLOCKER;
    //state.object5349].isActive = false;


    // ROOM 13 BACK DOOR

    state.objects[54].billboard = true;
    state.objects[54].textureID = doorTextureID;
    state.objects[54].position = glm::vec3(7.245, .55f, 22.55);
    state.objects[54].scale = glm::vec3(3, 1.25, 1);
    state.objects[54].width = 3;
    state.objects[54].depth = 0.0f;
    state.objects[54].height = 1.25;
    state.objects[54].entityType = DOOR;

    // ROOM 13 BOTTOM LEFT OF DOOR + 12 BOTTOM

    state.objects[55].billboard = true;
    state.objects[55].textureID = wallTextureID;
    //state.object5521].mesh = crateMesh;
    state.objects[55].position = glm::vec3(2.65, .5f, 22.55);
    state.objects[55].scale = glm::vec3(7.7, 1, 1);
    //state.object5518].rotation.y = 90;
    state.objects[55].width = 7.7f;
    state.objects[55].entityType = FRONTBLOCKER;
    //state.object5521].isActive = false;


    // ROOM 12 FRONT
    state.objects[56].billboard = true;
    state.objects[56].textureID = wallTextureID;
    //state.object5621].mesh = crateMesh;
    state.objects[56].position = glm::vec3(-0.35, .5f, 17);
    state.objects[56].scale = glm::vec3(1.71, 1, 1);
    //state.object5618].rotation.y = 90;
    state.objects[56].width = 1.2f;
    state.objects[56].depth = 0.0f;
    state.objects[56].entityType = FRONTBLOCKER;
    //state.objects[56].isActive = false;


    // ROOM 12 BACK
    state.objects[57].billboard = true;
    state.objects[57].textureID = wallTextureID;
    //state.object57[5].mesh = crateMesh;
    state.objects[57].position = glm::vec3(-1.2, .5f, 19.75);
    state.objects[57].scale = glm::vec3(1, 1, 5.6);
    state.objects[57].rotation.y = 90;
    state.objects[57].depth = 5.6f;
    state.objects[57].entityType = SIDEBLOCKER;



    // ROOM 12 FRONT DOOR

    state.objects[58].billboard = true;
    state.objects[58].textureID = doorTextureID;
    state.objects[58].position = glm::vec3(1.22, .5f, 17);
    state.objects[58].scale = glm::vec3(3, 1.25, 1);
    state.objects[58].width = 3;
    state.objects[58].depth = 0.0f;
    state.objects[58].height = 1.25;
    state.objects[58].entityType = DOOR;




    // ROOM 14 LEFT WALL
    state.objects[59].billboard = true;
    state.objects[59].textureID = wallTextureID;
    //state.object59[5].mesh = crateMesh;
    state.objects[59].position = glm::vec3(3.5, .5f, 25);
    state.objects[59].scale = glm::vec3(1, 1, 4.9);
    state.objects[59].rotation.y = 90;
    state.objects[59].depth = 4.9;
    state.objects[59].entityType = SIDEBLOCKER;



    // ROOM 14 + 15 BOTTOM
    state.objects[60].billboard = true;
    state.objects[60].textureID = wallTextureID;
    //state.object6021].mesh = crateMesh;
    state.objects[60].position = glm::vec3(11, .5f, 27.4);
    state.objects[60].scale = glm::vec3(15, 1, 1);
    //state.object6018].rotation.y = 90;
    state.objects[60].width = 15.2f;
    state.objects[60].entityType = FRONTBLOCKER;
    //state.objects[60].isActive = false;



    // ROOM 14 RIGHTWALL / ROOM 15 LEFT WALL
    state.objects[61].billboard = true;
    state.objects[61].textureID = wallTextureID;
    //state.object61[5].mesh = crateMesh;
    state.objects[61].position = glm::vec3(11.5, .5f, 21.225);
    state.objects[61].scale = glm::vec3(1, 1, 12.35);
    state.objects[61].rotation.y = 90;
    state.objects[61].depth = 12.35;
    state.objects[61].entityType = SIDEBLOCKER;



    // ROOM 15 RIGHT DOOR

    state.objects[62].billboard = true;
    state.objects[62].textureID = doorTextureID;
    state.objects[62].position = glm::vec3(17, .5f, 23.75);
    state.objects[62].scale = glm::vec3(1, 1.25, 3);
    state.objects[62].height = 1.25;
    state.objects[62].depth = 3;
    state.objects[62].width = 0.0f;
    state.objects[62].rotation.y = 90;
    state.objects[62].entityType = DOOR;




    // ROOM 15 RIGHT AFTER DOOR

    state.objects[63].billboard = true;
    state.objects[63].textureID = wallTextureID;
    //state.object632[8].mesh = crateMesh;
    state.objects[63].position = glm::vec3(17, .5f, 19.025);
    state.objects[63].scale = glm::vec3(1, 1, 7.95);
    state.objects[63].rotation.y = -90;
    state.objects[63].depth = 7.95;
    //state.object6343].width = 0;
    state.objects[63].entityType = SIDEBLOCKER;
    //state.object6343].isActive = false;



    // ROOM 16 BACK DOOR

    state.objects[64].billboard = true;
    state.objects[64].textureID = doorTextureID;
    state.objects[64].position = glm::vec3(19.25, .55f, 27.4);
    state.objects[64].scale = glm::vec3(3, 1.25, 1);
    state.objects[64].width = 3;
    state.objects[64].depth = 0.0f;
    state.objects[64].height = 1.25;
    state.objects[64].entityType = DOOR;



    for (int i = 1; i < LEVEL1_OBJECT_COUNT; i++) {
        if (state.objects[i].entityType == SIDEBLOCKER || state.objects[i].entityType == FRONTBLOCKER ) {
             state.objects[i].scale.y = 1.5;
        }  
        else if (state.objects[i].entityType == DOOR) {
            state.objects[i].scale.y = 1.5;
            state.objects[i].position.y = .65;
        }
    }

    GLuint keyTextureID = Util::LoadTexture("key-blue.png");

    state.objects[65].billboard = true;
    state.objects[65].textureID = keyTextureID;
    state.objects[65].animRight = new int[7]{ 0, 1, 2, 3, 4, 5 , 6 };
    state.objects[65].animIndices = state.objects[65].animRight;
    state.objects[65].animFrames = 7;
    state.objects[65].animIndex = 0;
    state.objects[65].animTime = 0;
    state.objects[65].animCols = 12;
    state.objects[65].animRows = 1;;
                  
    state.objects[65].position = glm::vec3(10, .5f, 18);
                  
    state.objects[65].entityType = KEY;



    GLuint exitTextureID = Util::LoadTexture("exit.png");

    state.objects[66].billboard = true;
    state.objects[66].textureID = exitTextureID;
    state.objects[66].animRight = new int[2]{ 0, 3 };
    state.objects[66].animIndices = state.objects[66].animRight;
    state.objects[66].animFrames = 1;
    state.objects[66].animIndex = 0;
    state.objects[66].animTime = 0;
    state.objects[66].animCols = 5;
    state.objects[66].animRows = 1;
    state.objects[66].position = glm::vec3(14, .62f, 15.2);
    state.objects[66].scale = glm::vec3(1, 1.25, 1);
    state.objects[66].entityType = EXIT;

    for (int i = 0; i < LEVEL1_OBJECT_COUNT; i++) {

        if (state.objects[i].entityType == FRONTBLOCKER) {
            state.objects[i].depth = 0.0f;
        }

        else if (state.objects[i].entityType == SIDEBLOCKER) {
            state.objects[i].width = 0.0f;
        }

        //state.objects[i].width = 0.0f;
        //state.objects[i].depth = 0.0f;

    }



    //state.player->keyAquired = true;



    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];

    GLuint enemyTextureID = Util::LoadTexture("enemy.png");


    state.enemies[0].billboard = true;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(-18, 0.65f, 55);
    state.enemies[0].rotation = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(1, 0, 1);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].speed = 0.2f;
    state.enemies[0].entityType = ENEMY;

    state.enemies[1].billboard = true;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(-5, 0.65f, 70);
    state.enemies[1].rotation = glm::vec3(0, 0, 0);
    state.enemies[1].velocity = glm::vec3(1, 0, 1);
    state.enemies[1].acceleration = glm::vec3(0, 0, 0);
    state.enemies[1].speed = 0.2f;
    state.enemies[1].entityType = ENEMY;

    state.enemies[2].billboard = true;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(-10, 0.65f, 55);
    state.enemies[2].rotation = glm::vec3(0, 0, 0);
    state.enemies[2].velocity = glm::vec3(1, 0, 1);
    state.enemies[2].acceleration = glm::vec3(0, 0, 0);
    state.enemies[2].speed = 0.2f;
    state.enemies[2].entityType = ENEMY;

    state.enemies[3].billboard = true;
    state.enemies[3].textureID = enemyTextureID;
    state.enemies[3].position = glm::vec3(10, 0.65f, 55);
    state.enemies[3].rotation = glm::vec3(0, 0, 0);
    state.enemies[3].velocity = glm::vec3(1, 0, 1);
    state.enemies[3].acceleration = glm::vec3(0, 0, 0);
    state.enemies[3].speed = 0.2f;
    state.enemies[3].entityType = ENEMY;

    state.enemies[4].billboard = true;
    state.enemies[4].textureID = enemyTextureID;
    state.enemies[4].position = glm::vec3(5, 0.65f, 30);
    state.enemies[4].rotation = glm::vec3(0, 0, 0);
    state.enemies[4].velocity = glm::vec3(1, 0, 1);
    state.enemies[4].acceleration = glm::vec3(0, 0, 0);
    state.enemies[4].speed = 0.2f;
    state.enemies[4].entityType = ENEMY;


    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].animRight = new int[2]{ 0, 1 };
        state.enemies[i].animIndices = state.objects[66].animRight;
        state.enemies[i].animFrames = 1;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 4;
        state.enemies[i].animRows = 1;
        state.enemies[i].scale = glm::vec3(1, 1.5, 1);
    }
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT, state.enemies, LEVEL1_ENEMY_COUNT);


    if (state.player->gameWon) {
        state.nextScene = 2;
    }
    if (state.player->keyAquired) {
        state.objects[66].animIndex = 1;
    }

    //state.player->Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT, state.enemies, LEVEL1_ENEMY_COUNT);
    for (int i = 0; i < LEVEL1_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT, state.enemies, LEVEL1_ENEMY_COUNT);
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        //if (glm::distance(state.player->position, state.enemies[i].position) < 12.0f) {
        //    state.enemies[i].velocity.x += 1;
        //    state.enemies[i].velocity.z += 1;


        //}
        state.enemies[i].Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT, state.enemies, LEVEL1_ENEMY_COUNT);
    }



}


void Level1::Render(ShaderProgram* program) {
    //state.map->Render(program);

    for (int i = 0; i < LEVEL1_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    //state.player->Render(program);
    //state.key->Render(program);
    //state.door->Render(program);

    GLuint fontTextureID = Util::LoadTexture("font2.png");
    Util::DrawText(program, fontTextureID, "Start!", .25, 0.02f, glm::vec3(-19, 1.0f, 90));

    if (state.player->keyAquired) {
        Util::DrawText(program, fontTextureID, "Key Aquired! Find the Exit!", .04, 0.02f, glm::vec3(state.player->position.x - .7, 1.2f, state.player->position.z - 2));
    }

    if (state.player->gameLost) {
        //Mix_PlayChannel(-1, death, 0);
        Util::DrawText(program, fontTextureID, "Game Lost!", .04, 0.02f, glm::vec3(state.player->position.x, 1.2f, state.player->position.z - 2));
    }

    //Util::DrawText(program, fontTextureID, "Lives: ", .25, 0.02f, glm::vec3(0.5, -0.8f, 0));


}