#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stddef.h>

int titleScreen();
int gameScreen();
int gameOverScreen(char* winner);


struct Player
{
    int id;
    int posX;
    int posY;
    int radius;
    int level;
    Color color;
};


void animateDice(Rectangle *diceRect, int i, int *diceVal)
{
    *diceVal = i + 1;
    diceRect->x = (i * 377 / 6.0f);
}

void spin(Texture2D *dice, Rectangle *sourceRec, Vector2 *position, int *diceVal)
{
    
    int frames[6] = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 10; i++)
    {
        int randomIndex = GetRandomValue(0, 5);
        animateDice(sourceRec, randomIndex, diceVal);
        DrawTextureRec(*dice, *sourceRec, *position, WHITE);
    }
}

void movePlayerToLevel(struct Player *pl, int level, int widthOfEachBlock, int heightOfEachBlock, int screenHeight)
{
    pl->posX = (widthOfEachBlock / 1.5) + (widthOfEachBlock / 1.5) * 0.46 * ((level - 1) % 10) + (widthOfEachBlock / 1.5) * ((level - 1) % 10);
    pl->posY = (screenHeight - heightOfEachBlock / 1.5) - (widthOfEachBlock / 1.5) * 0.46 * ((level - 1) / 10) - (widthOfEachBlock / 1.5) * ((level - 1) / 10);
    pl->level = level;
}



void movePlayer(struct Player *pl, int i, int widthOfEachBlock, int heightOfEachBlock, int screenHeight, int *arr)
{
    int levelToBeReached = pl->level +i;
    if(levelToBeReached<=100){
        movePlayerToLevel(pl, pl->level + i, widthOfEachBlock, heightOfEachBlock, screenHeight);
    };
    if (arr[(pl->level) - 1] != 0)
    {
        movePlayerToLevel(pl, (arr[(pl->level) - 1]), widthOfEachBlock, heightOfEachBlock, screenHeight);
    }
}

struct Player *checkWinner(struct Player *players, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (players[i].level >= 100)
        {
            return &players[i];
        };
    }
    return NULL;
};

int gameOverScreen(char* winner){
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
    DrawText("Game Over", GetScreenWidth()/2-100, GetScreenHeight()/2-180, 40, BLACK);
    //DrawText("Snake & Ladder", GetScreenWidth()/2-150, GetScreenHeight() / 2 - 150, 40, BLACK);
    //DrawText("Press the button to play", GetScreenWidth()/2-120, GetScreenHeight() / 2 - 90, 20, BLACK);
    DrawText(winner, GetScreenWidth()/2-160, GetScreenHeight() / 2 - 130, 30, BLACK);
    DrawText("wins the game", GetScreenWidth()/2-160+160, GetScreenHeight() / 2 - 130, 30, BLACK);

    // Draw "Restart" button
    Rectangle restartButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 50, 200, 100};
    DrawRectangleRec(restartButton, GRAY);
    DrawText("Restart", restartButton.x + 30, restartButton.y + 30, 30, BLACK);
    EndDrawing();
    // Check for mouse input to detect button click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();
        if (CheckCollisionPointRec(mousePosition, restartButton)) {
            // If "Restart" button is clicked, reset game state and transition to title screen
            titleScreen();
            CloseWindow();
        }
    }
    }
    CloseWindow();
    
    return 0;
};

int gameScreen(void)
{
    int screenHeight, screenWidth;
    int playerTurn=0;
    int count =0;
    char* buttonText;
    char Spin[]="Spin";
    char Stop[]="Stop";
    char* playerTexts[]={"Player 1","Player 2","Player 3"};
    buttonText=Spin;
    char diceValObtained[2];
    screenHeight = 600;
    screenWidth = 800;
    int heightOfEachBlock = screenHeight / 10;
    int widthOfEachBlock = screenHeight / 10;
    float diceSize = 377 / 6;
    struct Player players[3] = {
        {1, widthOfEachBlock / 1.5, (screenHeight - heightOfEachBlock / 1.5), heightOfEachBlock / 2.5, 1, DARKGRAY},
        {2, widthOfEachBlock / 1.5, (screenHeight - heightOfEachBlock / 1.5), heightOfEachBlock / 2.5, 1, DARKBLUE},
        {3, widthOfEachBlock / 1.5, (screenHeight - heightOfEachBlock / 1.5), heightOfEachBlock / 2.5, 1, DARKGREEN},
    };
    int arr[] = {
        0, 0, 0, 0, 0, 0, 29, 0, 0, 0,
        0, 24, 0, 0, 37, 0, 0, 0, 0, 0,
        0, 0, 41, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 71, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 86, 0, 0,
        0, 0, 0, 0, 0, 90, 0, 0, 0, 0,
        0, 0, 0, 75, 0, 0, 0, 0, 0, 0,
        0, 91, 0, 95, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 97, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Rectangle buttonRec = {630, 390, 120, 50};
    bool buttonPressed = false;
    Font defaultFont = GetFontDefault();
    SetTargetFPS(5);
    Image img = LoadImage("assets/board.png");
    ImageResize(&img, 600, 600);
    Texture2D background = LoadTextureFromImage(img);
    Texture2D dice = LoadTexture("assets/dice.png");
    Rectangle sourceRec = {0, 0, 377 / 6.0f, 377 / 6.0f};
    Vector2 position = {660, 600 - (377 / 6)};
    int diceVal = 0;

    while (!WindowShouldClose())
    {
        
        struct Player *winner = checkWinner(players, 3);

        if (CheckCollisionPointRec(GetMousePosition(), buttonRec))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                SetTargetFPS(5);
                buttonPressed = true;
            }else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                if(buttonPressed){
                    buttonPressed=false;
                    SetTargetFPS(1);
                    movePlayer(&players[playerTurn], diceVal, widthOfEachBlock, heightOfEachBlock, screenHeight, arr);
                    playerTurn=(playerTurn+1)% (int)(sizeof(players)/sizeof(players[0]));
                }

            }
            
        }
        // else
        // {
        //     buttonPressed = false;
        // };
        sprintf(diceValObtained, "%d", diceVal);
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);
        if (winner)
        {
            gameOverScreen(playerTexts[winner->id -1]);
        };
        for (int i = 0; i < 3; i++)
        {
            DrawCircle(players[i].posX, players[i].posY, players[i].radius, players[i].color);
        };
        DrawText(playerTexts[playerTurn], 690, 50, 25, players[playerTurn].color);
        DrawText("Turn: ", 600, 50, 25, BLACK);
        DrawRectangleRec(buttonRec, buttonPressed ? DARKPURPLE : PURPLE);
        DrawText(buttonText, 650, 395, 40, BLACK);
        DrawText(diceValObtained, 700, 140, 40, RED);
        for (int i = 0; i < sizeof(players)/sizeof(players[0]); i++)
        {
            DrawText(playerTexts[i],610,200 + i*40,30,players[i].color);
            if(playerTurn==i){
                DrawCircle(760, 217+i*40, 15, players[i].color);
            }
        };
        
        
        if(buttonPressed){
            buttonText=Stop;
            spin(&dice, &sourceRec, &position, &diceVal);
        }else{
            buttonText=Spin;
        };
        sourceRec.x = diceVal==0?0:((diceVal-1) * 377 / 6.0f);
        DrawTextureRec(dice, sourceRec, position, WHITE);
        
        
        SetTargetFPS(10);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}

int titleScreen(){
    Image snake = LoadImage("assets/snake.png");
    Image ladder = LoadImage("assets/ladder.png");
    ImageResize(&ladder,200,200);
    ImageResize(&snake,200,200);
    Texture2D snakeT = LoadTextureFromImage(snake);
    Texture2D ladderT = LoadTextureFromImage(ladder);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
    DrawText("Snake & Ladder", GetScreenWidth()/2-150, GetScreenHeight() / 2 - 150, 40, BLACK);
    DrawText("Press the button to play", GetScreenWidth()/2-120, GetScreenHeight() / 2 - 90, 20, BLACK);
    DrawTexture(snakeT,10,120,WHITE);
    DrawTexture(ladderT,GetScreenWidth()-200,100,WHITE);

    // Draw "Play" button
    Rectangle playButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 50, 200, 100};
    DrawRectangleRec(playButton, GRAY);
    DrawText("Play", playButton.x + 70, playButton.y + 30, 30, BLACK);
    EndDrawing();

    // Check for mouse input to detect button click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();
        if (CheckCollisionPointRec(mousePosition, playButton)) {
            // If "Play" button is clicked, transition to game screen
            gameScreen();
            CloseWindow();
        }
    }
    }
    CloseWindow();
    return 0;
};




int main(){
    int screenHeight = 600;
    int screenWidth = 800;
    InitWindow(screenWidth, screenHeight, "Snake & Ladder");
    titleScreen();
    return 0;
}