#pragma once
#include "terminationCriteria.h"

class TerminationCriteriaIteration : TerminationCriteria
{
public:
	int endIteration;
	TerminationCriteriaIteration(int enditeration);
};

