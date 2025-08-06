#pragma once
#include <vector>
#include <utility>

class SnakeGame {
public:
    SnakeGame();
    void Reset();
    void Step(int action);
    bool IsGameOver() const;
    int GetAppleCount() const;
    std::vector<float> GetSensors() const;
    void Draw(int x, int y, int w, int h) const;

private:
    void PlaceApple();
    int grid[10][10];
    int headX, headY, dir;
    int appleX, appleY;
    int snakeLen;
    int apples;
    int steps;
    bool gameOver;
    int stepsSinceApple;
<<<<<<< HEAD
    int stepsLeft;
=======
>>>>>>> origin/main
    std::vector<std::pair<int, int>> snake;
};