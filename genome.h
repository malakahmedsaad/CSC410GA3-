#ifndef GENOME_H
#define GENOME_H

// Represents one RGB pixel.
struct Pixel {
    int red;
    int green;
    int blue;
};

class genome {
public:
    // Initializes an empty genome.
    genome();

    // Frees dynamically allocated memory.
    ~genome();

    // Allocates memory for numGenes pixels.
    void allocate(int numGenes);

    // Frees the genome's dynamic array.
    void deallocate();

    // Randomly assigns RGB values from 0 to 255.
    void randomize();

    // Sets one color value at a valid index.
    void set_red(int index, int value);
    void set_green(int index, int value);
    void set_blue(int index, int value);

    // Returns one color value at a valid index, or -1 if invalid.
    int get_red(int index);
    int get_green(int index);
    int get_blue(int index);

    // Prints all genes in the genome.
    void print();

    // Sets and gets the mutation rate.
    void set_mRate(double val);
    double get_mRate();

    // Mutates one gene or the whole genome.
    void mutate_gene(int index);
    void mutate();

    // Calculates error/fitness compared to a target pixel or target image.
    double calculate_gene_fitness(int index, Pixel targetPixel);
    double calculate_overall_fitness(Pixel* target, int nPixels);

    // Sets or gets a full pixel.
    void set_pixel(int index, Pixel newPixel);
    Pixel get_pixel(int index);

private:
    Pixel* genes;
    int nGenes;
    double mRate;
};

#endif