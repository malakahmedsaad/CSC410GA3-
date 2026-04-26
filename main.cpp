#include "population.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void test_genome_functions() {
    cout << "\n===== Testing genome functions =====\n";

    genome g;
    g.allocate(3);

    // Test valid setters and getters.
    g.set_red(0, 100);
    g.set_green(0, 150);
    g.set_blue(0, 200);

    cout << "Testing valid setters/getters:\n";
    cout << "Expected R=100, got R=" << g.get_red(0) << endl;
    cout << "Expected G=150, got G=" << g.get_green(0) << endl;
    cout << "Expected B=200, got B=" << g.get_blue(0) << endl;

    // Test invalid index and invalid color value.
    g.set_red(-1, 50);
    g.set_green(10, 50);
    g.set_blue(0, 300);

    cout << "\nTesting invalid cases:\n";
    cout << "Invalid red index should return -1: " << g.get_red(-1) << endl;
    cout << "Invalid green index should return -1: " << g.get_green(10) << endl;
    cout << "Blue should still be 200 after invalid value 300: " << g.get_blue(0) << endl;

    // Test individual gene fitness.
    Pixel targetPixel;
    targetPixel.red = 100;
    targetPixel.green = 150;
    targetPixel.blue = 200;

    cout << "\nTesting calculate_gene_fitness():\n";
    cout << "Expected perfect match fitness/error near 0: "
         << g.calculate_gene_fitness(0, targetPixel) << endl;

    // Test mutation for one gene.
    g.set_mRate(1.0);
    cout << "\nBefore mutate_gene(0):\n";
    g.print();

    g.mutate_gene(0);

    cout << "\nAfter mutate_gene(0) with mutation rate 1.0:\n";
    g.print();

    g.deallocate();
}

void test_population_functions() {
    cout << "\n===== Testing population functions =====\n";

    int popSize = 6;
    int nGenes = 10;

    // Create target image.
    Pixel* target = new Pixel[nGenes];
    for (int i = 0; i < nGenes; i++) {
        target[i].red = rand() % 256;
        target[i].green = rand() % 256;
        target[i].blue = rand() % 256;
    }

    population pop;

    // Test print_parents before selecting parents.
    cout << "\nTesting print_parents() before select_parents():\n";
    pop.print_parents();

    // Set target and generate initial population.
    pop.set_target(target, nGenes);
    pop.generate_population(popSize, nGenes);

    // Test crossover setter/getter.
    pop.set_nCrossover(2);
    cout << "\nTesting set_nCrossover/get_nCrossover:\n";
    cout << "Expected crossover points = 2, got "
         << pop.get_nCrossover() << endl;

    // Set mutation rate.
    pop.set_mutation(0.05);

    cout << "\nInitial population:\n";
    pop.print_population();

    cout << "\nInitial average fitness/error: "
         << pop.calculate_overall_fitness() << endl;

    // Test parent selection.
    cout << "\nTesting select_parents():\n";
    pop.select_parents();
    pop.print_parents();

    // Test new population generation with regular parent selection.
    cout << "\nTesting generate_new_population(0):\n";
    pop.generate_new_population(0);

    cout << "Average fitness/error after new generation: "
         << pop.calculate_overall_fitness() << endl;

    // Test roulette version.
    cout << "\nTesting generate_new_population(1) with roulette:\n";
    pop.generate_new_population(1);

    cout << "Average fitness/error after roulette generation: "
         << pop.calculate_overall_fitness() << endl;

    delete[] target;
}

int main() {
    // Seed random number generator.
    srand(time(0));

    // Run unit-style tests for genome and population.
    test_genome_functions();
    test_population_functions();

    return 0;
}