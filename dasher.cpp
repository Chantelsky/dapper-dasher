#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

int main()
{
    //window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper-Dasher");
    
    //acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    //nebula
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebData{
        {0.0, 0.0, nebula.width/8, nebula.height/8}, // Rectangle rec
        {windowDimensions[0], windowDimensions[1] - nebula.height/8}, // Vector2 pos
        0, //int frame
        1.0/12.0, //float updateTime
        0 // float runningTIme
        };

    AnimData neb2Data{
        {0.0, 0.0, nebula.width/8, nebula.height/8},
        {windowDimensions[0] + 300, windowDimensions[1] - nebula.height/8},
        0,
        1.0/16.0,
        0
    };

    //nebula X velocity (pixels/second)
    int nebulaVelocity{-200};

    //Scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    int velocity{0};
    bool isInAir;

    //jump velocity (pixels/second)
    const int jumpVelocity{-600};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //delta time (time since last frame)
        const float deltaTime{GetFrameTime()};

        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        //ground check
        if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)
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
        
        //update nebula position
        nebData.pos.x += nebulaVelocity * deltaTime;
        // update second nebula's position
        neb2Data.pos.x += nebulaVelocity * deltaTime;

        //update scarfy position
        scarfyData.pos.y += velocity * deltaTime;

        //update scarfy's animation frame
        if (!isInAir)
        {
            // update running time
            scarfyData.runningTime += deltaTime;
            if (scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0;
                //update animation frame
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;
                if (scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
            }
        }

        //update nebula animation frame
        nebData.runningTime += deltaTime;
        if (nebData.runningTime >= nebData.updateTime)
        {
            nebData.runningTime = 0.0;
            nebData.rec.x = nebData.frame * nebData.rec.width;
            nebData.frame++;

            if (nebData.frame > 7)
            {
                nebData.frame = 0;
            }
        }

        //update nebula2 animation frame
        neb2Data.runningTime += deltaTime;
        if (neb2Data.runningTime >= neb2Data.updateTime)
        {
            neb2Data.runningTime = 0.0;
            neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
            neb2Data.frame++;

            if (neb2Data.frame > 7)
            {
                neb2Data.frame = 0;
            }
        }

        //draw nebula
        DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
        //draw second nebula
        DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, WHITE);

        //draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}