#pragma once
#include <iomanip>
#include "solution.h"
#include <time.h>

using namespace std;

class Solution;
class TerminationCriteria {
		public:
		int currentIteration;
		int bestFoundIteration;
		double startTime;
		double endTime;
		double bestFoundTime;
		bool started ;
		int endIteration;
		Solution* best;

		TerminationCriteria();
		void setTerminationCondition(int iteration);
		void start();
		void recordBest(Solution& best);
		bool terminationCriteriaMet();
		void finish();
		double getTimeToBest();
		double getTotalTime();
		int iterationsWithoutImproving();
		//setter and getter
		Solution getBest();
		void setBest(Solution& best);
		void recordInitial(Solution& initial);
		string& getInstanceName();
		int getOptimalValue();
		double getObjectiveDiffrence();
		int getBestFoundIteration();
		void setBestFoundIteration(int bestFoundIteration) ;
		double getBestFoundTime();
		void setBestFoundTime(double bestFoundTime);
		int getCurrentIteration();
		void setCurrentIteration(int currentIteration);
		double getEndTime();
		void setEndTime(double endTime);
		double getStartTime();
		void setStartTime(double startTime);
		bool isStarted();
		void setStarted(bool started);
		void showResult();
};