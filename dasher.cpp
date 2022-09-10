#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update runningtime
    data.runningTime += deltaTime;

    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;

        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }

    return data;
}

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
    const int sizeOfNebulae{6};
    AnimData nebulae[6]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    }

    float finishLine{ nebulae[sizeOfNebulae -1].pos.x };

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

    //background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float backgroundX{};

    //midground
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float midgroundX{};
    
    //foreground
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float foregroundX{};
    
    bool collision{};
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //delta time (time since last frame)
        const float deltaTime{GetFrameTime()};

        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //scroll the background
        backgroundX -= 20 * deltaTime;
        if (backgroundX <= background.width*2)
        {
            backgroundX = 0.0;
        }

        //scroll the midground
        midgroundX -= 40 * deltaTime;
        if (midgroundX <= -midground.width*2)
        {
            midgroundX = 0.0;
        }

        //scroll the foreground
        foregroundX -= 80 * deltaTime;
        if (foregroundX <= -foreground.width*2)
        {
            foregroundX = 0.0;
        }

        //draw background
        Vector2 background1Pos{backgroundX, 0.0};
        DrawTextureEx(background, background1Pos, 0.0, 2.0, WHITE);
        Vector2 background2Pos{backgroundX + background.width *2, 0.0};
        DrawTextureEx(background, background2Pos, 0.0 , 2.0, WHITE);

        //draw midground
        Vector2 midground1Pos{midgroundX, 0.0};
        DrawTextureEx(midground, midground1Pos, 0.0, 2.0, WHITE);
        Vector2 midground2Pos{midgroundX + midground.width *2, 0.0};
        DrawTextureEx(midground, midground2Pos, 0.0, 2.0, WHITE);

        //draw foreground
        Vector2 foreground1Pos{foregroundX, 0.0};
        DrawTextureEx(foreground, foreground1Pos, 0.0, 2.0, WHITE);
        Vector2 foreground2Pos{foregroundX + foreground.width *2, 0.0};
        DrawTextureEx(foreground, foreground2Pos, 0.0, 2.0, WHITE);
        
        //ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
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
        
        for (int i = 0; i < sizeOfNebulae; i++)
        {    
            //update the position of each nebula
            nebulae[i].pos.x += nebulaVelocity * deltaTime;
        }

        //update finish line
        finishLine += nebulaVelocity * deltaTime;

        //update scarfy position
        scarfyData.pos.y += velocity * deltaTime;

        //update scarfy's animation frame
        if (!isInAir)
        {
           scarfyData = updateAnimData(scarfyData, deltaTime, 5);
        }

        //update nebula animation frame
        for (int i = 0; i < sizeOfNebulae; i++)
        {     
           nebulae[i] = updateAnimData(nebulae[i], deltaTime, 7);
        }

        for (AnimData nebula : nebulae)
        {
            float padding{50};
            Rectangle nebulaRec{
                nebula.pos.x + padding,
                nebula.pos.y + padding,
                nebula.rec.width - 2* padding,
                nebula.rec.height - 2* padding
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebulaRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0]/4 , windowDimensions[1]/2, 40, RED);
        }
        else if (!collision && scarfyData.pos.x >= finishLine)
        {
            DrawText("You won!!!", windowDimensions[0]/4 , windowDimensions[1]/2, 40, GREEN);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                //draw nebula
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                //draw scarfy
                DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            }
        }



        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}