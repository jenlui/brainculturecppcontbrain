#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

TEST(learningStageTest, learningStageTest_OK) {

    int numSubpopulations = 100;
    vector<double> meanCommunityCultureValues(numSubpopulations);
    vector<vector<Agent> > NP(numSubpopulations);
    vector<vector<Agent> > P(numSubpopulations);

	/*Birth Parameters*/
	int minPopsize = 10; /*Maximum popsize when no adaptive knowledge*/
	double fecundityCurvature = 10;
	double growthRateR = 0.04*20; //should vary between 0.005 and 0.04 multiplied by generation length
	double geneticMutationRate = 0.001;

	/*Learning Parameters*/
	double adaptKnowDeviation = 0.1;
	double lossyLearningValue = .9; //0 = Complete loss, 1 = No loss
	double obliqueLearningDeviation = 0.5;
	double learningBiasDeviation = 0.5;
	double indivLearningDeviation = 0.5;
	double brainMutationDeviation = 0.5;
	double indivLearningMean = 0.5;

	/*Migration Parameters*/
	double migrationRate = 0.1;

	/*Selection Parameters*/
	double maxBrainCost = 20;
	double deathRateLambda = 1.0;

	/*Initial values*/
	double initBrain = 1;
	double initAdaptKnowledge = 0.0;
	double initIndivLearning = 1.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 0.0;
	double initLearningBias = 5.0;
	double initPopSize = 10;

initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearning, initLearningBias,
			initAdaptKnowledge, P);

birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationDeviation, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);


vector<vector<Agent> > PopulationPreLearning(NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation,indivLearningMean);

vector<vector<Agent> > PopulationPostLearning(NP);

ASSERT_FALSE(NP.empty());
ASSERT_EQ(NP.size(), P.size());

//test that culture of agents are greater than/equal to original culture
for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < NP[p].size(); a++) {

ASSERT_TRUE(PopulationPostLearning[p][a].getCulture() >= PopulationPreLearning[p][a].getCulture());
            }
    }

}
//**************************************************************************************//

// test that agents with high learning bias pick smarter agents to learn from
TEST(learningStageTest, learningBiasAffects) {

 int numSubpopulations = 100;
    vector<double> meanCommunityCultureValues(numSubpopulations);
    vector<vector<Agent> > NP(numSubpopulations);
    vector<vector<Agent> > P(numSubpopulations);

	/*Birth Parameters*/
	int minPopsize = 10; /*Maximum popsize when no adaptive knowledge*/
	double fecundityCurvature = 10;
	double growthRateR = 0.04*20; //should vary between 0.005 and 0.04 multiplied by generation length
	double geneticMutationRate = 0.0;


	/*Learning Parameters*/
	double adaptKnowDeviation = 0.5;
	double lossyLearningValue = 1; //0 = Complete loss, 1 = No loss
	double obliqueLearningDeviation = 0;
	double learningBiasDeviation = 0;
	double indivLearningDeviation = 0;
	double brainMutationDeviation = 0;
	double indivLearningMean = 1;

	/*Migration Parameters*/
	double migrationRate = 0.1;

	/*Selection Parameters*/
	double maxBrainCost = 20;
	double deathRateLambda = 1.0;

	/*Initial values*/
	double initBrain = 400;
	double initAdaptKnowledge = 200;
	double initIndivLearning = 0.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 1;
	double initLearningBias = 0.0;
	double initPopSize = 50;

initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearning, initLearningBias,
			initAdaptKnowledge, P);

birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationDeviation, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation,indivLearningMean);

double NPcultureValueLOW = 0;

for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < NP[p].size(); a++) {

NPcultureValueLOW = NP[p][a].getCulture() + NPcultureValueLOW;
            }
    }



// now test for larger LB to see affects

int numSubpopulations1 = 100;
    vector<double> meanCommunityCultureValues1(numSubpopulations);
    vector<vector<Agent> > NP1(numSubpopulations);
    vector<vector<Agent> > P1(numSubpopulations);

    /*Birth Parameters*/
	int minPopsize1 = 10; /*Maximum popsize when no adaptive knowledge*/
	double fecundityCurvature1 = 10;
	double growthRateR1 = 0.04*20; //should vary between 0.005 and 0.04 multiplied by generation length
	double geneticMutationRate1 = 0.0;

	/*Learning Parameters*/
	double adaptKnowDeviation1 = 0.5;
	double lossyLearningValue1 = 1; //0 = Complete loss, 1 = No loss
	double obliqueLearningDeviation1 = 0;
	double learningBiasDeviation1 = 0;
	double indivLearningDeviation1 = 0;
	double brainMutationDeviation1 = 0;
	double indivLearningMean1 = 1;

	/*Migration Parameters*/
	double migrationRate1 = 0.1;

	/*Selection Parameters*/
	double maxBrainCost1 = 20;
	double deathRateLambda1 = 1.0;

	/*Initial values*/
	double initBrain1 = 400;
	double initAdaptKnowledge1 = 200;
	double initIndivLearning1 = 0.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning1 = 1;
	double initLearningBias1 = 1000.0;
	double initPopSize1 = 50;


initializePopulation(numSubpopulations1, initPopSize1, initBrain1,
			initIndivLearning1, initObliqueLearning1, initLearningBias1,
			initAdaptKnowledge1, P1);

birthStage(numSubpopulations1, P1, meanCommunityCultureValues1,
				growthRateR1, minPopsize1, fecundityCurvature1, geneticMutationRate1,
				brainMutationDeviation1, indivLearningDeviation1, obliqueLearningDeviation1,
				learningBiasDeviation1, lossyLearningValue1, adaptKnowDeviation1,
				indivLearningMean1, NP1);

learningStage(numSubpopulations1, NP1, P1, meanCommunityCultureValues1,
              lossyLearningValue1, adaptKnowDeviation1, indivLearningMean1);


double NPcultureValueHIGH = 0;

for (int p = 0; p < numSubpopulations1; p++) {
		for (int a = 0; a < NP1[p].size(); a++) {

NPcultureValueHIGH = NP1[p][a].getCulture() + NPcultureValueHIGH;
            }
    }

ASSERT_TRUE(NPcultureValueHIGH > NPcultureValueLOW);


}






int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
