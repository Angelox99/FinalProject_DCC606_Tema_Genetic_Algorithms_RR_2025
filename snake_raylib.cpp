// snake_raylib.cpp
// Jogo da cobrinha com raylib + C++ orientado a objetos
// Requisitos: raylib instalado corretamente no sistema

#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int CELL_SIZE = 20;
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 20;
const int SCREEN_WIDTH = CELL_SIZE * GRID_WIDTH;
const int SCREEN_HEIGHT = CELL_SIZE * GRID_HEIGHT;

struct Vector2Int {
    int x, y;
    bool operator==(const Vector2Int &other) const { return x == other.x && y == other.y; }
};

class SnakeGame {
private:
    std::vector<Vector2Int> snake;
    Vector2Int direction;
    Vector2Int apple;
    float moveDelay = 0.2f;
    float moveTimer = 0.0f;
    bool gameOver = false;

public:
    SnakeGame() {
        Reset();
    }

    void Reset() {
        snake.clear();
        snake.push_back({ GRID_WIDTH / 2, GRID_HEIGHT / 2 });
        direction = { 1, 0 };
        SpawnApple();
        gameOver = false;
        moveTimer = 0.0f;
    }

    void SpawnApple() {
        while (true) {
            apple.x = GetRandomValue(0, GRID_WIDTH - 1);
            apple.y = GetRandomValue(0, GRID_HEIGHT - 1);
            bool onSnake = false;
            for (auto &s : snake) {
                if (s == apple) {
                    onSnake = true;
                    break;
                }
            }
            if (!onSnake) break;
        }
    }

    void HandleInput() {
        if (IsKeyPressed(KEY_UP) && direction.y != 1) direction = {0, -1};
        if (IsKeyPressed(KEY_DOWN) && direction.y != -1) direction = {0, 1};
        if (IsKeyPressed(KEY_LEFT) && direction.x != 1) direction = {-1, 0};
        if (IsKeyPressed(KEY_RIGHT) && direction.x != -1) direction = {1, 0};
    }

    void MoveSnake() {
        Vector2Int newHead = { snake[0].x + direction.x, snake[0].y + direction.y };

        if (newHead.x < 0 || newHead.x >= GRID_WIDTH || newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
            gameOver = true;
            return;
        }

        for (size_t i = 0; i < snake.size(); i++) {
            if (newHead == snake[i]) {
                gameOver = true;
                return;
            }
        }

        snake.insert(snake.begin(), newHead);

        if (newHead == apple) {
            if (snake.size() == (size_t)(GRID_WIDTH * GRID_HEIGHT)) {
                gameOver = true;
            } else {
                SpawnApple();
            }
        } else {
            snake.pop_back();
        }
    }

    void Update() {
        if (IsKeyPressed(KEY_R) && gameOver) Reset();
        if (gameOver) return;

        HandleInput();

        moveTimer += GetFrameTime();
        if (moveTimer >= moveDelay) {
            moveTimer = 0.0f;
            MoveSnake();
        }
    }

    void DrawCell(Vector2Int cell, Color color) {
        DrawRectangle(cell.x * CELL_SIZE, cell.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
    }

    void Draw() {
        ClearBackground(BLACK);

        // Arena
        DrawRectangleLines(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DARKGRAY);

        // Maçã
        DrawCell(apple, RED);

        // Cobra
        for (size_t i = 0; i < snake.size(); i++) {
            DrawCell(snake[i], i == 0 ? GREEN : DARKGREEN);
        }

        // Fim de jogo
        if (gameOver) {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, WHITE);
            DrawText("Pressione R para reiniciar", SCREEN_WIDTH / 2 - MeasureText("Pressione R para reiniciar", 20) / 2, SCREEN_HEIGHT / 2, 20, GRAY);
        }
    }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game - Raylib + C++");
    SetTargetFPS(60);
    InitAudioDevice();
    srand(time(nullptr));

    SnakeGame game;

    while (!WindowShouldClose()) {
        game.Update();

        BeginDrawing();
        game.Draw();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
