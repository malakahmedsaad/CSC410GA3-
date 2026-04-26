#ifndef POPULATION_H
#define POPULATION_H

#include "genome.h"
#include <iostream>

class population {
private:
    genome* individuals;
    int nIndividuals;

    int parentIndex1;
    int parentIndex2;
    bool parentsSelected;

    int nCrossover;
    double mutationRate;

    Pixel* targetGenome;
    int targetSize;

public:
    // Initializes an empty population.
    population();

    // Frees dynamically allocated population memory.
    ~population();

    // Creates popSize genomes, each with nGenes pixels.
    void generate_population(int popSize, int nGenes);

    // Stores the target image used for fitness calculation.
    void set_target(Pixel* target, int imageSize);

    // Selects the best two individuals based on fitness/error.
    void select_parents();

    // Sets and gets the number of crossover points.
    void set_nCrossover(int n);
    int get_nCrossover();

    // Sets mutation rate for all individuals.
    void set_mutation(double mRate);

    // Creates a new generation using crossover and mutation.
    void generate_new_population(int useRoulette);

    // Returns the average fitness/error of the population.
    double calculate_overall_fitness();

    // Prints selected parents or a message if none are selected.
    void print_parents();

    // Prints the full population.
    void print_population();

    // Extra credit: selects parents using roulette wheel logic.
    void find_roulette_top_individuals();
};

#endif