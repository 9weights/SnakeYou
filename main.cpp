#include "raylib.h"
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <thread>



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
    VecInt m_heading{};
    bool m_isAlive{true};
    std::vector<int> m_position{}; //small enough to keep on stack for now
    const VecInt& m_gridSize{};
    int m_passTillMove{50};

public:
    Snake(const VecInt& pos, const VecInt& gridSize)
        : m_gridSize{gridSize}
        , m_position{}
    {
        m_position.emplace_back(pos.x + pos.y * m_gridSize.x);
    }
    bool m_canMove{true};
    void askInput()
    {
        if (IsKeyDown(KEY_UP)){m_heading = {0,-1};} // ignore case that multiple keys pressed, negative because raylib is annoying >:(
        if (IsKeyDown(KEY_LEFT)){m_heading = {-1,0};}
        if (IsKeyDown(KEY_DOWN)){m_heading = {0,1};}
        if (IsKeyDown(KEY_RIGHT)){m_heading = {1,0};}
    }

    void moveSnake(std::vector<State>& boardState)
    {
        static int steps{0};
        steps++;
        if (steps < m_passTillMove)
        {
            return;
        }
        steps = 0;
        // update snake head
        int newPosition = m_position[m_position.size() - 1] + m_heading.x + m_heading.y * m_gridSize.x;
        // Check if other entities occupy space and handle
        switch (boardState[newPosition])
        {
        case State::wall: 
        case State::snake:
            m_isAlive = false;
            break;
        case State::empty:
            boardState[m_position[0]] = State::empty;
            if (m_position.size() != 1)
            {
                for (auto idx{0}; idx < m_position.size() - 1; ++idx) // off by one?
                {
                    m_position[idx] = m_position[idx + 1];
                }
            }
            m_position[m_position.size() - 1] = newPosition;
            break;
        case State::food:
            boardState[m_position[0]] = State::empty;
            m_position.emplace_back(newPosition);
            break;
        case State::poison: // later make poison cut snake in half and turn into walls
            break;
        }
        // update board
        boardState[m_position[m_position.size() - 1]] = State::snake;

    }
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
            coord.y = idx / gridSize.x;
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
                int test{};
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
    const int boardSizex{100};
    const VecInt targetScreenDimensions{1000,800};
    const int cellSize{targetScreenDimensions.x / boardSizex};
    const VecInt boardSize{boardSizex,targetScreenDimensions.y / cellSize};
    const VecInt screenDimensions{targetScreenDimensions.x, boardSize.y * cellSize};
    const int nBoardCells{boardSize.x * boardSize.y};
    std::vector<State> boardState(nBoardCells);
    // initialise other vars
    Snake snake{VecInt {boardSize.x/2,boardSize.y/2}, boardSize};
    Camera2D camera{};
    camera.zoom = 1;
    camera.offset = {0,0};
    camera.target = {0,0};
    camera.rotation = {0};
    InitWindow(screenDimensions.x, screenDimensions.y, "Snake GUI");

    // welcome screen
    bool isMenu{false};
    while(isMenu)
    {

    }

    // gameplay loop
    bool isPlay {true};

    while (!WindowShouldClose())
    {
        if (snake.m_canMove)
        {
            snake.askInput();
            snake.moveSnake(boardState);
            {
                using namespace std::this_thread;
                using namespace std::chrono_literals;
                //sleep_for(0.5s);
            }
        }
        boardState[165] = State::food;
        renderScene(boardState, boardSize, cellSize, camera); // later add interpolation
    }
    return 0;
}
