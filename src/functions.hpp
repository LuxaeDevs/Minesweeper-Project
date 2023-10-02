#include "struct.hpp"
int CheckMineNearby(Field &field, vector<vector<Field>> &mineField, Settings &game)
{
    int result = 0;
    for (int y = -1; y < 2; y++)
    {
        for (int x = -1; x < 2; x++)
        {
            Vector2 checkPos = {field.position.x + x,
                                field.position.y + y};
            if ((checkPos.x == field.position.x && checkPos.y == field.position.y) || checkPos.x < 0 ||
                checkPos.x >= game.board.size || checkPos.y < 0 || checkPos.y >= game.board.size)
                continue;
            Field checkField = mineField[(int)checkPos.y][(int)checkPos.x];
            if (checkField.condition != MINE)
                continue;
            result++;
        }
    }
    return result;
}
template <typename T>
void ClickCascade(vector<vector<T>> &mineField, T &field, Settings &game)
{
    if (field.condition != IDLE)
        return;
    field.condition = CLICKED;
    field.screenColor = GRAY;
    if (field.mineNearby > 0)
        return;
    for (int y = -1; y < 2; y++)
    {
        for (int x = -1; x < 2; x++)
        {
            Vector2 checkPos = {field.position.x + x,
                                field.position.y + y};
            if ((checkPos.x == field.position.x && checkPos.y == field.position.y) || checkPos.x < 0 ||
                checkPos.x >= game.board.size || checkPos.y < 0 || checkPos.y >= game.board.size)
                continue;

            ClickCascade<T>(mineField, mineField[(int)checkPos.y][(int)checkPos.x], game);
        }
    }
}
void CheckField(vector<vector<Field>> &mineField, Settings &game)
{
    for (int i = 0; i < (int)mineField.size(); i++)
    {
        for (int j = 0; j < (int)mineField[i].size(); j++)
        {
            if (mineField[i][j].condition == MINE)
                continue;

            mineField[i][j].mineNearby = CheckMineNearby(mineField[i][j], mineField, game);
        }
    }
}
bool CheckIfClick(Vector2 mousePos, Vector2 position, Vector2 size)
{
    if (mousePos.x < position.x || mousePos.x > position.x + size.x || mousePos.y < position.y || mousePos.y > position.y + size.y)
        return false;
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        return false;

    return true;
}
void SetField(vector<vector<Field>> &mineField, Settings &game)
{
    for (vector<Field> owo : mineField)
    {
        owo.clear();
    }
    mineField.clear();
    for (int y = 0; y < game.board.size; y++)
    {
        mineField.push_back(vector<Field>());
        for (int x = 0; x < game.board.size; x++)
        {
            mineField[y].push_back(Field({(float)x, (float)y}, game.board.fieldColor, IDLE));
        }
    }
    vector<Vector2> mineCoord;
    do
    {
        Vector2 temp = {(float)GetRandomValue(0, game.board.size - 1),
                        (float)GetRandomValue(0, game.board.size - 1)};

        bool overlapped = false;
        for (int i = 0; i < (int)mineCoord.size(); i++)
        {
            if (temp.x != mineCoord[i].x || temp.y != mineCoord[i].y)
                continue;
            overlapped = true;
            break;
        }
        if (overlapped)
            continue;
        mineCoord.push_back(temp);

    } while ((int)mineCoord.size() <= game.board.mineAmount);

    for (int i = 0; i < (int)mineCoord.size(); i++)
    {
        mineField[(int)mineCoord[i].y][(int)mineCoord[i].x].SetMine(game);
    }
    CheckField(mineField, game);
}
void DrawFields(vector<vector<Field>> &mineField, Settings &game, Vector2 mousePos)
{
    for (int i = 0; i < game.board.size; i++)
    {

        for (int j = 0; j < game.board.size; j++)
        {
            mineField[i][j].DrawField(game, BLACK, mousePos, mineField);
        }
    }
}