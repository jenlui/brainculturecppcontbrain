#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

/*
double mutateGeneticTrait(double geneticMutationRate, double deviation, double parentTraitValue, bool allowNegative, bool gt1) {
        //Assume individual learning bias is a genetic trait from the parent
        double trait = -1;
        double rand = randZeroOne();
        if (rand < geneticMutationRate) {
                trait = getNormalRandom(parentTraitValue, deviation);
                if (!gt1) {
                        trait = min(trait,1.0);
                }
                if (!allowNegative) {
                        trait = max(0.0, trait);
                }
        } else {
                trait = parentTraitValue;
        }
        return trait;
}
*/

TEST(mutateGeneticTraitTest, noMutateGeneticTrait_OK) {
double geneticMutationRate = -1;      // the if statement will never happen
double deviation = 1;
double parentTraitValue = 10;
double nonMutatedGeneticTrait = mutateGeneticTrait(geneticMutationRate, deviation, parentTraitValue, true, true);
ASSERT_TRUE(parentTraitValue == nonMutatedGeneticTrait);
}

TEST(mutateGeneticTraitTest, mutateGeneticTrait_OK){
double geneticMutationRate = 2;
double deviation = 0.0001;
double parentTraitValue = 1;
double mutatedGeneticTrait = mutateGeneticTrait(geneticMutationRate, deviation, parentTraitValue, true, true);
EXPECT_NEAR(1,mutatedGeneticTrait,0.1);
}

TEST(mutateGeneticTraitTest, maxMutateGeneticTrait_OK) {
double geneticMutationRate = 2;
double deviation = 0.5;
double parentTraitValue = 50;
double mutatedGeneticTrait = mutateGeneticTrait(geneticMutationRate, deviation, parentTraitValue, true, false);
ASSERT_TRUE(mutatedGeneticTrait == 1);
}

TEST(mutateGeneticTraitTest, minMutateGeneticTrait_OK) {
double geneticMutationRate = 2;
double deviation = 0.5;
double parentTraitValue = -50;
double mutatedGeneticTrait = mutateGeneticTrait(geneticMutationRate, deviation, parentTraitValue, false, true);
ASSERT_TRUE(mutatedGeneticTrait == 0);
}

TEST(mutateGeneticTraitTest, minAndMaxMutateGeneticTrait_OK){
double geneticMutationRate = 2;
double deviation = 0.0001;
double parentTraitValue = 1;
double mutatedGeneticTrait = mutateGeneticTrait(geneticMutationRate, deviation, parentTraitValue, false, false);
//ASSERT_TRUE(mutatedGeneticTrait == 1);
EXPECT_NEAR(1,mutatedGeneticTrait,0.01);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
