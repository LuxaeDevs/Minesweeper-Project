#include <iostream>
#include <vector>
#include <raylib.h>
#include <raymath.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <filesystem>
#include <fstream>

using std::cout;
using std::string;
using std::vector;
enum ScreenTab
{
    MENU,
    SETTINGS,
    GAME
};
enum Condition
{
    IDLE,
    CLICKED,
    MINE,
    FLAGGED
};
struct Settings
{
    struct BoardInfo
    {
        int size = 10;
        Vector2 start = {30.0f, 30.0f};
        float screenSize = 300.0f;
        float fieldRatio = screenSize / size;
        int mineAmount = (size * size) / 10;
        int mineMax = (size * size) / 3;
        Color fieldColor = WHITE;
        Color mineColor = RED;
    };
    BoardInfo board;
    struct Screen
    {
        float height = 480.0f;
        float width = 800.0f;
        int FPS = 120;
    };
    ScreenTab atScreen = GAME;
    Screen screen;
};
struct Coordinate
{
    Color screenColor;
    Vector2 position;
    Vector2 direction;
    Coordinate(Vector2 position, Color screenColor)
    {
        this->position = position;
        this->screenColor = screenColor;
    }
    Coordinate()
    {
    }
};
struct RecObject : Coordinate
{
    Vector2 size;
    RecObject(Rectangle rec, Color screenColor) : Coordinate({rec.x, rec.y}, screenColor)
    {
        size = {rec.width, rec.height};
    }
    RecObject() : Coordinate()
    {
    }
    Rectangle GetRectangleData()
    {
        return {position.x, position.y, size.x, size.y};
    }
};
template <typename T>
void ClickCascade(vector<vector<T>> &mineField, T &field, Settings &game);
bool CheckIfClick(Vector2 mousePos, Vector2 position, Vector2 size);
struct Field : Coordinate
{

    Condition condition;
    int mineNearby = 0;
    Field(Vector2 position, Color screenColor, Condition condition) : Coordinate(position, screenColor)
    {
        this->condition = condition;
    }
    Field() : Coordinate()
    {
    }
    void SetMine(Settings &game)
    {
        condition = MINE;
        screenColor = game.board.mineColor;
    }

    void OnMouseClick()
    {
    }
    Rectangle RecScreenSize(Settings &game)
    {
        return {game.board.start.x + (position.x * game.board.fieldRatio), game.board.start.y + (position.y * game.board.fieldRatio), (game.board.fieldRatio), (game.board.fieldRatio)};
    }
    void DrawField(Settings &game, Color outline, Vector2 mousePos, vector<vector<Field>> &mineField)
    {
        Rectangle check = RecScreenSize(game);
        Vector2 appPos = {check.x, check.y};
        Vector2 frameSize = {check.width, check.height};
        Vector2 size = {frameSize.x - frameSize.x / 10, frameSize.y - frameSize.y / 10};
        if (CheckIfClick(mousePos, appPos, size))
            ClickCascade<Field>(mineField, *this, game);
        DrawRectangleV(appPos, frameSize, outline);
        DrawRectangleV(appPos, size, screenColor);
        if (mineNearby == 0 || condition != CLICKED)
            return;
        DrawText(std::to_string(mineNearby).c_str(), appPos.x + MeasureText(std::to_string(mineNearby).c_str(), size.y) / 2,
                 appPos.y, size.y, BLACK);
    }
};
void SetField(vector<vector<Field>> &mineField, Settings &game);
struct Button : RecObject
{
    string text;
    Color textColor;
    Button(Rectangle rec, Color colorInfo, Color textColor, string text) : RecObject(rec, colorInfo)
    {
        this->text = text;
        this->textColor = textColor;
    }
    bool ButtonCentered()
    {
        bool centered = GuiButton(GetRectangleData(), "");
        int fontSize = size.y;
        if (MeasureText(text.c_str(), fontSize) > size.x)
        {
            fontSize = (int)(fontSize * size.x / MeasureText(text.c_str(), fontSize));
        }
        DrawText(text.c_str(), position.x + size.x / 2 - MeasureText(text.c_str(), fontSize) / 2, position.y, fontSize, textColor);
        return centered;
    }
    void ResetGame(vector<vector<Field>> &mineField, Settings &game)
    {
        if (!ButtonCentered())
            return;
        SetField(mineField, game);
    }
};