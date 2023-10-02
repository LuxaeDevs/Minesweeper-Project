#include "functions.hpp"

int main()
{
    Settings game;
    InitWindow((int)game.screen.width, (int)game.screen.height, "Minesweeper");
    InitAudioDevice();
    SetTargetFPS(game.screen.FPS);
    vector<vector<Field>> mineField;
    SetField(mineField, game);
    Button resetButton({game.screen.width / 2 - 100, game.screen.height - 75, 200, 50}, GREEN, BLACK, "RESET");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        switch (game.atScreen)
        {
            cout << "uwu";
        case GAME:
            DrawFields(mineField, game, GetMousePosition());
            resetButton.ResetGame(mineField, game);
            break;

        default:
            break;
        }
        EndDrawing();
    }
    CloseWindow();
}