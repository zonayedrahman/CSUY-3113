#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    originialPosition = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
    modelMatrix = glm::mat4(1.0f);
    
    speed = 0.0f;

    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;


    lives = 0;

    //isActive = true;
    //keyAquired = false;
    //openingDoor = false;
    //billboard = false;
}


bool Entity::CheckCollision(Entity* other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;

    return false;
}


void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Entity* enemies, int enemyCount)
{

    if (isActive == false) return;


    if (animIndices != NULL) {

        if (entityType != EXIT && entityType != ENEMY) {
            animTime += deltaTime;
            
            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }

        else {
            if (entityType != EXIT) {
                animIndex = 0;
            }
        }

    }

    glm::vec3 previousPosition = position;

    if (billboard) {
        //if (entityType == SIDEBLOCKER) {
        //    if (position.x < player->position.x) {
        //    rotation.y = 90;
        //    }
        //    else {
        //        rotation.y = 270;
        //    }
        //}

        //else if (entityType == FRONTBLOCKER || entityType == BACKBLOCKER || entityType == DOOR) {
        //    
        //}

        if (entityType == ENEMY) {
            float directionX = position.x - player->position.x;
            float directionZ = position.z - player->position.z;
            rotation.y = glm::degrees(atan2f(directionX, directionZ));


        }
    }



    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    

    if (entityType == PLAYER) {
        //if (position.x < -19.6 || position.x > 19.6 || position.z < -99.6 || position.z > 99.0) {
        //    position = previousPosition;
        //}


        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;

            if (CheckCollision(&objects[i])) {

                if (objects[i].entityType == DOOR) {
                    if (openingDoor) {
                        objects[i].isActive = false;
                        openingDoor = false;
                        break;
                    }
                    else if (objects[i].isActive) {
                        position = previousPosition;
                        break;
                    }
                }

                else if (objects[i].entityType == KEY) {
                    keyAquired = true;  
                    objects[i].isActive = false; 
                }

                else if (objects[i].entityType == EXIT) {
                    if (keyAquired) {
                        gameWon = true;
                    }
                    else {
                        position = previousPosition;
                        break;
                    }
                }
                else {
                    position = previousPosition;
                    break;
                }

            }
        }

        for (int i = 0; i < enemyCount; i++) {
            if (CheckCollision(&enemies[i])) {
                isActive = false;
                gameLost = true;
            }
        }
    }



    if (entityType == ENEMY) {
        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;

            if (CheckCollision(&objects[i])) {
                velocity *= -1;
                break;
            }
        }
    }


    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


}


void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}



void Entity::DrawBillboard(ShaderProgram* program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);


    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    else  {
         glBindTexture(GL_TEXTURE_2D, textureID);

        if (billboard) {
            DrawBillboard(program);
        }
        else {
            mesh->Render(program);
        }
    }
    //program->SetModelMatrix(modelMatrix);
    

    
    
}

