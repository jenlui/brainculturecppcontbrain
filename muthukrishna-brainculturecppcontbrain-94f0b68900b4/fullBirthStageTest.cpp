#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>


//void fullBirthStageTest() {
TEST(fullBirthStageTest,fullBirthStageTest_OK){
    int numSubpopulations = 100;    //***changed numSubpopulations to 100***
    vector<double> meanCommunityCultureValues(numSubpopulations);
    vector<vector<Agent> > NP(numSubpopulations);
    vector<vector<Agent> > P(numSubpopulations);


	/*Birth Parameters*/

	int minPopsize = 10; /*Maximum popsize when no adaptive knowledge*/
	double fecundityCurvature = 10;
	double growthRateR = 0.04*20; //should vary between 0.005 and 0.04 multiplied by generation length
	double geneticMutationRate = 0.001;
	int brainMutationValue = 1;

	/*Learning Parameters*/
	double adaptKnowDeviation = 0.1;
	double lossyLearningValue = .9; //0 = Complete loss, 1 = No loss
	double obliqueLearningDeviation = 0.5;
	double learningBiasDeviation = 0.5;
	double indivLearningDeviation = 0.5;
	double indivLearningMean = 0.5;

	/*Migration Parameters*/
	double migrationRate = 0.1;

	/*Selection Parameters*/
	double maxBrainCost = 20;
	double deathRateLambda = 1.0;

	/*Initial values*/
	int initBrain = 1;
	double initAdaptKnowledge = 0.0;
	double initIndivLearning = 1.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 0.0;
	double initLearningBias = 0.0;
	double initPopSize = 10;

    initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearning, initLearningBias,initAdaptKnowledge, P);

    birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationValue, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

ASSERT_FALSE(NP.empty());
ASSERT_EQ(NP.size(), P.size());
ASSERT_TRUE(meanCommunityCultureValues.size() == numSubpopulations);



}




int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
