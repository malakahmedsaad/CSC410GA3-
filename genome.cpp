#include "genome.h"
#include <iostream>
#include <cstdlib>   // for rand()
#include <cmath>
using namespace std;

// Constructor
genome::genome() {
    genes = NULL;
    nGenes = 0;
    mRate = 0.0;
}

// Destructor
genome::~genome() {
    deallocate();
}

// Allocate memory
void genome::allocate(int numGenes) {
    // If already allocated, free first
    if (genes != NULL) {
        deallocate();
    }

    if (numGenes <= 0) {
        return;
    }

    genes = new Pixel[numGenes];
    nGenes = numGenes;

    // Initialize all pixels to 0
    for (int i = 0; i < nGenes; i++) {
        genes[i].red = 0;
        genes[i].green = 0;
        genes[i].blue = 0;
    }
}

// Deallocate memory
void genome::deallocate() {
    if (genes == NULL) {
        return;
    }

    delete[] genes;
    genes = NULL;
    nGenes = 0;
}

// Randomize RGB values (0–255)
void genome::randomize() {
    for (int i = 0; i < nGenes; i++) {
        genes[i].red = rand() % 256;
        genes[i].green = rand() % 256;
        genes[i].blue = rand() % 256;
    }
}

// Setters with bounds checking
void genome::set_red(int index, int value) {
    if (index >= 0 && index < nGenes && value >= 0 && value <= 255) {
        genes[index].red = value;
    }
}

void genome::set_green(int index, int value) {
    if (index >= 0 && index < nGenes && value >= 0 && value <= 255) {
        genes[index].green = value;
    }
}

void genome::set_blue(int index, int value) {
    if (index >= 0 && index < nGenes && value >= 0 && value <= 255) {
        genes[index].blue = value;
    }
}

// Getters with bounds checking
int genome::get_red(int index) {
    if (index >= 0 && index < nGenes) {
        return genes[index].red;
    }
    return -1;
}

int genome::get_green(int index) {
    if (index >= 0 && index < nGenes) {
        return genes[index].green;
    }
    return -1;
}

int genome::get_blue(int index) {
    if (index >= 0 && index < nGenes) {
        return genes[index].blue;
    }
    return -1;
}

void genome::set_mRate(double val) {
    if (val >= 0.0 && val <= 1.0) {
        mRate = val;
    }
}

double genome::get_mRate() {
    return mRate;
}

// Print genome
void genome::print() {
    cout << "Genome contents (" << nGenes << " pixels):" << endl;

    for (int i = 0; i < nGenes; i++) {
        cout << "Pixel " << i << ": "
             << "R=" << genes[i].red << " "
             << "G=" << genes[i].green << " "
             << "B=" << genes[i].blue << endl;
    }
}

void genome::mutate_gene(int index) {
    if (index < 0 || index >= nGenes) return;

    double r;

    // Red
    r = (double) rand() / RAND_MAX;
    if (r < mRate) {
        genes[index].red = rand() % 255;
    }

    // Green
    r = (double) rand() / RAND_MAX;
    if (r < mRate) {
        genes[index].green = rand() % 255;
    }

    // Blue
    r = (double) rand() / RAND_MAX;
    if (r < mRate) {
        genes[index].blue = rand() % 255;
    }
}
void genome::mutate() {
    for (int i = 0; i < nGenes; i++) {
        mutate_gene(i);
    }
}
double genome::calculate_gene_fitness(int index, Pixel targetPixel) {
    if (index < 0 || index >= nGenes) return 0.0;

    double diffRed = fabs(genes[index].red - targetPixel.red);
    double diffGreen = fabs(genes[index].green - targetPixel.green);
    double diffBlue = fabs(genes[index].blue - targetPixel.blue);

    double avgDiff = (diffRed + diffGreen + diffBlue) / 3.0;

    return avgDiff / 255.0;
}
double genome::calculate_overall_fitness(Pixel* target, int nPixels) {
    if (nPixels != nGenes || target == NULL) return 0.0;
    if (nGenes == 0) return 0.0;

    double totalError = 0.0;

    for (int i = 0; i < nGenes; i++) {
        totalError += calculate_gene_fitness(i, target[i]);
    }

    return totalError / nGenes;
}
void genome::set_pixel(int index, Pixel newPixel) {
    if (index < 0 || index >= nGenes) return;

    if (newPixel.red < 0 || newPixel.red > 255) return;
    if (newPixel.green < 0 || newPixel.green > 255) return;
    if (newPixel.blue < 0 || newPixel.blue > 255) return;

    genes[index] = newPixel;
}
Pixel genome::get_pixel(int index) {
    Pixel p = {0, 0, 0};

    if (index >= 0 && index < nGenes) {
        return genes[index];
    }

    return p;
}