#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

TEST(selectionStageTest, selectionStageTest_ALL_ALIVE) {

    int numSubpopulations = 3;
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
	double maxBrainCost = 100000;
	double deathRateLambda = 1.0;

	/*Initial values*/
	double initBrain = 1;
	double initAdaptKnowledge = 1;
	double initIndivLearning = 1.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 0.0;
	double initLearningBias = 0.0;
	double initPopSize = 10;

initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearning, initLearningBias,
			initAdaptKnowledge, P);

birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationValue, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation, indivLearningMean);

migrateStage(P, NP, numSubpopulations, migrationRate);

selectionStage(P, NP, numSubpopulations, deathRateLambda, maxBrainCost);

ASSERT_FALSE(NP[0].empty());
ASSERT_FALSE(NP[1].empty());
ASSERT_FALSE(NP[3].empty());
ASSERT_EQ(NP[0].size(), P[0].size());
ASSERT_EQ(NP[1].size(), P[1].size());
ASSERT_EQ(NP[2].size(), P[2].size());

}

TEST(selectionStageTest, selectionStageTest_HALF_ALIVE) {

    int numSubpopulations = 3;
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
	double maxBrainCost = 100000;
	double deathRateLambda = 1.0;

	/*Initial values*/
	double initBrain = 5;
	double initAdaptKnowledge = 1;
	double initIndivLearning = 1.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 0.0;
	double initLearningBias = 0.0;
	double initPopSize = 10;

initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearning, initLearningBias,
			initAdaptKnowledge, P);
/*
birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationValue, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation, indivLearningMean);

migrateStage(P, NP, numSubpopulations, migrationRate);
*/
selectionStage(P, NP, numSubpopulations, deathRateLambda, 2.0816);

int NPSize = NP[0].size() + NP[1].size() + NP[2].size();
int PSize = P[0].size() + P[1].size() + P[2].size();
EXPECT_NEAR(NPSize,PSize,10);

}


TEST(selectionStageTest, selectionStageTest_ALL_DEAD) {

    int numSubpopulations = 3;
    vector<double> meanCommunityCultureValues(numSubpopulations);
    vector<vector<Agent> > NP(numSubpopulations);
    vector<vector<Agent> > P(numSubpopulations);

	/*Birth Parameters*/
	int minPopsize = 10; /*Maximum popsize when no adaptive knowledge*/
	double fecundityCurvature = 10;
	double growthRateR = 0.04*20; //should vary between 0.005 and 0.04 multiplied by generation length
	double geneticMutationRate = 0;
	int brainMutationValue = 1;

	/*Learning Parameters*/
	double adaptKnowDeviation = 0.0;
	double lossyLearningValue = 1.0; //0 = Complete loss, 1 = No loss
	double obliqueLearningDeviation = 0.0;
	double learningBiasDeviation = 0.0;
	double indivLearningDeviation = 0.0;
	double indivLearningMean = 0.0;

	/*Migration Parameters*/
	double migrationRate = 0.1;

	/*Selection Parameters*/
	double maxBrainCost = 1;
	double deathRateLambda = 1.0;

	/*Initial values*/
	double initBrain = 0;
	double initAdaptKnowledge = 0;
	double initIndivLearning = 0.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 0.0;
	double initLearningBias = 0.0;
	double initPopSize = 10;

initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearning, initLearningBias,
			initAdaptKnowledge, NP);
/*
birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationValue, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation, indivLearningMean);

migrateStage(P, NP, numSubpopulations, migrationRate);
*/
selectionStage(P, NP, numSubpopulations, deathRateLambda, maxBrainCost);

ASSERT_TRUE(NP[0].empty());
ASSERT_TRUE(NP[1].size() == 0);
ASSERT_TRUE(NP[2].size() == 0);

}





int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
