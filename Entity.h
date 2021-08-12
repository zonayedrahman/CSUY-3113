#pragma once
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
#include "Mesh.h"

enum EntityType { PLAYER, ENEMY, CUBE,  FLOOR, CRATE, SIDEBLOCKER, FRONTBLOCKER, BACKBLOCKER, DOOR, KEY, EXIT };

class Entity {
public:
    EntityType entityType;
    bool isActive =  true;
    bool keyAquired = false;;
    bool openingDoor = false;
    bool gameWon = false;
    bool gameLost = false;
    int lives;

    
    glm::vec3 position;
    glm::vec3 originialPosition;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    glm::vec3 scale;

    float speed;
    

    bool billboard = false;;
    float width;
    float height;
    float depth;


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


    GLuint textureID;
    Mesh* mesh;

    
    glm::mat4 modelMatrix;
    
    Entity();


    bool CheckCollision(Entity* other);
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Entity* enemies, int enemyCount);
    void DrawBillboard(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
    void Render(ShaderProgram *program);
};



