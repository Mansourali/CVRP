//=======================================================================================	
// Author      : Mansour Alssager, mansour.alssager@gmail.com
//=======================================================================================
#include "utility.h"
#include "instance.h"
#include "solution.h"
#include "solver.h"
#include "terminationCriteria.h"
#include "hillclimbing.h"
#include "GDHillclimbing.h"
#include "randomhillclimbing.h"
#include "iteratedlocalsearch.h"
#include "simulatedannealing.h"
//#include "tabusearch.h"
#include "greatdeluge.h"
#include "cuckoosearch_rand.h"
#include "cuckoosearch_best.h"
using namespace setting;

int main () {

	Utility u = Utility(); 

	u.POPULATION.setNEIGHBORHOOD_STRUCTURE	( SHIFT_1_0, SHIFT_2_0, SWAP_1_1, SWAP_1_2, SWAP_2_2, CROSS, K_SHIFT, REINSERTION, OR_OPT2, OR_OPT3, TWO_OPT, EXCAHNGE);
	u.POPULATION.setNEIGHBOR_SELECTOR		( neighborSelectorType::BEST );
	u.POPULATION.setTERMINATION_CRITERIA	( 700 );
	u.POPULATION.setPOPULATION_SIZE			( 40 );
	u.POPULATION.setSELECTION_STRATEGY		( selectionStrategy::DISRUPTIVE );

	u.SINGLE.setNEIGHBORHOOD_STRUCTURE		( SHIFT_1_0, SHIFT_2_0, SWAP_1_1, SWAP_1_2, SWAP_2_2, CROSS, K_SHIFT, REINSERTION, OR_OPT2, OR_OPT3, TWO_OPT, EXCAHNGE);
	u.SINGLE.setNEIGHBOR_SELECTOR			( neighborSelectorType::BEST ); // improvements strategy
	u.SINGLE.setTERMINATION_CRITERIA		( 300 );

	int maxRun = 21;

	vector<vector<double>> constructionDiver, constructionBest;

		u.readAllInstancesName("data/"); 

		for (size_t currentInstance = 9; currentInstance < 16; currentInstance++)	{

		Instance instance;
		vector<InstanceResult> allRunResult(maxRun, InstanceResult());

		for (size_t i = 0; i < maxRun; i++){

			try { 
				instance.loadInstance(u.instancesName[currentInstance], Instance::integerType); }
			catch (VRPException &e) { cout << e.what(); }

			vector<Solution> initialSolutions(u.POPULATION.getPOPULATION_SIZE(), Solution(&instance));
			vector<Solution> newSolutions	 (u.POPULATION.getPOPULATION_SIZE(), Solution());

			///TerminationCriteria hcc;
			//hcc.start();
			initialSolutions = Solution::solutionsFactory(&initialSolutions, initializationTechnique::NN_FR_DEPOT_NEAREST_ADD, u);
			//hcc.setBest(initialSolutions[0]); hcc.finish(); cout << "Finished in: " << hcc.getTotalTime() << endl;
			newSolutions = initialSolutions;
			Solution bestSolution = Solution::getCurrentBestSolution(&newSolutions);

			//diversification calculation
			//	constructionDiver.push_back(Solution::measureDiversification(initialSolutions, instance.minTrucks));
			//vector<double> best;
			//for (vector<Solution>::iterator it = initialSolutions.begin(); it != initialSolutions.end(); ++it)
			//	best.push_back(it->getInitialFitness());
			//constructionBest.push_back(best);

			TerminationCriteria tc;
			tc.setTerminationCondition(u.getTERMINATION_CRITERIA());
			tc.setBest(bestSolution);

			//Solver<Solution>*    localSearch = new HillClimbing(SINGLE_BASED, u, newSolutions[newSolutions.size() - 1]);
			//Solver<Solution>* localSearch = new SimulatedAnnealing(SINGLE_BASED, u, newSolutions[newSolutions.size() - 1]);
			Solver<vector<Solution>>* BasicCuckooSeachRandNeighbour = new CuckooSearch_rand(POPULATION_BASED, u, newSolutions, tc, 0.25);
			Solver<vector<Solution>>* BasicCuckooSeachBestNeighbour = new CuckooSearch_best(POPULATION_BASED, u, newSolutions, tc, 0.25);

			tc.start();

			try {
				BasicCuckooSeachRandNeighbour->run();
			}
			catch (VRPException &e) { cout << endl << e.what() << endl; };

			//while (!tc.terminationCriteriaMet()) { // hybrid solver
			//	try {
			//	//printf("Iteration= %i :\n", tc.currentIteration);
			//		solver->run();
			//		if (solver->getCurrentState() < tc.getBest())
			//			tc.recordBest(Solution::getCurrentBestSolution(&newSolutions));
			//		localSearch->CurrentSolution = solver->CurrentSolution;
			//			localSearch->run();
			//		if (localSearch->getCurrentState() < tc.getBest())
			//			tc.recordBest(Solution::getCurrentBestSolution(&newSolutions));
			//		Utility::spinner();
			//	}
			//	catch (VRPException &e) { cout << endl << e.what() << endl; };
			//}

			tc.recordBest(Solution::getCurrentBestSolution(&newSolutions));
			tc.finish();
			tc.showResult();

			//u.printSetting(u.SINGLE);
			u.printSetting(u.POPULATION);

			allRunResult.at(i).saveInstanceRunDetails(tc.best->getInitialFitness(), tc.best->getSolutionFitness(), tc.getTimeToBest(), tc.getObjectiveDiffrence(), tc.getTotalTime(), tc.getOptimalValue());
			for (int i = 0; i < initialSolutions.size();i++) {
				cout << initialSolutions[i].getSolutionFitness()<< "  " << newSolutions[i].getSolutionFitness() << endl;
			}

			delete BasicCuckooSeachRandNeighbour; //delete localSearch;
		}

			//Utility::printTwoDimintionVector(constructionBest);
			//Utility::printTwoDimintionVector(constructionDiver);
			//constructionBest.clear(); constructionDiver.clear(); cout << currentInstance << ". previous instance: " << u.instancesName.at(currentInstance) << endl;

		if (maxRun > 1) u.printSetting(allRunResult);
	}

	system("PAUSE");

	return EXIT_SUCCESS;
};
