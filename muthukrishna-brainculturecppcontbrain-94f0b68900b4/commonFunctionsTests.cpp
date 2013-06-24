#include "gtest/gtest.h"
#include <vector>
#include <string>
#include  "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"

TEST(commonFunctionTests, getMeanCommunityCulturePASS) {


double b = 2.0;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
double C = 0.5;

// initialize subpopulation vector
std::vector<Agent> subpop;

// create agents to put in subpop
Agent testAgent1 = Agent(b, il, ol, lb, C);
Agent testAgent2 = Agent(b, il, ol, lb, C);
Agent testAgent3 = Agent(b, il, ol, lb, C);

// insert agents into subpop vector
subpop.push_back(testAgent1);
subpop.push_back(testAgent2);
subpop.push_back(testAgent3);



ASSERT_EQ(0.5, getMeanCommunityCulture(subpop));

}

TEST(commonFunctionTests, getMeanCommunityCultureZERO) {


double b = 2.0;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
double C = 0;

// initialize subpopulation vector
std::vector<Agent> subpop;

// create agents to put in subpop
Agent testAgent1 = Agent(b, il, ol, lb, C);
Agent testAgent2 = Agent(b, il, ol, lb, C);
Agent testAgent3 = Agent(b, il, ol, lb, C);

// insert agents into subpop vector
subpop.push_back(testAgent1);
subpop.push_back(testAgent2);
subpop.push_back(testAgent3);



ASSERT_EQ(0, getMeanCommunityCulture(subpop));

}

TEST(commonFunctionTests, getMeanCommunityCultureFAIL) {


double b = 2.0;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
// invalid value, exception should be thrown when creating agents
double C = -0.5;


// initialize subpopulation vector
std::vector<Agent> subpop;

EXPECT_ANY_THROW(  {

// create agents to put in subpop
Agent testAgent1 = Agent(b, il, ol, lb, C);
// code should never reach here
FAIL();
Agent testAgent2 = Agent(b, il, ol, lb, C);
Agent testAgent3 = Agent(b, il, ol, lb, C);

// insert agents into subpop vector
subpop.push_back(testAgent1);
subpop.push_back(testAgent2);
subpop.push_back(testAgent3);



ASSERT_EQ(-0.5, getMeanCommunityCulture(subpop));

};
);
}



TEST(commonFunctionTests, carryingCapacityTestCompare_OKAY1) {

int scale = 1;
int minPop = 10;
double lowMeanCulture[5] = {1,10,25,40,50 };
double highMeanCulture[5] = {55,60,75,90,100};

for (int i=0; i<4 ;i++) {
double lowCultureCarryCapacity = carryingCapacity(scale, minPop, lowMeanCulture[i]);
double highCultureCarryCapacity = carryingCapacity(scale, minPop, highMeanCulture[i]);

ASSERT_TRUE(lowCultureCarryCapacity < highCultureCarryCapacity);
}

}

TEST(commonFunctionTests, carryingCapacityTestValues_OKAY2) {

int scale = 2;

int minPop = 10;
double meanCulture = 0.5;
double carryCapacity = carryingCapacity(scale, minPop, meanCulture);
double abs_error = 0.05;
EXPECT_NEAR(10.81, carryCapacity, abs_error);

}




TEST(commonFunctionTests, carryingCapacityTestValues_OKAY3) {

int scale = 10;
int minPop = 10;
double meanCulture = 0.5;
int carryCapacity = carryingCapacity(scale, minPop, meanCulture);
EXPECT_EQ(14, carryCapacity);

}

TEST(commonFunctionTests, numChildrenTest_COMPARE) {

int N0 = 10;
int minPop = 10;
double r = 0.8;
double small_c = 0.5;
double big_c = 1.0;

int lowNumChildren = numChildren(N0,r,small_c,minPop);
int highNumChildren = numChildren(N0,r,big_c,minPop);

ASSERT_TRUE(lowNumChildren < highNumChildren);

}


TEST(commonFunctionTests, numChildrenTest_VALUE) {

int N0 = 10;
int minPop = 10;
double r = 0.8;
double c = 0.1;

int numOfChildren = numChildren(N0,r,c,minPop);

// TODO numChildren() returns an int (rounded down?)
EXPECT_NEAR(10,numOfChildren,0.1);

}

TEST(commonFunctionTests, getTotalCommunityCultureToPowerTest) {

double b = 2.0;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
double C = 0.75;

// initialize subpopulation vector
std::vector<Agent> subpop;

// create agents to put in subpop
Agent testAgent1 = Agent(b, il, ol, lb, C);
Agent testAgent2 = Agent(b, il, ol, lb, C);
Agent testAgent3 = Agent(b, il, ol, lb, C);

// insert agents into subpop vector
subpop.push_back(testAgent1);
subpop.push_back(testAgent2);
subpop.push_back(testAgent3);

double cultureToPower = getTotalCommunityCultureToPower(subpop, 2.5);

EXPECT_NEAR(1.461, cultureToPower, 0.1);

}

TEST(commonFunctionTests, sigmoidTestValuePASS) {

double value = 1.5;
double inflection = 1.2;
double curvature = 2.0;

double valueReturned = sigmoid(value, inflection, curvature);
EXPECT_NEAR(0.65, valueReturned, 0.1);
}

TEST(commonFunctionTests, sigmoidTestValue_FAILValue) {

double value = -1.0;
// inflection can't be too big??
double inflection = 1.2;
double curvature = 2.0;

EXPECT_ANY_THROW( {

double valueReturned = sigmoid(value, inflection, curvature);
EXPECT_NEAR(0.35, valueReturned, 0.1);

});
}

TEST(commonFunctionTests, sigmoidTestValue_FAIL_Curvature) {

double value = 1.5;
// inflection can't be too big??
double inflection = 1.2;
double curvature = -2.0;

EXPECT_ANY_THROW( {

double valueReturned = sigmoid(value, inflection, curvature);
EXPECT_NEAR(0.65, valueReturned, 0.1);

});
}

TEST(commonFunctionTests, sigmoidTestValueValue) {

double value = 1.5;
// inflection can't be too big??
double inflection = 1.2;
double curvature = 2.0;

double valueReturned = sigmoid(value, inflection, curvature);
EXPECT_NEAR(0.65, valueReturned, 0.1);

}

TEST(commonFunctionTests, getTotalCommunityCultureFromSigmoidTest) {

double b = 2.0;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
double C = 0.5;

// initialize subpopulation vector
std::vector<Agent> subpop;

// create agents to put in subpop
Agent testAgent1 = Agent(b, il, ol, lb, C);
Agent testAgent2 = Agent(b, il, ol, lb, C);
Agent testAgent3 = Agent(b, il, ol, lb, C);

// insert agents into subpop vector
subpop.push_back(testAgent1);
subpop.push_back(testAgent2);
subpop.push_back(testAgent3);

double meanCulture = getMeanCommunityCulture(subpop);

ASSERT_EQ(meanCulture,0.5);

ASSERT_EQ(sigmoid(0.5,meanCulture, 2.0) + sigmoid(0.5, meanCulture, 2.0) + sigmoid(0.5, meanCulture, 2.0),getTotalCommunityCultureFromSigmoid(subpop,meanCulture, 2.0));


}

TEST(commonFunctionTests, deathRateDIETest) {

double deathRateLambda = 1;
double brain = 0;
double culture = 0.5;


ASSERT_EQ(1, deathRate(deathRateLambda,brain,brain,culture, 100));

}

TEST(commonFunctionTests, deathRateCompareTest_BrainConst) {

double deathRateLambda = 1;
double smallB = 1.0;
double bigB = 15.0;
double brain=10;
double highCulture = 10;
double lowCulture = 9;

ASSERT_TRUE(deathRate(deathRateLambda,brain,brain,highCulture, 100) < deathRate(deathRateLambda,brain,brain,lowCulture, 100));

ASSERT_TRUE(deathRate(deathRateLambda,smallB,smallB,highCulture, 100) < deathRate(deathRateLambda,bigB,bigB,highCulture, 100));
}

TEST(commonFunctionTests, deathRateValueTest) {

double deathRateLambda = 1;
double brain = 10;
double culture = 1.0;
double brainCost = 100;


EXPECT_NEAR(0.09,deathRate(deathRateLambda,brain, brain, culture, brainCost), 0.1);


}

TEST(commonFunctionTests, getMeanPopulationParametersTest) {

double b = 50;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
double C = 1.0;

double b2 = 25;
double il2 = 0.5;
double ol2 = 0.5;
double lb2 = 0.5;
double C2 = 0.5;

double b3 = 25;
double il3 = 0.5;
double ol3 = 0.5;
double lb3 = 0.5;
double C3 = 0.0;

// initialize subpopulation vector
std::vector<std::vector<Agent> > testPop;
std::vector<Agent> subpop1;
std::vector<Agent> subpop2;
std::vector<Agent> subpop3;

// create agents to put in subpop
Agent testAgent1_1 = Agent(b, il, ol, lb, C);
Agent testAgent1_2 = Agent(b, il, ol, lb, C);
Agent testAgent1_3 = Agent(b, il, ol, lb, C);


Agent testAgent2_1= Agent(b2, il2, ol2, lb2, C2);
Agent testAgent2_2 = Agent(b2, il2, ol2, lb2, C2);
Agent testAgent2_3 = Agent(b2, il2, ol2, lb2, C2);

Agent testAgent3_1= Agent(b3, il3, ol3, lb3, C3);
Agent testAgent3_2 = Agent(b3, il3, ol3, lb3, C3);
Agent testAgent3_3 = Agent(b3, il3, ol3, lb3, C3);


// insert agents into subpop vector
subpop1.push_back(testAgent1_1);
subpop1.push_back(testAgent1_2);
subpop1.push_back(testAgent1_3);

subpop2.push_back(testAgent2_1);
subpop2.push_back(testAgent2_2);
subpop2.push_back(testAgent2_3);


subpop3.push_back(testAgent3_1);
subpop3.push_back(testAgent3_2);
subpop3.push_back(testAgent3_3);

testPop.push_back(subpop1);
testPop.push_back(subpop2);
testPop.push_back(subpop3);

vector<double> expectedOutput;
double brainMean = 300.0/9;
double cultureMean= 4.5/9; //187.5/9
double iLearningTotal = 0.5;
double oLearningTotal = 0.5;
double learningBiasTotal = 0.5;
expectedOutput.push_back(brainMean);
expectedOutput.push_back(cultureMean);
expectedOutput.push_back(iLearningTotal);
expectedOutput.push_back(oLearningTotal);
expectedOutput.push_back(learningBiasTotal);



ASSERT_EQ(expectedOutput, getMeanPopulationParameters(testPop));

}






int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
