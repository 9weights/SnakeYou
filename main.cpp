#include "raylib.h"
#include <iostream>
#include <vector>
#include <array>

struct VecInt
{
    int x{};
    int y{};
};


enum class State
{
    empty,
    wall,
    snake,
    food,
    poison,
    outofbounds,
    COUNT
};


class Snake
{
private:
    VecInt m_headPos{};
    VecInt m_heading{};
public:
    bool m_canMove{true};
    void askInput()
    {
        if (IsKeyDown(KEY_UP)){m_heading = {0,1};} // ignore case that multiple keys pressed
        if (IsKeyDown(KEY_LEFT)){m_heading = {-1,0};}
        if (IsKeyDown(KEY_DOWN)){m_heading = {0,-1};}
        if (IsKeyDown(KEY_RIGHT)){m_heading = {1,0};}
    }

    void moveSnake()
    {
        m_headPos.x += m_heading.x;
        m_headPos.y += m_heading.y;
    }
    
    VecInt getHeadPos() const {return m_heading;}

};


void renderScene(std::vector<State>& boardState, const VecInt& gridSize, int cellSize, Camera2D& camera)
{
    // convert 1d index to 2d coordinates for rectangle
    VecInt coord{};
    Rectangle pos{};
    BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        for (auto idx{0}; idx < boardState.size(); ++idx)
        {
            coord.x = idx % gridSize.x;
            coord.y = idx / gridSize.y;
            pos.x = coord.x * cellSize;
            pos.y = coord.y * cellSize;
            pos.width = cellSize;
            pos.height = cellSize;

            if (boardState[idx] == State::wall)
            {
                DrawRectangleRec(pos,GRAY);
            }
            else if (boardState[idx] == State::snake)
            {
                DrawRectangleRec(pos,BLUE);
            }
            else if (boardState[idx] == State::food)
            {
                DrawRectangleRec(pos,RED);
            }
            else if (boardState[idx] == State::poison)
            {
                DrawRectangleRec(pos,GREEN);
            }
            else if (boardState[idx] == State::outofbounds)
            {
            }
            else if (boardState[idx] == State::empty)
            {
            }
        }
        EndMode2D();
    EndDrawing();
}

int main()
{

    // Prelaunch initialisation and calculation

    // make board vars
    const int boardSizex{20};
    const VecInt screenDimensions{1000,800};
    const int cellSize{screenDimensions.x / boardSizex};
    const VecInt boardSize{boardSizex,screenDimensions.y / cellSize};
    const int nBoardCells{boardSize.x * boardSize.y};
    std::vector<State> boardState(nBoardCells);
    // initialise other vars
    Snake snake{};
    Camera2D camera{};
    camera.zoom = 1;
    camera.offset = {0,0};
    camera.target = {0,0};
    camera.rotation = {0};

    // welcome screen
    bool isMenu{false};
    while(isMenu)
    {

    }

    // gameplay loop
    bool isPlay {true};

    while (isPlay)
    {
        if (snake.m_canMove)
        {
            snake.askInput();
            snake.moveSnake();
        }

        renderScene(boardState, boardSize, cellSize, camera);
    }
    // snake move
    // if snake eat food or poison change size, if snake hit wall, game over
    // if gameover, show gameover screen
    // allow reset, or quit
    return 0;
}
