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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, first_player, second_player, ball, projectionMatrix;



glm::vec3 first_player_position = glm::vec3(0, 0, 0);
glm::vec3 first_player_movement = glm::vec3(0, 0, 0);
glm::vec3 second_player_position = glm::vec3(0, 0, 0);
glm::vec3 second_player_movement = glm::vec3(0, 0, 0); 
glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(1.0f, 1.0f, 0);


float player_speed = 4.0f;
float ball_speed = 0.0f;
float ball_next_y = 1.0f;
float ball_next_x = 1.0f;

bool collision = false;


GLuint brick_textureID;
GLuint ball_textureID;


// texture function (fully copy pasted from slides, no changes made)
GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}



void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");



    viewMatrix = glm::mat4(1.0f);
    first_player = glm::mat4(1.0f);
    second_player = glm::mat4(1.0f);
    ball = glm::mat4(1.0f);

    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);



    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    brick_textureID = LoadTexture("brick.png");
    ball_textureID = LoadTexture("x_wing.png");

}


bool collision_check()  {   

    float xdist = fabs(first_player_position.x  - ball_position.x) - (( 0.5f + 0.4f ) / 2.0f);
    float ydist = fabs(first_player_position.y  - ball_position.y) - (( 0.5f + 1.4f ) / 2.0f);
    float second_xdist = fabs(second_player_position.x - ball_position.x) - ((0.5f + 0.4f) / 2.0f);
    float second_ydist = fabs(second_player_position.y - ball_position.y) - ((0.5 + 1.4f) / 2.0f);

     //it hits brick
    //if ((xdist < 0 && ydist < 0)) {
    //    // Colliding!
    //    ball_next_x = -1.0f;
    //    return true;
    //}
    if (second_xdist < 0 && second_ydist < 0) {
        // Colliding!
        ball_next_x = -1.0f;
        return true;
    }
    


    //// if hits boundary
    if (ball_position.y >= 3.5f) {

        ball_next_y = -1.0f;
        return true;
    }

    else if (ball_position.y <= -3.5f) {
        ball_next_y = 1.0f;
        return true;
    }
    
    // temp. after collusion is fixed, make it so ends game is touches sides
    if (ball_position.x >= 4.8f) {

        ball_next_x = -1.0f;
        return true;
    }

    else if (ball_position.x <= -4.8f) {
        ball_next_x = 1.0f;
        return true;
    }


}


void ProcessInput() {

    first_player_movement = glm::vec3(0);
    second_player_movement = glm::vec3(0);
    ball_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // move left
                        break;
                    case SDLK_RIGHT:
                        // move right
                        break;
                    case SDLK_SPACE:
                        ball_speed = 3.0f;
                        break;
                }
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);


    // accounts for first player's vertical movement
    if (keys[SDL_SCANCODE_W]) {
        if (first_player_position.y <= 3.0f) {
            first_player_movement.y = 1.0f;
        }
    }
    else if (keys[SDL_SCANCODE_S]) {
        if (first_player_position.y >= -3.0f) {
            first_player_movement.y = -1.0f;
        }
    }
    // accounts for second player's vertical movement
    if (keys[SDL_SCANCODE_UP]) {
        if (second_player_position.y <= 3.0) {
            second_player_movement.y = 1.0f;
        }
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        if (second_player_position.y >= -3.0) {
            second_player_movement.y = -1.0f;
        }
    }



    // stablizes speed
    if (glm::length(first_player_movement) > 1.0f) {
        first_player_movement = glm::normalize(first_player_movement);
    }

    if (glm::length(second_player_movement) > 1.0f) {
        second_player_movement = glm::normalize(second_player_movement);
    }

    if (glm::length(ball_movement) > 1.0f) {
        ball_movement = glm::normalize(ball_movement);
    }



    ball_movement.x += ball_next_x;
    ball_movement.y = ball_next_y;








}



float lastTicks = 0.0f;

float ball_moves_forward = 0.0f;
void Update()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    
    //player_x += 1.0f * deltaTime;
    //player_rotate += -90.0f * deltaTime;

    //ball_moves_forward += .08f;



    first_player_position += first_player_movement * player_speed * deltaTime;
    second_player_position += second_player_movement * player_speed * deltaTime;
    ball_position += ball_movement * ball_speed * deltaTime;
    

    first_player = glm::mat4(1.0f);
    //first_player = glm::scale(first_player, glm::vec3(0.2f, 1.0f, 0.0f));
    first_player = glm::translate(first_player, glm::vec3(-4.5f, 0.0f, 0.0f));

    first_player = glm::translate(first_player, first_player_position);



    second_player = glm::mat4(1.0f);
    /*second_player = glm::scale(second_player, glm::vec3(0.2f, 1.0f, 0.0f));*/
    second_player = glm::translate(second_player, glm::vec3(4.5f, 0.0f, 0.0f));

    second_player = glm::translate(second_player, second_player_position);



    ball = glm::mat4(1.0f);
    
    //ball = glm::translate(ball, glm::vec3(9.1f, 7.0f, 0.0f));
    //ball = glm::rotate(ball, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));




    collision = collision_check();

    ball = glm::translate(ball, (ball_position));

    //first_player = glm::scale(first_player, glm::vec3(0.2f, 1.0f, 0.0f));
    //second_player = glm::scale(second_player, glm::vec3(0.2f, 1.0f, 0.0f));
    //ball = glm::scale(ball, glm::vec3(0.5f, 0.5f, 0.0f));



}


void Draw_paddles(glm::mat4 paddle) {
    program.SetModelMatrix(paddle);
    glBindTexture(GL_TEXTURE_2D, brick_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Draw_ball() {
    program.SetModelMatrix(ball);
    glBindTexture(GL_TEXTURE_2D, brick_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}






void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float second_vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    float vertices[] = { -0.2, -0.7, .2, -0.7, 0.2, 0.7, -0.2, -0.7, 0.2, 0.7, -0.2, .7 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };




    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    Draw_paddles(first_player);
    Draw_paddles(second_player);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, second_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    Draw_ball();



    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);

}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}