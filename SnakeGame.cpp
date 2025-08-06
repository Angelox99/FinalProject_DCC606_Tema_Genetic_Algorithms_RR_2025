#include "SnakeGame.h"
#include <algorithm>
#include <random>
#include <raylib.h>


SnakeGame::SnakeGame() { Reset(); }

void SnakeGame::Reset() {
<<<<<<< HEAD
    stepsSinceApple = 0;
    stepsLeft = 100; // Inicializa com 100 passos
=======
    stepsSinceApple = 0; // Inicializa o contador
>>>>>>> origin/main
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            grid[i][j] = 0;
    snake.clear();
    headX = 5; headY = 5; dir = 0; snakeLen = 3; apples = 0; steps = 0; gameOver = false;
    snake.push_back({headX, headY});
    PlaceApple();
}

void SnakeGame::PlaceApple() {
    static std::mt19937 rng{std::random_device{}()};
    do {
        appleX = rng() % 10;
        appleY = rng() % 10;
    } while (std::find(snake.begin(), snake.end(), std::make_pair(appleX, appleY)) != snake.end());
}

void SnakeGame::Step(int action) {
    if (gameOver) return;
    // 0: frente, 1: esquerda, 2: direita
    dir = (dir + (action == 1 ? 3 : (action == 2 ? 1 : 0))) % 4;
    int dx[] = {0, 1, 0, -1}, dy[] = {-1, 0, 1, 0};
    int nx = headX + dx[dir], ny = headY + dy[dir];
    if (nx < 0 || nx >= 10 || ny < 0 || ny >= 10 || std::find(snake.begin(), snake.end(), std::make_pair(nx, ny)) != snake.end()) {
        gameOver = true; return;
    }
    snake.insert(snake.begin(), {nx, ny});
    headX = nx; headY = ny;
    if (headX == appleX && headY == appleY) {
        apples++; PlaceApple();
<<<<<<< HEAD
        stepsLeft += 1; // Ganha 1 passo ao comer maçã
    } else {
        snake.pop_back();
    }
    stepsLeft--; // Perde 1 passo a cada movimento
    if (stepsLeft <= 0) {
        gameOver = true;
        return;
=======
    } else {
        snake.pop_back();
    }
    snake.insert(snake.begin(), {nx, ny});
    headX = nx; headY = ny;
    if (headX == appleX && headY == appleY) {
        apples++; PlaceApple();
        stepsSinceApple = 0; // Reseta ao comer maçã
    } else {
        snake.pop_back();
        stepsSinceApple++; // Incrementa se não comeu
        if (stepsSinceApple >= 100) {
            gameOver = true; // Morre se passar de 50 sem comer
            return;
        }
>>>>>>> origin/main
    }
    steps++;
}

bool SnakeGame::IsGameOver() const { return gameOver; }
int SnakeGame::GetAppleCount() const { return apples; }
std::vector<float> SnakeGame::GetSensors() const {
    std::vector<float> sensors;
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    int relDir[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; ++i) {
        int d = (dir + relDir[i]) % 4;
        int x = headX, y = headY;
        int dist_wall = 0;
        float sensor_value = 0;

        bool found_apple = false, found_tail = false;
        while (x + dx[d] >= 0 && x + dx[d] < 10 && y + dy[d] >= 0 && y + dy[d] < 10) {
            x += dx[d];
            y += dy[d];
            dist_wall++;
            if (!found_apple && x == appleX && y == appleY) {
                sensor_value = 5.0f;
                found_apple = true;
            }
            if (!found_tail && std::find(snake.begin() + 1, snake.end(), std::make_pair(x, y)) != snake.end()) {
                // Só marca se não achou maçã antes
                if (!found_apple) sensor_value = 2.0f;
                found_tail = true;
            }
        }
        if (!found_apple && !found_tail)
            sensor_value = dist_wall / 9.0f; // Normaliza para 0~1

        sensors.push_back(sensor_value);
    }
    return sensors;
}

void SnakeGame::Draw(int x, int y, int w, int h) const {
    int cell = std::min(w, h) / 10;
    int offsetX = x + (w - cell * 10) / 2;
    int offsetY = y + (h - cell * 10) / 2;
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            DrawRectangleLines(offsetX + i * cell, offsetY + j * cell, cell, cell, DARKGRAY);
    for (auto& s : snake)
        DrawRectangle(offsetX + s.first * cell, offsetY + s.second * cell, cell, cell, GREEN);
    DrawRectangle(offsetX + appleX * cell, offsetY + appleY * cell, cell, cell, RED);
}