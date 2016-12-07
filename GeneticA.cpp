#include "GeneticA.h"
#include "robot.h"

Genome& GeneticA::RouletteWheelSelection()
{
	double slice = ((rand())/(RAND_MAX+1.0)) * totalFitnessScore;
	double total = 0.0;
	int selectedGenome = 0;

	for(int i = 0; i < popSize; ++i)
	{
		total += genomes[i].fitness;

		if(total > slice)
		{
			selectedGenome = i;
			break;
		}
	}

	return genomes[selectedGenome];
}

void GeneticA::Mutate(std::vector<int> &bits)
{
	for(int curBit=0; curBit<bits.size(); curBit++)
	{
		//do we flip this bit?
		if(((rand())/(RAND_MAX+1.0)) < mutationRate)
		{
			//flip the bit
			bits[curBit] = !bits[curBit];
		}
	}//next bit
}

void GeneticA::Crossover( const std::vector<int> &mum,
						const std::vector<int> &dad,
						std::vector<int>		  &baby1,
						std::vector<int>		  &baby2)
{
	if ( (((rand())/(RAND_MAX+1.0)) > crossoverRate) || (mum == dad)) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}
	
	//determine a crossover point
	int cp = rand()%chromoLength;

	//swap the bits
	for (int i=0; i<cp; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (int i=cp; i<mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}
}

void GeneticA::Run()
{
	CreateStartPopulation();

	busy = true;
}

void GeneticA::CreateStartPopulation()
{
	//clear existing population
	genomes.clear();

	for(int i=0; i<popSize; i++)
	{
		genomes.push_back(Genome(chromoLength));
	}

	//reset variables
	generation = 0;
	fittestGenome = 0;
	bestFitnessScore = 0;
	totalFitnessScore = 0;
}

void GeneticA::Epoch(ROBOT* robot)
{
	
	//---------UPDATE FITNESS SCORES------------------------------------//
	fittestGenome		= 0;
	bestFitnessScore	= 0;
	totalFitnessScore	= 0;
	 
	//update the fitness scores and keep a check on fittest so far
	for (int i=0; i<popSize; ++i)
	{
		//decode each genomes chromosome into a vector of directions
		std::vector<int> directions = Decode(genomes[i].bits);

		//get it's fitness score
		genomes[i].fitness = robot->TestRoute(directions);

		if(robot->TestRoute(directions) == 1)
			MessageBoxA(NULL,"0","0",0);

		//update total
		totalFitnessScore += genomes[i].fitness;

		//if this is the fittest genome found so far, store results
		if (genomes[i].fitness > bestFitnessScore)
		{
			bestFitnessScore = genomes[i].fitness;

			fittestGenome = i;

			//Has Bob found the exit?
			if (genomes[i].fitness == 1)
			{
				//is so, stop the run
				busy = false;

				//send fittest genome to robot class
				robot->SetGeneticRoute(Decode(genomes[i].bits));


			}
		}
	
	}//next genome
	//--------END: UPDATE FITNESS SCORES--------------------------------//


	//Now to create a new population
	int newBabies = 0;

	//create some storage for the baby genomes 
	std::vector<Genome> babyGenomes;

	while (newBabies < popSize)
	{
		//select 2 parents
		Genome mum = RouletteWheelSelection();
		Genome dad = RouletteWheelSelection();

		//operator - crossover
		Genome baby1, baby2;
		Crossover(mum.bits, dad.bits, baby1.bits, baby2.bits);

		//operator - mutate
		Mutate(baby1.bits);
		Mutate(baby2.bits);

		//add to new population
		babyGenomes.push_back(baby1);
		babyGenomes.push_back(baby2);

		newBabies += 2;
	}

	//copy babies back into starter population
	genomes = babyGenomes;

	//increment the generation counter
	++generation;
}

std::vector<int> GeneticA::Decode(const std::vector<int> &bits)
{
	std::vector<int>	directions;

	//step through the chromosome a gene at a time
	for (int gene=0; gene < bits.size(); gene += geneLength)
	{
		//get the gene at this position
		std::vector<int> thisGene;
		
		for (int bit = 0; bit < geneLength; ++bit)
		{
			thisGene.push_back(bits[gene+bit]);
		}

		//convert to decimal and add to list of directions
		directions.push_back(BinToInt(thisGene));
	}

	return directions;
}

int	GeneticA::BinToInt(const std::vector<int> &vec)
{
	int val = 0;
	int multiplier = 1;
	
	for (int bit=vec.size(); bit>0; bit--)
	{
		val += vec[bit-1] * multiplier;
		
		multiplier *= 2;
	}

	return val;
}