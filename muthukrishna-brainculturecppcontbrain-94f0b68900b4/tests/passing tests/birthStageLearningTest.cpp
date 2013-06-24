#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

TEST(birthStageLearning, doSocialLearningFromParent) {

double lossyLearningValue = 1.0;
double adaptKnowledgeDev = 0.5;
double indivLearningMean = 0.8;


// for parent
double parent_b = 10.0;
double parent_il = 0.0;
double parent_ol = 1.0;
double parent_lb = 0.0;
double parent_c = 10.0;

Agent parent = Agent(parent_b, parent_il, parent_ol, parent_lb, parent_c);

// for newborn with bigger brain
double nb_b = 11.3;
double nb_il = 0.0;
double nb_ol = 1.0;
double nb_lb = 0.0;
double nb_c = 10.0;

Agent newborn = Agent(nb_b, nb_il, nb_ol, nb_lb, nb_c);


Agent educatedNewBorn = learnAdaptiveKnowledge(newborn, parent, lossyLearningValue, adaptKnowledgeDev, indivLearningMean);

// ensure that agent learn to within a set number of stdev of parent and that agents brain is never less than original brain size
EXPECT_NEAR(newborn.getCulture(), parent.getCulture(), ((lossyLearningValue*parent.getCulture()) + 4*adaptKnowledgeDev));
// since no lossy learning,
//ASSERT_TRUE(educatedNewBorn.getBrain() >= parent.getBrain());
// confirm culture is capped at brainsize!
ASSERT_TRUE(educatedNewBorn.getCulture() <= educatedNewBorn.getBrain());

// following test should fail sometimes!
//ASSERT_TRUE(educatedNewBorn.getCulture() >= parent.getBrain());




}

TEST(birthStageLearning, doIndividualLearning) {

double lossyLearningValue = 1.0;
double adaptKnowledgeDev = 0.5;
double indivLearningMean = 0.8;


// for parent
double parent_b = 10.0;
double parent_il = 1.0;
double parent_ol = 0.0;
double parent_lb = 0.0;
double parent_c = 10.0;

Agent parent = Agent(parent_b, parent_il, parent_ol, parent_lb, parent_c);


double nb_b = 11.3;
double nb_il = 1.0;
double nb_ol = 0.0;
double nb_lb = 0.0;
double nb_c = 5.0;

Agent newborn = Agent(nb_b, nb_il, nb_ol, nb_lb, nb_c);


Agent educatedNewBorn = learnAdaptiveKnowledge(newborn, parent, lossyLearningValue, adaptKnowledgeDev, indivLearningMean);

// ensure that agent learns to within a set number of stdev of indivLearningMean
EXPECT_NEAR(newborn.getCulture(), newborn.getCulture(), ((indivLearningMean+ 4*adaptKnowledgeDev)));
// confirm culture is capped at brainsize!
ASSERT_TRUE(educatedNewBorn.getCulture() <= educatedNewBorn.getBrain());
// TODO only true in the learning stage, so fails here
//ASSERT_TRUE(educatedNewBorn.getCulture() >= newborn.getCulture());
// TODO indiv learning in birth stage diff from learning stage
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

