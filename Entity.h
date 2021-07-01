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


enum  EntityType {PLAYER, OBSTACLE, GOAL};


class Entity {
public:
	EntityType lastCollision;
	EntityType entityType;
	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float speed;


	float height = 1;
	float width = 1;
	

	GLuint textureID;

	glm::mat4 modelMatrix;
	Entity();

	
	//, Entity* platforms, int platformCount
	void Update(float deltaTime, Entity* objects, int objectCount);
	void Update(float deltaTime, Entity* object);
	void Render(ShaderProgram* program, int index, int row, int cols);

	bool CheckCollision(Entity* other);
	void CheckCollisionsY(Entity* objects, int objectCount);
	void CheckCollisionsX(Entity* objects, int objectCount);

private:
	
};
