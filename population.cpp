#include "population.h"
#include <cstdlib>
#include <algorithm>
using namespace std;

population::population() {
    individuals = NULL;
    nIndividuals = 0;

    parentIndex1 = -1;
    parentIndex2 = -1;
    parentsSelected = false;

    nCrossover = 1;
    mutationRate = 0.0;

    targetGenome = NULL;
    targetSize = 0;
}

population::~population() {
    if (individuals != NULL) {
        delete[] individuals;
        individuals = NULL;
    }
    nIndividuals = 0;
}

void population::generate_new_population(int useRoulette) {
    if (nIndividuals == 0 || targetGenome == NULL) return;

    if (useRoulette == 1) {
        find_roulette_top_individuals();
    } else {
        select_parents();
    }

    genome* newPop = new genome[nIndividuals];

    genome& p1 = individuals[parentIndex1];
    genome& p2 = individuals[parentIndex2];

    int nGenes = targetSize;

    for (int i = 0; i < nIndividuals; i++) {
        newPop[i].allocate(nGenes);

        bool useFirst = true;

        for (int j = 0; j < nGenes; j++) {
            int segmentSize = nGenes / (nCrossover + 1);
            if (segmentSize <= 0) segmentSize = 1;

            if (j > 0 && j % segmentSize == 0) {
                useFirst = !useFirst;
            }

            Pixel pixel;

            if (useFirst) {
                pixel = p1.get_pixel(j);
            } else {
                pixel = p2.get_pixel(j);
            }

            newPop[i].set_pixel(j, pixel);
        }

        newPop[i].set_mRate(mutationRate);
        newPop[i].mutate();
    }

    delete[] individuals;
    individuals = newPop;
    parentsSelected = false;
}

void population::set_target(Pixel* target, int imageSize) {
    targetGenome = target;
    targetSize = imageSize;
}

void population::select_parents() {
    if (targetGenome == NULL || nIndividuals < 2) return;

    parentIndex1 = 0;
    parentIndex2 = 1;

    double best1 = individuals[0].calculate_overall_fitness(targetGenome, targetSize);
    double best2 = individuals[1].calculate_overall_fitness(targetGenome, targetSize);

    if (best2 < best1) {
        swap(parentIndex1, parentIndex2);
        swap(best1, best2);
    }

    for (int i = 2; i < nIndividuals; i++) {
        double fit = individuals[i].calculate_overall_fitness(targetGenome, targetSize);

        if (fit < best1) {
            best2 = best1;
            parentIndex2 = parentIndex1;

            best1 = fit;
            parentIndex1 = i;
        } else if (fit < best2) {
            best2 = fit;
            parentIndex2 = i;
        }
    }

    parentsSelected = true;
}

void population::set_nCrossover(int n) {
    if (n > 0) nCrossover = n;
}

int population::get_nCrossover() {
    return nCrossover;
}

void population::set_mutation(double mRate) {
    mutationRate = mRate;

    for (int i = 0; i < nIndividuals; i++) {
        individuals[i].set_mRate(mRate);
    }
}
void population::generate_new_population(int useRoulette) {
    if (nIndividuals == 0) return;

    if (useRoulette == 1) {
        find_roulette_top_individuals();
    } else {
        if (!parentsSelected) return;
    }
    genome* newPop = new genome[nIndividuals];

    genome& p1 = individuals[parentIndex1];
    genome& p2 = individuals[parentIndex2];

    int nGenes = targetSize;

    for (int i = 0; i < nIndividuals; i++) {
        newPop[i].allocate(nGenes);

        int segmentLength = nGenes / nCrossover;
        bool useFirst = true;
        int currentIndex = 0;

        for (int c = 0; c < nCrossover; c++) {
            int end = (c == nCrossover - 1) ? nGenes : currentIndex + segmentLength;

            for (int j = currentIndex; j < end; j++) {
                Pixel pixel = useFirst ? p1.get_pixel(j) : p2.get_pixel(j);
                newPop[i].set_pixel(j, pixel);
            }

            useFirst = !useFirst;
            currentIndex = end;
        }

        newPop[i].set_mRate(mutationRate);
        newPop[i].mutate();
    }

    delete[] individuals;
    individuals = newPop;
}

double population::calculate_overall_fitness() {
    if (targetGenome == NULL || nIndividuals == 0) return 0.0;

    double total = 0.0;

    for (int i = 0; i < nIndividuals; i++) {
        total += individuals[i].calculate_overall_fitness(targetGenome, targetSize);
    }

    return total / nIndividuals;
}

void population::print_parents() {
    if (!parentsSelected) {
        cout << "no parents yet set." << endl;
        return;
    }

    cout << "Parent 1:\n";
    individuals[parentIndex1].print();

    cout << "Parent 2:\n";
    individuals[parentIndex2].print();
}

void population::print_population() {
    cout << "Population size: " << nIndividuals << endl;
    cout << "Crossover points: " << nCrossover << endl;
    cout << "Mutation rate: " << mutationRate << endl;

    for (int i = 0; i < nIndividuals; i++) {
        cout << "Individual " << i << ":\n";
        individuals[i].print();
    }
}
void population::find_roulette_top_individuals() {
    if (targetGenome == NULL || nIndividuals < 2) return;

    double* weights = new double[nIndividuals];
    double totalWeight = 0.0;

    // Convert error → weight (higher weight = better)
    for (int i = 0; i < nIndividuals; i++) {
        double fitness = individuals[i].calculate_overall_fitness(targetGenome, targetSize);

        // Avoid division by zero
        weights[i] = 1.0 / (fitness + 1e-6);
        totalWeight += weights[i];
    }

    // Helper to pick one index
    auto pick_one = [&](void) {
        double r = ((double) rand() / RAND_MAX) * totalWeight;

        double cumulative = 0.0;
        for (int i = 0; i < nIndividuals; i++) {
            cumulative += weights[i];
            if (r <= cumulative) return i;
        }
        return nIndividuals - 1;
    };

    parentIndex1 = pick_one();

    // Ensure second parent is different
    do {
        parentIndex2 = pick_one();
    } while (parentIndex2 == parentIndex1);

    parentsSelected = true;

    delete[] weights;
}