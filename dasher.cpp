#include "raylib.h"

int main()
{
    //window dimensions
    const int WindowWidth{512};
    const int WindowHeight{380};
    //initialize window
    InitWindow(WindowWidth, WindowHeight, "Dapper-Dasher");
    
    //acceleration due to gravity
    const int gravity{1};

    //textures
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = WindowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = WindowHeight - scarfyRec.height;

    int velocity{0};

    bool isInAir;
    //jump velocity
    const int jumpVelocity{-22};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        //ground check
        if (scarfyPos.y >= WindowHeight - scarfyRec.height)
        {
            isInAir = false;
            velocity = 0;
        }
        else
        {
            isInAir = true;
            velocity += gravity;
        }

        //jump on space key pressed
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVelocity;
        }
        
        scarfyPos.y += velocity;
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
}