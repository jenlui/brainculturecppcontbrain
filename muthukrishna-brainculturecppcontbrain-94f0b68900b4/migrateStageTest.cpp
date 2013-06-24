#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

TEST(migrateStageTest, migrateStageTest_OK) {

    int numSubpopulations = 100;
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

//population size stayed the same
ASSERT_FALSE(NP.empty());
ASSERT_EQ(NP.size(), P.size());

}

TEST(migrateStageTest, migrateStageTest_NO_Migration) {

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
	double migrationRate = 0;

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

vector<Agent> subpop1(3);
vector<Agent> subpop2(3);
vector<Agent> subpop3(3);

subpop1.push_back(Agent(1, initIndivLearning, initObliqueLearning,	initLearningBias, 0.1));
subpop1.push_back(Agent(2, initIndivLearning, initObliqueLearning,	initLearningBias, 0.2));
subpop1.push_back(Agent(3, initIndivLearning, initObliqueLearning,	initLearningBias, 0.3));
subpop2.push_back(Agent(4, initIndivLearning, initObliqueLearning,	initLearningBias, 0.4));
subpop2.push_back(Agent(5, initIndivLearning, initObliqueLearning,	initLearningBias, 0.5));
subpop2.push_back(Agent(6, initIndivLearning, initObliqueLearning,	initLearningBias, 0.6));
subpop3.push_back(Agent(7, initIndivLearning, initObliqueLearning,	initLearningBias, 0.7));
subpop3.push_back(Agent(8, initIndivLearning, initObliqueLearning,	initLearningBias, 0.8));
subpop3.push_back(Agent(9, initIndivLearning, initObliqueLearning,	initLearningBias, 0.9));

P.push_back(subpop1);
P.push_back(subpop2);
P.push_back(subpop3);

birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationValue, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation, indivLearningMean);

migrateStage(P, NP, numSubpopulations, migrationRate);

//agents stay in the same place
for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < NP[p].size(); a++) {
ASSERT_TRUE(NP[p][a].getCulture() == P[p][a].getCulture());
            }
    }
}

TEST(migrateStageTest, migrateStageTest_ALL_Migration) {

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
	double migrationRate = 1;

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

vector<Agent> subpop1(3);
vector<Agent> subpop2(3);
vector<Agent> subpop3(3);

subpop1.push_back(Agent(1, initIndivLearning, initObliqueLearning,	initLearningBias, 0.1));
subpop1.push_back(Agent(2, initIndivLearning, initObliqueLearning,	initLearningBias, 0.2));
subpop1.push_back(Agent(3, initIndivLearning, initObliqueLearning,	initLearningBias, 0.3));
subpop2.push_back(Agent(4, initIndivLearning, initObliqueLearning,	initLearningBias, 0.4));
subpop2.push_back(Agent(5, initIndivLearning, initObliqueLearning,	initLearningBias, 0.5));
subpop2.push_back(Agent(6, initIndivLearning, initObliqueLearning,	initLearningBias, 0.6));
subpop3.push_back(Agent(7, initIndivLearning, initObliqueLearning,	initLearningBias, 0.7));
subpop3.push_back(Agent(8, initIndivLearning, initObliqueLearning,	initLearningBias, 0.8));
subpop3.push_back(Agent(9, initIndivLearning, initObliqueLearning,	initLearningBias, 0.9));

P.push_back(subpop1);
P.push_back(subpop2);
P.push_back(subpop3);

birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurvature, geneticMutationRate,
				brainMutationValue, indivLearningDeviation, obliqueLearningDeviation,
				learningBiasDeviation, lossyLearningValue, adaptKnowDeviation,
				indivLearningMean, NP);

learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
              lossyLearningValue, adaptKnowDeviation, indivLearningMean);

migrateStage(P, NP, numSubpopulations, migrationRate);

bool isMigrated = true;

//agents all moved to different locations
//for each agent in a subpop in NP, if that agent is in the same subpop as P[subpop], then return false

for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < NP[p].size(); a++) {
            int agent = NP[p][a].getCulture();
            for (int k = 0; k < P[p].size(); k++) {
                 if (agent == P[p][k].getCulture()) {isMigrated = false;}
            }
        }
}
ASSERT_TRUE(isMigrated);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
