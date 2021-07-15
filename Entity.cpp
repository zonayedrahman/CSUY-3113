#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    //isActive = true;
    movement = glm::vec3(0);
    velocity = glm::vec3(0);
    accelaration = glm::vec3(0);
    speed = 0;
    //gameover = false;

    modelMatrix = glm::mat4(1.0f);
}


bool Entity::CheckCollision(Entity* other)
{
    if (other == this) return false;
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0)
    {
        lastCollision = other->entityType;
        return true;
    }
    return false;
}


void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if (object->entityType == ENEMY) {
                    gameover = true;
                    isActive = false;
                }
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (object->entityType == ENEMY) {
                    object->isActive = false;
                }
            }
        }
    }
}


void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }

            if (object->entityType == ENEMY) {
                isActive = false;
                gameover = true;
            }
        }
    }
}


bool Entity::SensorCheckCollision(Entity* other, glm::vec3 sensorPoint) {

    if (other == this) return false;
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(sensorPoint.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(sensorPoint.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0)
    {
        return true;
    }

    return false;
}

void Entity::AIWalker(Entity* platforms, int platformCount) {
    switch (aiState) {
    case IDLE:
        aiState = WALKING;
        break;
    case WALKING:
        CheckCollisionsX(platforms, platformCount);
        CheckCollisionsY(platforms, platformCount);

        if (collidedRight) {
            x = -1;
            //y *= -1;
        }
        if (collidedLeft) {
            x = 1;
            //y *= -1;
        }
        movement = glm::vec3(x, 0, 0);
        break;
    }
    int x = 1;
    int y = 1;
    
    //glm::vec3 sensorRight = glm::vec3(position.x + 0.06f, position.y - 0.06f, 0.0f);
    //glm::vec3 sensorLeft = glm::vec3(position.x - 0.06f, position.y - 0.06f, 0.0f);
    //CheckCollisionsX(platforms, platformCount);
    //CheckCollisionsY(platforms, platformCount);
    //if (collidedTop) {
    //    y = -1;
    //}
    //else if (collidedBottom) {
    //    y = 1;
    //}
    //if (collidedRight) {
    //    x = -1;
    //}
    //else if (collidedLeft) {
    //    x = 1;
    //}
    //for (int i = 1 ; i < platformCount - 1; i++) {
    //    Entity* platform = &platforms[i];
    //    if ((platform->position.x != (platforms[i-1].position.x + 1)) || (platform->position.x != (platforms[i + 1].position.x + 1)))
    //        if (!SensorCheckCollision(platform, sensorRight)) {
    //            x = -1;
    //            break;
    //        }
    //    if ((platform->position.x != (platforms[i - 1].position.x - 1)) || (platform->position.x != (platforms[i + 1].position.x - 1)))
    //        if (!SensorCheckCollision(platform, sensorLeft)) {
    //             x = 1;
    //            break;
    //        }
    //}
    //if (position.x >= 3.9) {
    //    movement = glm::vec3(-1, 0, 0);
    //}
    //else if (position.x < 1.5f) {
    //    movement = glm::vec3(1, 0, 0);
    //}

}


void Entity::AIWaitAndGo(Entity* player, Entity* platforms, int platformCount) {
    switch (aiState) {
    case IDLE:
        if (glm::distance(position, player->position) < 5.0f) {
            aiState = WALKING;
        }
        break;
    case WALKING:
        CheckCollisionsX(platforms, platformCount);
        CheckCollisionsY(platforms, platformCount);
        if (collidedRight) {
            if (collidedBottom) {
                movement = glm::vec3(-1, 0, 0);
            }
            //movement = glm::vec3(-1, 0, 0);
        }
        else if (collidedLeft) {
            if (collidedBottom) {
                movement = glm::vec3(1, 0, 0);
            }
            //movement = glm::vec3(1, 0, 0);
        }
        else if (movement == glm::vec3(0)) {
            movement = glm::vec3(1, 0, 0);
        }
        //movement = glm::vec3(x, 0, 0);
        break;
    case ATTACKING:
        break;
    }

}


void Entity::AIPatrol(Entity* player, Entity* platforms, int platformCount) {
    switch (aiState) {
    case IDLE:
        break;

    case WALKING:
        CheckCollisionsX(platforms, platformCount);
        CheckCollisionsY(platforms, platformCount);
        //if (collidedBottom) {
        //    y = 1;
        //}

        if (collidedTop) {  
            if (collidedLeft) {
                x = 1;

            }
            else if (x != 1) {
                x = -1;
                y = 1;
            }
        
            else if (x == 1 && collidedRight) {
                y = -1;
            }
        }
        else if (collidedBottom) {
            x = 0;
            y = 1;
        }

        else if (collidedLeft) {
            x = 1;
        }
        else if (movement == glm::vec3(0)) {
            x = 0;
            y = -1;
        }
        movement = glm::vec3(x, y, 0);
        break;
    case ATTACKING:
        break;
    }
}

void Entity::AI(Entity* player, Entity* platforms, int platformCount) {
    switch (aiType) {
    case WALKER:
        AIWalker(platforms, platformCount);
        break;

    case WAITANDGO:
        AIWaitAndGo(player, platforms, platformCount);
        break;

    case PATROL:
        AIPatrol(player, platforms, platformCount);
        break;
    }
}



void Entity::Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemyCount)
{
    if (isActive == false) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;




    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
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
            animIndex = 0;
        }
    }


    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }

    //position += movement * speed * deltaTime;
    velocity.x = movement.x * speed;
    if (accelaration.y == 0.0f) {
        velocity.y = movement.y * speed;
    }

    velocity += accelaration * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(platforms, platformCount);// Fix if needed
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(platforms, platformCount);// Fix if needed

    //position.y += movement.y;

    if (entityType == PLAYER) {
        // check enemy collisions
        CheckCollisionsY(enemies, enemyCount);
        CheckCollisionsX(enemies, enemyCount);

    }

    if (entityType == ENEMY) {
        AI(player, platforms, platformCount);
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
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



void Entity::Render(ShaderProgram* program) {

    if (isActive == false) return;


    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    //if (entityType == ENEMY) {
    //    vertices = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25};
    //}

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}