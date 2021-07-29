#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    originalPosition = glm::vec3(0);
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
                    decrease_life = true;
                    reset = true;
                }
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (object->entityType == ENEMY) {
                    if (object->aiType == PATROL) {
                        decrease_life = true;
                        reset = true;
                    }
                    else {
                        object->isActive = false;
                    }
                }
            }

            if (object->entityType == KEY) {
                keyAquired = true;
                object->isActive = false;
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
                decrease_life = true;
                reset = true;
            }
            if (object->entityType == KEY) {
                keyAquired = true;
                object->isActive = false;
            }
        }
    }
}


void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::AIWalker(Map* map) {
    switch (aiState) {
    case IDLE:
        aiState = WALKING;
        break;
    case WALKING:
        CheckCollisionsX(map);
        CheckCollisionsY(map);

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




}


void Entity::AIWaitAndGo(Entity* player, Map* map) {
    switch (aiState) {
    case IDLE:
        if (glm::distance(position, player->position) < 8.0f) {
            aiState = WALKING;
        }
        break;
    case WALKING:
        CheckCollisionsX(map);
        CheckCollisionsY(map);
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
            movement = glm::vec3(-1, 0, 0);
        }
        //movement = glm::vec3(x, 0, 0);
        break;
    case ATTACKING:
        break;
    }

}


void Entity::AIPatrol(Entity* player, Map* map) {
    switch (aiState) {
    case IDLE:
        break;

    case WALKING:
        break;
    case ATTACKING:
        break;
    }
}

void Entity::AI(Entity* player, Map* map) {
    switch (aiType) {
    case WALKER:
        AIWalker(map);
        break;

    case WAITANDGO:
        AIWaitAndGo(player, map);
        break;

    case PATROL:
        AIPatrol(player, map);
        break;
    }
}



void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Entity* key, Entity* door, Map* map)
{
    if (isActive == false) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;




    if (animIndices != NULL) {
        if (entityType == PLAYER) {
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

        else if (entityType == DOOR) {
            if (player->keyAquired) {
                animIndex = 1;
            }
            else {
                animIndex = 0;
            }
        }

        else {
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
    CheckCollisionsY(map);// Fix if needed
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);// Fix if needed

    //position.y += movement.y;

    if (entityType == PLAYER) {
        // check enemy collisions
        CheckCollisionsY(objects, objectCount);
        CheckCollisionsX(objects, objectCount);
        CheckCollisionsY(key, 1);
        CheckCollisionsX(key, 1);

        if (!keyAquired) {
            CheckCollisionsX(door, 1);
        }

    }

    if (entityType == ENEMY) {
        AI(player, map);
    }

    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);
    if (entityType == PLAYER) {
        if (animIndices == animLeft) {
            modelMatrix = glm::scale(modelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
        }
        else if (animIndices == animRight) {
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }

    if (entityType == ENEMY) {
        if (movement.x < 0) {
            modelMatrix = glm::scale(modelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
        }
        else if (movement.x >= 0) {
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }


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