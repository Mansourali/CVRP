#include "terminationcriteriaiteration.h"


TerminationCriteriaIteration::TerminationCriteriaIteration(int enditeration){
		endIteration = enditeration;
}

bool TerminationCriteriaIteration::timeToFinish(Solution& s) {   
	this->currentIteration++;
	return (currentIteration == endIteration);
}