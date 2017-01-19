#include "terminationCriteria.h"

TerminationCriteria::TerminationCriteria() { }

	void TerminationCriteria::setTerminationCondition(int iteration) {
		endIteration=iteration;
	}

	void TerminationCriteria::start (){
		currentIteration = 0;
		bestFoundIteration = 0;
		endTime = -1;
		startTime = Utility::get_cpu_time();
		bestFoundTime = startTime;
		started = true;
		cout << "Algorithm is running: \n" ;
	}

	bool TerminationCriteria::terminationCriteriaMet(){
		currentIteration ++;
		return (currentIteration == endIteration);
	}

	void TerminationCriteria::finish () {
		endTime = Utility::get_cpu_time();
	}

	void TerminationCriteria::recordBest(Solution& s) {	
		if (started && s.getSolutionFitness() < best->getSolutionFitness()) {
			best = &s;
			bestFoundTime = Utility::get_cpu_time();
			bestFoundIteration = currentIteration;
		}
	}


	double TerminationCriteria::getTimeToBest(){
		return (bestFoundTime - startTime);// / CLOCKS_PER_SEC;
	}
  
	double TerminationCriteria::getTotalTime(){
		return (endTime - startTime);// / CLOCKS_PER_SEC;
	}
  
	int TerminationCriteria::iterationsWithoutImproving() {
		return currentIteration - bestFoundIteration;
	}

//	Getters and Setters

	Solution TerminationCriteria::getBest() {
			return *best;
	}

	void TerminationCriteria::setBest(Solution& best) {
		this->best = &best;
	}

	string& TerminationCriteria::getInstanceName(){
		return best->problem_instance->instanceName;
	}

	int TerminationCriteria::getOptimalValue(){
		return best->problem_instance->optimalValue;
	}

	double TerminationCriteria::getObjectiveDiffrence(){
		return best->getSolutionFitness() - best->problem_instance->optimalValue;
	}

	int TerminationCriteria::getBestFoundIteration() {
		return bestFoundIteration;
	}

	void TerminationCriteria::setBestFoundIteration(int bestFoundIteration) {
		this->bestFoundIteration = bestFoundIteration;
	}

	double TerminationCriteria::getBestFoundTime() {
		return bestFoundTime;
	}

	void TerminationCriteria::setBestFoundTime(double bestFoundTime) {
		this->bestFoundTime = bestFoundTime;
	}

	int TerminationCriteria::getCurrentIteration() {
		return currentIteration;
	}

	void TerminationCriteria::setCurrentIteration(int currentIteration) {
		this->currentIteration = currentIteration;
	}

	double TerminationCriteria::getEndTime() {
		return endTime;
	}

	void TerminationCriteria::setEndTime(double endTime) {
		this->endTime = endTime;
	}

	double TerminationCriteria::getStartTime() {
		return startTime;
	}

	void TerminationCriteria::setStartTime(double startTime) {
		this->startTime = startTime;
	}

	bool TerminationCriteria::isStarted() {
		return started;
	}

	void TerminationCriteria::setStarted(bool started) {
		this->started = started;
	}

	void TerminationCriteria::showResult (){
		cout << fixed << showpoint << setprecision (2);
		cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl ;
		cout << setfill('.') 
			 << left << setw(30) << "Best result for instance: "	 << left << " " << getInstanceName()<<"-"<< getOptimalValue() << endl;
		cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl ;
		best->printSolution();
		cout << left << setw(30) << "Optimal value: "				 << left << " " << getOptimalValue()			<< endl;								
		cout << left << setw(30) << "Obtained value: "				 << left << " " << best->getSolutionFitness()	<< endl;
		cout << left << setw(30) << "Difference in objective: "		 << left << " " << getObjectiveDiffrence()		<< endl;
		cout << left << setw(30) << "Percentage similarities: "		 << left << " " << getOptimalValue() / best->getSolutionFitness() * 100 << " %" << endl;
		cout << left << setw(30) << "Iteration until best found: "	 << left << " " << getBestFoundIteration()		<< endl;
		cout << left << setw(30) << "Iteration without improvement: "<< left <<		  iterationsWithoutImproving () << endl ;
		cout << left << setw(30) << "Total iterations: "			 << left << " " << getCurrentIteration ()		<< endl ;
		cout << left << setw(30) << "Time until best found: "		 << left << " " << getTimeToBest ()				<< endl ;
		cout << left << setw(30) << "Total time elapsed: "			 << left << " " << getTotalTime ()				<< endl << endl ;
		cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl ;
	}

