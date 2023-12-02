#include "raylib.h"
#include <iostream>
#include <vector>

class Grid
{
private:
    int m_iSize{};
    int m_jSize{};
public:
    Grid(int iSize, int jSize)
        : m_iSize{iSize}
        , m_jSize{jSize}
        {
        }
    int Coord2int(Vector2 coord){return coord.x * m_iSize + coord.x;}
    Vector2 int2Coord(int i){return Vector2{i % m_jSize, i / m_jSize}} // int div rounds down
};

class Snake
{
private:
    int m_size{0};
    int m_head{};
    std::vector<int> m_body{};
public:
    Snake(int startPos)
        : m_head{startPos}
    {
    }
    
    void eatFood()
    {

    }
    void eatPoison()
    {

    }    

    void slither(Grid grid)
    {
        Vector2 coord = grid.int2Coord(m_head);
        if (IsKeyDown(KEY_LEFT)){coord.x}
        if (IsKeyDown(KEY_RIGHT)){}
        if (IsKeyDown(KEY_UP)){}
        if (IsKeyDown(KEY_DOWN)){}
         


    }
    // move head
    // if head on fruit or poison enum, grow or die
    // if head on wall or poison, die (maybe poison cuts body in half and turns into wall)

};

void spawnFruit(){}



int main()
{
    Vector2 startPosition{2,2};
    Snake snake{startPosition};
    bool active{true};
    float deltat{1};
    float spawnTimer{1};
    float gameTime{0};
    while(active)
    {
        snake.slither();
        if (static_cast<int>(gameTime) % static_cast<int>(spawnTimer == 0)){
            
            spawnFruit();
        }
        
        
        gameTime += deltat;

    }
    
    return 0;
}
