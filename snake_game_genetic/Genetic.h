// Genetic.h
#pragma once
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <iostream>

class Genetic {
public:
    using Chromosome = std::vector<int>;
    using Population = std::vector<Chromosome>;
    using FitnessFunction = std::function<double(const Chromosome&)>;

    Population current_population;
    std::vector<double> current_fitness;
    int current_generation = 0;

    Genetic(int chromosome_size,
            FitnessFunction fitness_function,
            bool fitness_minimize = false,
            int population_size = 10,
            int generations = 10,
            double best = 0.5,
            double mutation_prob = 0.001)
        : chromosome_size(chromosome_size),
          population_size(population_size),
          generations(generations),
          fitness_function(fitness_function),
          fitness_minimize(fitness_minimize),
          best(best),
          mutation_prob(mutation_prob)
    {}

    Population _generate_population() {
        Population population;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-1000, 1000);
        for (int i = 0; i < population_size; ++i) {
            Chromosome chromosome;
            for (int j = 0; j < chromosome_size; ++j)
                chromosome.push_back(dis(gen));
            population.push_back(chromosome);
        }
        return population;
    }

    // Seleciona os melhores indivíduos
    Population _selection(const Population& population, const std::vector<double>& fitness, double best) {
        std::vector<size_t> idx(population.size());
        for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
        if (fitness_minimize)
            std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return fitness[a] < fitness[b]; });
        else
            std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return fitness[a] > fitness[b]; });
        int n = std::max(1, int(population.size() * best));
        Population selected;
        for (int i = 0; i < n; ++i)
            selected.push_back(population[idx[i]]);
        return selected;
    }

    // Cruzamento simples de 1 ponto
    Chromosome crossover(const Chromosome& p1, const Chromosome& p2) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, chromosome_size - 1);
        int point = dis(gen);
        Chromosome child;
        child.insert(child.end(), p1.begin(), p1.begin() + point);
        child.insert(child.end(), p2.begin() + point, p2.end());
        return child;
    }

    // Mutação simples
    void mutate(Chromosome& chromosome) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> prob(0.0, 1.0);
        std::uniform_int_distribution<> dis(-1000, 1000);
        for (auto& gene : chromosome)
            if (prob(gen) < mutation_prob)
                gene = dis(gen);
    }

    // Avança uma geração
    bool StepGeneration() {
        if (current_population.empty()) return false;
        // Avalia fitness
        current_fitness.resize(current_population.size());
        for (size_t i = 0; i < current_population.size(); ++i)
            current_fitness[i] = fitness_function(current_population[i]);
        if (current_generation >= generations) return false;

        // Seleciona elite
        Population elite = _selection(current_population, current_fitness, best);

        // Gera nova população
        Population new_population = elite;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, elite.size() - 1);

        while ((int)new_population.size() < population_size) {
            const Chromosome& p1 = elite[dis(gen)];
            const Chromosome& p2 = elite[dis(gen)];
            Chromosome child = crossover(p1, p2);
            mutate(child);
            new_population.push_back(child);
        }
        // Garante tamanho exato
        if ((int)new_population.size() > population_size)
            new_population.resize(population_size);

        current_population = new_population;
        current_generation++;
        return true;
    }

    // Dados públicos
    int chromosome_size, population_size, generations;
    FitnessFunction fitness_function;
    bool fitness_minimize;
    double best, mutation_prob;
};