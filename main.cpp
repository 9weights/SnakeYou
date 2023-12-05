#include "raylib.h"
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include <stdlib.h>

int g_execTillStep{50};

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
    VecInt m_heading{1,0};
    std::vector<int> m_position{}; //small enough to keep on stack for now
    const VecInt& m_gridSize{};
    int m_passTillMove{g_execTillStep};

public:
    Snake(const VecInt& pos, const VecInt& gridSize)
        : m_gridSize{gridSize}
        , m_position{}
    {
        m_position.emplace_back(pos.x + pos.y * m_gridSize.x);
    }
    void askInput()
    {
        if (IsKeyDown(KEY_UP) && (m_heading.x != 0 && m_heading.y != -1)){m_heading = {0,-1};} // ignore case that multiple keys pressed, negative because raylib is annoying >:(
        if (IsKeyDown(KEY_LEFT) && (m_heading.x != -1 && m_heading.y != -0)){m_heading = {-1,0};}
        if (IsKeyDown(KEY_DOWN) && (m_heading.x != 0 && m_heading.y != 1)){m_heading = {0,1};}
        if (IsKeyDown(KEY_RIGHT) && (m_heading.x != 1 && m_heading.y != 0)){m_heading = {1,0};}
    }

    bool moveSnake(std::vector<State>& boardState)
    {
        static int steps{0};
        steps++;
        if (steps < m_passTillMove)
        {
            return false;
        }
        steps = 0;
        // update snake head
        int newPosition = m_position[m_position.size() - 1] + m_heading.x + m_heading.y * m_gridSize.x;
        // Check if other entities occupy space and handle
        switch (boardState[newPosition])
        {
        case State::wall: 
        case State::snake:
            return true;
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
        return false;
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

            if (boardState[idx] == State::wall) // should be switch case
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

void spawnFood(std::vector<State>& boardState, const VecInt& gridSize, int nosMaxFood)
{
    int nosFood{0};
    for (auto idx{0}; idx < boardState.size(); ++idx)
    {
        if (boardState[idx] == State::food){++nosFood;} // SHouldn't be iterating again here
    }
    int nosCells{gridSize.x * gridSize.y};
    while (nosFood < nosMaxFood)
    {
        int x{};
        x = rand()/((RAND_MAX + 1u)/nosCells); //
        switch(boardState[x])
        {
        case State::food:
        case State::wall:
        case State::snake:
            continue;
        case State::empty:
            boardState[x] = State::food;
            ++nosFood;
        }
    }
}

int main()
{

    // Prelaunch initialisation and calculation

    // make board vars
    constexpr int boardSizex{30};
    constexpr VecInt targetScreenDimensions{1000,800};
    constexpr int cellSize{targetScreenDimensions.x / boardSizex};
    constexpr VecInt boardSize{boardSizex,targetScreenDimensions.y / cellSize};
    constexpr VecInt screenDimensions{targetScreenDimensions.x, boardSize.y * cellSize};
    constexpr int nBoardCells{boardSize.x * boardSize.y};
    std::vector<State> boardState(nBoardCells);

    //Create walls
    std::array<VecInt, boardSize.y> leftWall{};
    for (auto idx{0}; idx < leftWall.size(); idx++) {leftWall[idx] = VecInt{0,idx};} 
    std::array<VecInt, boardSize.y> rightWall{};
    for (auto idx{0}; idx < rightWall.size(); idx++) {rightWall[idx] = VecInt{boardSize.x - 1,idx};} 
    std::array<VecInt, boardSize.x> bottomWall{};
    for (auto idx{0}; idx < bottomWall.size(); idx++) {bottomWall[idx] = VecInt{idx,0};} 
    std::array<VecInt, boardSize.x> topWall{};
    for (auto idx{0}; idx < topWall.size(); idx++) {topWall[idx] = VecInt{idx,boardSize.y - 1};} 

    int idx{};
    for (auto wall : leftWall)          // there has to be a better way of doing this
    {
        idx = wall.x + wall.y * boardSize.x;
        boardState[idx] = State::wall;
    }
    for (auto wall : rightWall)          
    {
        idx = wall.x + wall.y * boardSize.x;
        boardState[idx] = State::wall;
    }
    for (auto wall : topWall)          
    {
        idx = wall.x + wall.y * boardSize.x;
        boardState[idx] = State::wall;
    }

    for (auto wall : bottomWall)          
    {
        idx = wall.x + wall.y * boardSize.x;
        boardState[idx] = State::wall;
    }



    // initialise other vars
    Snake snake{VecInt {boardSize.x/2,boardSize.y/2}, boardSize};
    Camera2D camera{};
    camera.zoom = 1;
    camera.offset = {0,0};
    camera.target = {0,0};
    camera.rotation = {0};
    InitWindow(screenDimensions.x, screenDimensions.y, "Snake GUI");
    int nosMaxFruit{5};
    int stepsTillSpawn{100};
    int nosExecs{g_execTillStep * stepsTillSpawn};
    // gameplay loop
    bool isGameOver {false};

    while (!WindowShouldClose())
    {
        
        snake.askInput();
        isGameOver = snake.moveSnake(boardState);

        if (nosExecs == g_execTillStep * stepsTillSpawn)
        {
            spawnFood(boardState, boardSize, nosMaxFruit);
            nosExecs = 0;
        }
        ++nosExecs;
        renderScene(boardState, boardSize, cellSize, camera); // later add interpolation
        if (isGameOver)
        {
            while(!WindowShouldClose())
            {
                BeginDrawing();
                    DrawText("You lose", screenDimensions.x/3, screenDimensions.y/2, 100, RED); // text size does not scale with screen size :)
                EndDrawing();
                
            }
            break;
        }
    }
    
    return 0;
}
