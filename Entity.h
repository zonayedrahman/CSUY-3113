#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#pragma once
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Map.h"


enum EntityType { PLAYER, PLATFORM, ENEMY, KEY, DOOR };
enum AIType { WALKER, WAITANDGO, PATROL };
enum AIState { IDLE, WALKING, ATTACKING };


class Entity {
public:

    EntityType entityType;
    AIType aiType;
    AIState aiState;
    EntityType lastCollision;


    glm::vec3 position;
    glm::vec3 originalPosition;
    glm::vec3 movement;
    glm::vec3 velocity;
    glm::vec3 accelaration;
    float speed;

    float width = 1;
    float height = 1;
    bool jump = false;
    float jumpPower = 0;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;


    bool isActive = true;
    bool gameover = false;
    bool keyAquired = false;
    bool reset = false;
    bool decrease_life = false;
    int lives = 3;


    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;



    int x = 1;
    int y = 1; 
    Entity();


    // collisions
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsY(Map* map);
    void CheckCollisionsX(Map* map);

    // general
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Entity* key, Entity* door, Map* map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    // enemy 
    void AI(Entity* player, Map* map);
    void AIWalker(Map* map);
    void AIWaitAndGo(Entity* player, Map* map);
    void AIPatrol(Entity* player, Map* map);
};