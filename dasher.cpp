#include "raylib.h"

int main()
{
    //window dimensions
    const int WindowWidth{512};
    const int WindowHeight{380};
    //initialize window
    InitWindow(WindowWidth, WindowHeight, "Dapper-Dasher");
    
    //acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

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

    //jump velocity (pixels/second)
    const int jumpVelocity{-600};

    //animation frame
    int frame{};

    //amount of time before we update the animation frame
    const float updateTime{1.0/12.0};

    float runningTime{};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //delta time (time since last frame)
        const float deltaTime{GetFrameTime()};

        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        //ground check
        if (scarfyPos.y >= WindowHeight - scarfyRec.height)
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            velocity += gravity * deltaTime;
            isInAir = true;
        }

        //jump on space key pressed
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVelocity;
        }
        
        //update the position
        scarfyPos.y += velocity * deltaTime;

        // update running time
        runningTime += deltaTime;

        if (runningTime >= updateTime)
        {
            runningTime = 0;
            //update animation frame
            scarfyRec.x = frame * scarfyRec.width;
            frame++;
            if (frame > 5)
            {
                frame = 0;
            }
        }

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
}