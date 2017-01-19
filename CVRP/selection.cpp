#include "selection.h"

Selection::Selection(vector<Solution>& s) : solutios(&s) {
	allSolutionFittness = 0;
}

int Selection::getRandomSolution(){
	if (!solutios->empty())
		return rand() % solutios->size();
	else throw runtime_error("solutions size it empty");
}

void Selection::calculateAllSolutionFittness(){
	allSolutionFittness = 0;
	for (auto s = solutios->begin(); s != solutios->end(); s++)
		allSolutionFittness += s->getSolutionFitness();
}

vector<double> Selection::fitnessCalculation(){
	vector<double> fitness;
	for (auto s = solutios->begin(); s != solutios->end(); s++)
		fitness.push_back(1 / (1 + (s->getSolutionFitness())));
	return fitness;
}

vector<double> Selection::absoulutefitnessCalcuation(){

	vector<double> fit;
	double average;

	average = allSolutionFittness / double(solutios->size());

	for (auto s = solutios->begin(); s != solutios->end(); s++)
		fit.push_back(abs(s->getSolutionFitness() - average));

	return fit;
}

int Selection::getSolutionByTournament(){
	tournamentSelection();
	return rouletteWheelSelection();
}

int Selection::getSolutionByRank(int iteration){
	rankSelection(iteration);
	return rouletteWheelSelection();
}

int Selection::getSolutionByDisruptive(){
	disruptiveSelection();
	return rouletteWheelSelection();
}

int Selection::rouletteWheelSelection(){
	double tF = 0, sum = 0;

	for (auto s = solutios->begin(); s != solutios->end(); s++)
		tF += s->getProbabilityRank();

	double choisen = Utility::getRandomUniformNoBetween(0, tF);

	int j = 0; 
	for (auto s = solutios->begin(); s != solutios->end(); s++, j++){
		sum += s->getProbabilityRank();
		if (sum > choisen)
			return j;
	}
}

void Selection::tournamentSelection(){
	int tF = 0, ai;
	vector<int> a;
	double Sum = 0;

	random_shuffle(solutios->begin(), solutios->end());

	vector<double> fit = fitnessCalculation();

	for (auto s1 = solutios->begin(); s1 != solutios->end(); s1++){
		ai = 0;
		for (auto s2 = solutios->begin(); s2 != solutios->end(); s2++){
			if (s1 < s2)
				ai = ai + 1;
		}
		a.push_back(ai);
		Sum += ai;
	}

	int i = 0;
	for (auto s = solutios->begin(); s != solutios->end(); s++, i++)
		s->setProbabilityRank(a[i] / Sum);
}

void Selection::rankSelection(int maxIteration){

	double n = solutios->size();

	Solution::sortSolutionByFitness(solutios);
	reverse(solutios->begin(), solutios->end());

	int k = 0;
	for (auto s = solutios->begin(); s != solutios->end(); s++, k++) {
		double aT = 0.2 + ((3 * k) / (4 * double(maxIteration)));
		s->setProbabilityRank( (1 / n) + (aT * ((n + 1 - (2 * k)) / (n*(n + 1)))));
	}
}

void Selection::disruptiveSelection(){

	Solution::sortSolutionByFitness(solutios);
	vector<double> absFit = absoulutefitnessCalcuation();
	calculateAllSolutionFittness();
	double sum = allSolutionFittness;

	int i = 0;
	for (auto s = solutios->begin(); s != solutios->end(); s++, i++)
		s->setProbabilityRank(absFit[i] / sum);
}
