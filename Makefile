program: genome.o population.o main.o
	g++ genome.o population.o main.o -o program

genome.o: genome.cpp genome.h
	g++ -c genome.cpp

population.o: population.cpp population.h genome.h
	g++ -c population.cpp

main.o: main.cpp population.h
	g++ -c main.cpp

clean:
	rm -f *.o program