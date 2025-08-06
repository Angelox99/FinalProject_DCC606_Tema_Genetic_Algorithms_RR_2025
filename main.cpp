// main.cpp
#include "NeuralNet.h"
#include "raylib.h"
#include "SnakeGame.h"
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include "Genetic.h"

// --- Configurações do Algoritmo Genético ---
const int DNA_SIZE = 42;
const int POPULATION_SIZE = 1000;
const int GENERATIONS = 1000;
const double BEST_PROP = 0.10;
const double MUTATION_PROB = 0.005;
const int LOG_INTERVAL = 10;

// --- DNA manual: se vazio, usa GA; se preenchido, exibe direto este agente ---
const std::vector<int> DNA_OVERRIDE = {
    //491, -292, -358, -462, -41, 674, 608, -2, 564, 92, -171, 943, 991, 740, 491, -478, 303, -855, 17, -186, -797, -780, 887, 106, 554, -67, -585, -262, -113, 222, 935, 188, 872, 305, 328, 606, 379, -16, 41, -299, -599, 532
    // estrategia circular -602, -562, -341, 381, 488, 455, 256, -338, 978, -474, -583, 3, 852, -633, -50, 605, -998, 645, -999, 384, 777, -683, -272, -300, 396, 252, 591, -34, 230, 585, -680, 168, 904, 376, -402, 276, 998, -841, -638, 183, -684, -747
    // 20 macas 915, -340, 207, 635, 838, -124, -718, 53, -852, 233, 499, -483, -946, -517, -377, 502, -469, -371, -552, -226, -781, 41, -746, 608, -544, -46, -559, -288, 895, -10, 845, 425, -503, -464, 477, -825, 836, -936, 115, -13, 773, 265
};

// --- função fitines ---
double FitnessFunction(const std::vector<int>& chromosome) {
    std::vector<float> dna;
    for (int gene : chromosome)
        dna.push_back(gene / 1000.0f);
    SnakeGame env;
    NeuralNet net(dna);

    int steps_taken = 0;
    while (!env.IsGameOver()) {
        auto sensors = env.GetSensors();
        int action = net.Decide(sensors);
        env.Step(action);
        steps_taken++;
    }
    // Foco principal: número de maçãs coletadas
    double fitness = env.GetAppleCount() * 100.0 - steps_taken;
    return fitness;
}

// --- Função para desenhar o DNA ---
void DrawDNA(const std::vector<int>& chromosome, int x, int y, Color color) {
    DrawText("DNA:", x, y, 18, color);
    std::string dnaStr;
    for (size_t i = 0; i < chromosome.size(); ++i) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.2f ", chromosome[i] / 1000.0f);
        dnaStr += buf;
        if ((i + 1) % 7 == 0) dnaStr += "\n";
    }
    DrawText(dnaStr.c_str(), x, y + 24, 16, color);
}

// --- Função para atualizar fitness de toda a população ---
void UpdateFitness(Genetic& ga) {
    ga.current_fitness.resize(ga.current_population.size());
    #pragma omp parallel for
    for (long i = 0; i < (long)ga.current_population.size(); ++i)
        ga.current_fitness[i] = ga.fitness_function(ga.current_population[i]);
}

// --- Função para encontrar o melhor e pior indivíduo ---
void FindBestWorst(const std::vector<double>& fitness, int& best_idx, int& worst_idx) {
    best_idx = 0;
    worst_idx = 0;
    for (size_t i = 1; i < fitness.size(); ++i) {
        if (fitness[i] > fitness[best_idx]) best_idx = i;
        if (fitness[i] < fitness[worst_idx]) worst_idx = i;
    }
}

// --- Visualização da rede neural com layout mais bonito ---
void DrawNeuralNet(const NeuralNet& net, int cx, int y, int layerGap, int nodeGap, int nodeR,
                   const char* inputLabels[4], const char* outputLabels[3]) {
    auto inputs = net.GetLastInputs();
    auto hidden = net.GetLastHidden();
    auto outputs = net.GetLastOutputs();

    // Conexões: entradas -> oculto
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 6; ++j)
            DrawLineBezier(
                (Vector2){(float)(cx - 2 * layerGap + nodeR), (float)(y + 60 + i * nodeGap)},
                (Vector2){(float)(cx - layerGap), (float)(y + 50 + (i + j) * nodeGap / 2)},
                2.0f, Fade(LIGHTGRAY, 0.5f)
            );
    // Conexões: oculto -> saída
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 3; ++j)
            DrawLineBezier(
                (Vector2){(float)(cx + 0 * layerGap + nodeR), (float)(y + 40 + i * nodeGap)},
                (Vector2){(float)(cx + layerGap), (float)(y + 50 + (i + j) * nodeGap / 2)},
                2.0f, Fade(LIGHTGRAY, 0.5f)
            );

    // Entradas
    for (int i = 0; i < 4; ++i) {
        DrawCircle(cx - 2 * layerGap, y + 60 + i * nodeGap, nodeR, SKYBLUE);
        DrawText(TextFormat("%.2f", i < inputs.size() ? inputs[i] : 0.0f),
                 cx - 2 * layerGap - 36, y + 50 + i * nodeGap, 14, DARKBLUE);
        DrawText(inputLabels[i], cx - 2 * layerGap + nodeR + 8, y + 50 + i * nodeGap, 14, SKYBLUE);
    }
    // Camada oculta
    for (int i = 0; i < 6; ++i) {
        DrawCircle(cx, y + 40 + i * nodeGap, nodeR, ORANGE);
        DrawCircleLines(cx, y + 40 + i * nodeGap, nodeR + 3, Fade(ORANGE, 0.3f));
        DrawText(TextFormat("%.2f", i < hidden.size() ? hidden[i] : 0.0f),
                 cx - 18, y + 30 + i * nodeGap, 14, DARKBROWN);
    }
    // Saídas
    for (int i = 0; i < 3; ++i) {
        DrawCircle(cx + 2 * layerGap, y + 60 + i * nodeGap, nodeR, PINK);
        DrawCircleLines(cx + 2 * layerGap, y + 60 + i * nodeGap, nodeR + 3, Fade(PINK, 0.3f));
        DrawText(TextFormat("%.2f", i < outputs.size() ? outputs[i] : 0.0f),
                 cx + 2 * layerGap + nodeR + 8, y + 50 + i * nodeGap, 14, MAROON);
        DrawText(outputLabels[i], cx + 2 * layerGap - nodeR - 60, y + 50 + i * nodeGap, 14, PINK);
    }
    // Destaca decisão
    int dec = net.GetLastDecision();
    DrawCircleLines(cx + 2 * layerGap, y + 60 + dec * nodeGap, nodeR + 7, GREEN);
}

int main() {
    std::vector<int> best_chromosome;

    // --- Se DNA_OVERRIDE está vazio, roda o GA normalmente ---
    if (DNA_OVERRIDE.empty()) {
        Genetic ga(
            DNA_SIZE,
            FitnessFunction,
            false,
            POPULATION_SIZE,
            GENERATIONS,
            BEST_PROP,
            MUTATION_PROB
        );

        ga.current_generation = 0;
        ga.current_population = ga._generate_population();
        UpdateFitness(ga);

        int best_idx = 0;
        for (; ga.current_generation < GENERATIONS; ) {
            if (ga.current_population.empty()) break;
            if (ga.current_generation % LOG_INTERVAL == 0) {
                int best, worst;
                FindBestWorst(ga.current_fitness, best, worst);
                std::cout << "Geração " << ga.current_generation
                          << " / " << GENERATIONS
                          << " | Melhor pontuação: " << ga.current_fitness[best]
                          << std::endl;
            }
            ga.StepGeneration();
            if (ga.current_population.empty()) break;
            UpdateFitness(ga);
        }
        FindBestWorst(ga.current_fitness, best_idx, best_idx);
        best_chromosome = ga.current_population[best_idx];

        // --- Imprime DNA do melhor agente ao final ---
        std::cout << "\nDNA do melhor agente:\n";
        for (size_t i = 0; i < best_chromosome.size(); ++i) {
            std::cout << best_chromosome[i];
            if (i + 1 < best_chromosome.size()) std::cout << ", ";
        }
        std::cout << std::endl;
    } else {
        // --- Se DNA_OVERRIDE não está vazio, exibe direto este agente ---
        best_chromosome = DNA_OVERRIDE;
        std::cout << "\nExibindo agente do DNA fornecido:\n";
        for (size_t i = 0; i < best_chromosome.size(); ++i) {
            std::cout << best_chromosome[i];
            if (i + 1 < best_chromosome.size()) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // --- Visualização gráfica do melhor agente ---
    std::vector<float> best_dna;
    for (int gene : best_chromosome) best_dna.push_back(gene / 1000.0f);

    SnakeGame best_env;
    NeuralNet best_net(best_dna);
    int best_apples = 0;
    int best_steps = 0;

    // --- Parâmetros de layout ---
    const int screenW = 1200, screenH = 820, sidePanelW = 320;
    const int margin = 32;
    const int colW = screenW - sidePanelW - 2 * margin;
    const int leftX = margin;
    const int gridSize = 400;
    const int gridY = 40;
    const int gridX = leftX + (colW - gridSize) / 2;

    // --- Legendas das entradas e saídas ---
    const char* inputLabels[4] = {"Frente", "Direita", "Tras", "Esquerda"};
    const char* inputDesc[4] = {"Sensor frente", "Sensor direita", "Sensor tras", "Sensor esquerda"};
    const char* outputLabels[3] = {"Frente", "Esquerda", "Direita"};
    const char* outputDesc[3] = {"Ir em frente", "Virar esquerda", "Virar direita"};

    InitWindow(screenW, screenH, "Melhor Agente - Snake Genético");
    SetTargetFPS(60);

    best_env.Reset();
    while (!WindowShouldClose()) {
        if (best_env.IsGameOver()) {
            best_env.Reset();
            best_apples = 0;
            best_steps = 0;
        }

        auto sensors = best_env.GetSensors();
        int action = best_net.Decide(sensors);
        best_env.Step(action);
        best_apples = best_env.GetAppleCount();
        best_steps++;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- Painel lateral direito com informações do GA ---
        DrawRectangle(screenW - sidePanelW, 0, sidePanelW, screenH, Fade(LIGHTGRAY, 0.6f));
        int infoX = screenW - sidePanelW + 24;
        int infoY = 40;
        DrawText("Algoritmo Genético", infoX, infoY, 24, DARKBLUE);
        infoY += 40;
        DrawText(TextFormat("População: %d", POPULATION_SIZE), infoX, infoY, 20, DARKGRAY); infoY += 32;
        DrawText(TextFormat("Gerações: %d", GENERATIONS), infoX, infoY, 20, DARKGRAY); infoY += 32;
        DrawText(TextFormat("Melhor pontuação: %.2f", FitnessFunction(best_chromosome)), infoX, infoY, 20, DARKGREEN); infoY += 32;
        DrawText(TextFormat("Maçãs do melhor: %d", best_apples), infoX, infoY, 20, DARKGREEN); infoY += 32;
        DrawText(TextFormat("Passos: %d", best_steps), infoX, infoY, 20, DARKGRAY);

        // --- Painel do agente centralizado ---
        DrawText("Melhor Agente", gridX + (gridSize - MeasureText("Melhor Agente", 28)) / 2, gridY - 36, 28, DARKGREEN);

        // --- Grade do campo ---
        int cell = gridSize / 10;
        for (int i = 0; i <= 10; ++i) {
            DrawLine(gridX + i * cell, gridY, gridX + i * cell, gridY + gridSize, Fade(GRAY, 0.5f));
            DrawLine(gridX, gridY + i * cell, gridX + gridSize, gridY + i * cell, Fade(GRAY, 0.5f));
        }
        best_env.Draw(gridX, gridY, gridSize, gridSize);

        // --- DNA ---
        DrawDNA(best_chromosome, leftX, screenH - 180, DARKGREEN);

        // --- Painel da rede neural ---
        int nnPanelY = gridY + gridSize + 40;
        DrawText("Rede Neural em Execução", leftX + (colW - 300) / 2, nnPanelY, 22, DARKBLUE);

        // --- Visualização da rede neural ---
        int cx = leftX + colW / 2;
        int nodeR = 18;
        int layerGap = 110;
        int nodeGap = 48;
        DrawNeuralNet(best_net, cx, nnPanelY, layerGap, nodeGap, nodeR, inputLabels, outputLabels);

        // --- Legenda das entradas ---
        int legendX = leftX + 40;
        int legendY = nnPanelY + 6 * nodeGap + 60;
        DrawRectangle(legendX - 10, legendY - 10, 340, 90, Fade(BLACK, 0.7f));
        DrawText("Legenda Entradas:", legendX, legendY, 16, SKYBLUE);
        for (int i = 0; i < 4; ++i)
            DrawText(TextFormat("%s: %s", inputLabels[i], inputDesc[i]), legendX + 8, legendY + 20 + i * 16, 14, LIGHTGRAY);

        // --- Legenda das saídas ---
        int legend2X = leftX + colW - 340;
        DrawRectangle(legend2X - 10, legendY - 10, 340, 70, Fade(BLACK, 0.7f));
        DrawText("Legenda Decisões:", legend2X, legendY, 16, PINK);
        for (int i = 0; i < 3; ++i)
            DrawText(TextFormat("%s: %s", outputLabels[i], outputDesc[i]), legend2X + 8, legendY + 20 + i * 16, 14, LIGHTGRAY);

        EndDrawing();
    }
    CloseWindow();
}