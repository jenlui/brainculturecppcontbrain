#include "gtest/gtest.h"
#include <vector>
#include <string>
#include  "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"

TEST(initializePopulationTests, initializePopulationTest_OK) {


     int numSubpopulations = 100;
     double initPopSize = 10.0;
     int initBrain = 1;
     double initIndivLearning = 1.0;
     double initObliqueLearn = 0.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = 0.0;
     vector<vector<Agent> > P(numSubpopulations);

    initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,
                          initObliqueLearn, initLearningBias, initAdaptKnowledge, P);


    ASSERT_FALSE(P.empty());
    ASSERT_EQ(numSubpopulations, P.size());

    // double check all agents in P have initAdaptKnowledge
    for (int p = 0; p < numSubpopulations; p++) {
        vector<Agent> subpop = P[p];
		for (int a = 0; a < initPopSize; a++) {
        ASSERT_EQ(initAdaptKnowledge, subpop[a].getCulture());
		}
        subpop.clear();
        }

    // double check all agents in P have initBrain size
    for (int p = 0; p < numSubpopulations; p++) {
        vector<Agent> subpop = P[p];
		for (int a = 0; a < initPopSize; a++) {
        ASSERT_EQ(initBrain, subpop[a].getBrain());
		}
        subpop.clear();
        }

    // double check all agents in P have initBrain
    for (int p = 0; p < numSubpopulations; p++) {
        vector<Agent> subpop = P[p];
		for (int a = 0; a < initPopSize; a++) {
        ASSERT_EQ(initBrain, subpop[a].getBrain());
		}
        subpop.clear();
        }

    // double check all agents in P have initObliqueLearn
    for (int p = 0; p < numSubpopulations; p++) {
        vector<Agent> subpop = P[p];
		for (int a = 0; a < initPopSize; a++) {
        ASSERT_EQ(initObliqueLearn, subpop[a].getObliqueLearning());
		}
        subpop.clear();
        }

    // double check all agents in P have initLearningBias
    for (int p = 0; p < numSubpopulations; p++) {
        vector<Agent> subpop = P[p];
		for (int a = 0; a < initPopSize; a++) {
        ASSERT_EQ(initLearningBias, subpop[a].getLearningBias());
		}
        subpop.clear();
        }



}


TEST(initializePopulationTests, initializePopulationTest_throwInvalidNumOfSubPop) {


     int numSubpopulations = -100;
     double initPopSize = 10.0;
     int initBrain = 1;
     double initIndivLearning = 1.0;
     double initObliqueLearn = 0.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = 0.0;
     vector<vector<Agent> > P;

     // Make sure exception is thrown if agents is not initialized with valid numSubpopulations

    EXPECT_ANY_THROW(  {
                     initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,initObliqueLearn, initLearningBias, initAdaptKnowledge, P);
                        };
                    );

}

TEST(initializePopulationTests, initializePopulationTest_throwInvalidInitPopSize) {

     int numSubpopulations = 100;
     double initPopSize = -10.0;
     int initBrain = 1;
     double initIndivLearning = 0.0;
     double initObliqueLearn = 0.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = 0.0;
     vector<vector<Agent> > P;

     // Make sure exception is thrown if population is not initialized with a valid PopSize

    EXPECT_ANY_THROW(  {
                     initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,initObliqueLearn, initLearningBias, initAdaptKnowledge, P);
                        };
                    );

}

TEST(initializePopulationTests, initializePopulationTest_throwInvalidBrainSize) {

     int numSubpopulations = 100;
     double initPopSize = 10.0;
     int initBrain = -1;
     double initIndivLearning = 1.0;
     double initObliqueLearn = 0.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = 0.0;
     vector<vector<Agent> > P;

    EXPECT_ANY_THROW(  {
                     initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,initObliqueLearn, initLearningBias, initAdaptKnowledge, P);
                        };
                    );

}

TEST(initializePopulationTests, initializePopulationTest_throwInvalidIndivLearning) {

     int numSubpopulations = 100;
     double initPopSize = 10.0;
     int initBrain = 1;
     double initIndivLearning = -1.0;
     double initObliqueLearn = 0.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = 0.0;
     vector<vector<Agent> > P;

    EXPECT_ANY_THROW(  {
                     initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,initObliqueLearn, initLearningBias, initAdaptKnowledge, P);
                        };
                    );

}

TEST(initializePopulationTests, initializePopulationTest_throwInvalidObliqueLearning) {

     int numSubpopulations = 100;
     double initPopSize = 10.0;
     int initBrain = 1;
     double initIndivLearning = 1.0;
     double initObliqueLearn = -1.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = 0.0;
     vector<vector<Agent> > P;

    EXPECT_ANY_THROW(  {
                     initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,initObliqueLearn, initLearningBias, initAdaptKnowledge, P);
                        };
                    );

}

TEST(initializePopulationTests, initializePopulationTest_throwInvalidAdaptKnowledge) {

     int numSubpopulations = 100;
     double initPopSize = 10.0;
     int initBrain = 1;
     double initIndivLearning = 1.0;
     double initObliqueLearn = 0.0;
     double initLearningBias = 0.0;
     double initAdaptKnowledge = -1.0;
     vector<vector<Agent> > P;

    EXPECT_ANY_THROW(  {
                     initializePopulation(numSubpopulations, initPopSize, initBrain, initIndivLearning,initObliqueLearn, initLearningBias, initAdaptKnowledge, P);
                        };
                    );

}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
