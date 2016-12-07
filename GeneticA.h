#ifndef _GENETICA_H
#define _GENETICA_H

#include <vector>
#include <sstream>

struct Genome
{
	std::vector<int> bits;
	double fitness;

	Genome():fitness(0){}

	Genome(const int numBits):fitness(0)
	{
		//create random bit string
		for(int i = 0; i<numBits; i++)
		{
			bits.push_back(rand()%2);
		}
	}
};

class ROBOT;

class GeneticA
{
private:
	//genome population
	std::vector<Genome> genomes;

	int popSize;
	double crossoverRate;
	double mutationRate;
	int chromoLength;//bits per chromosome
	int geneLength;//bits per gene
	int fittestGenome;
	double bestFitnessScore;
	double totalFitnessScore;
	int generation;
	bool busy;

	void Mutate(std::vector<int> &bits);
	void Crossover(const std::vector<int> &mum,
					const std::vector<int> &dad,
					std::vector<int> &baby1,
					std::vector<int> &baby2);
	Genome& RouletteWheelSelection();
	std::vector<int> Decode(const std::vector<int> &bits);
	int BinToInt(const std::vector<int> &v);
	void CreateStartPopulation();

public:
	GeneticA(double cross_rat,
				double mut_rat,
				int pop_size,
				int num_bits,
				int gene_len):crossoverRate(cross_rat),
								mutationRate(mut_rat),
								popSize(pop_size),
								chromoLength(num_bits),
								totalFitnessScore(0.0),
								generation(0),
								geneLength(gene_len),
								busy(false)
	{
		CreateStartPopulation();
	}

	void Run();
	void Epoch(ROBOT* robot);//update function

	int Generation(){return generation;}
	int GetFittest(){return fittestGenome;}
	bool Started(){return busy;}
	void Stop(){busy=false;}
};

#endif