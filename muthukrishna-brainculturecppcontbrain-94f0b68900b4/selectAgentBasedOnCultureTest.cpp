#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>



TEST(selectParentTest, statisticallyOK) {


// initializing agent characteristics for subpop[0]
double b = 1000.0;
double il = 0.0;
double ol = 0.0;
double lb = 0.0;
double dumb = 0.0;
double avg= 0.50;
double smart=1.0;

//initializing agent characteristics for subpop[1]
double b1 = 10.0;
double dumb1 = 0.0;
double avg1= 0.25;
double smart1=0.50;

//initializing vectors to store Agents
std::vector<std::vector<Agent> > Population;
std::vector<Agent> parents;
std::vector<Agent> parents2;

// creating parents for subpop(0)
Agent dumbParent = Agent(b, il, ol, lb, dumb);
Agent avgParent = Agent(b, il, ol, lb, avg);
Agent smartParent = Agent(b, il, ol, lb, smart);

parents.push_back(dumbParent);
parents.push_back(avgParent);
parents.push_back(smartParent);

Population.push_back(parents);

// creating parents for subpop(1)
Agent dumbParent1 = Agent(b1, il, ol, lb, dumb1);
Agent avgParent1 = Agent(b1, il, ol, lb, avg1);
Agent smartParent1 = Agent(b1, il, ol, lb, smart1);

parents2.push_back(dumbParent1);
parents2.push_back(avgParent1);
parents2.push_back(smartParent1);

Population.push_back(parents2);

// sending variables for the parent selection
int fecundityCurve = 1.0;
double meanCommunityCulture = getMeanCommunityCulture(Population[0]);
double sigmoidCurvature = 10.0;
double totalCommunityCulture = getTotalCommunityCultureFromSigmoid(
					Population[0], meanCommunityCulture, fecundityCurve);

int dumbParentCount =0;
int avgParentCount =0;
int smartParentCount =0;

// Have 1000 newborns pick parent

for (int i=0; i<15000; i++) {



Agent parent = selectAgentBasedOnCulture(Population,0,meanCommunityCulture,
                                         fecundityCurve, totalCommunityCulture);


//cout << parent.getCulture() << endl;

if (parent.getCulture() == dumb) {
    dumbParentCount++;
}
if (parent.getCulture() == avg) {
    avgParentCount++;
}

if (parent.getCulture() == smart) {
    smartParentCount++;
}

}

    cout << "smarter subpopulation dumbparent count " << dumbParentCount << endl;
    cout << "smarter subpopulation averageParent count " << avgParentCount << endl;
    cout << "smarter subpopulation smartParent count " << smartParentCount << endl;

// sending variables for the parent selection
int fecundityCurve1 = 1.0;
double meanCommunityCulture1 = getMeanCommunityCulture(Population[1]);
double sigmoidCurvature1 = 10.0;

double totalCommunityCulture1 = getTotalCommunityCultureFromSigmoid(
					Population[1], meanCommunityCulture1, fecundityCurve1);


int dumbParentCount1 =0;
int avgParentCount1 =0;
int smartParentCount1 =0;

for (int i=0; i<15000; i++) {



Agent parent = selectAgentBasedOnCulture(Population,1,meanCommunityCulture1,
                                         fecundityCurve1, totalCommunityCulture1);


//cout << parent.getCulture() << endl;

if (parent.getCulture() == dumb1) {
    dumbParentCount1++;
}
if (parent.getCulture() == avg1) {
    avgParentCount1++;
}

if (parent.getCulture() == smart1) {
    smartParentCount1++;
}

}

cout << "dumber subpopulation dumbparent count " << dumbParentCount1 << endl;
cout << "dumber subpopulation averageParent count " << avgParentCount1 << endl;
cout << "dumber subpopulation smartParent count " << smartParentCount1 << endl;







// expect that the smartParents will be picked more often
EXPECT_TRUE(avgParentCount < smartParentCount);
EXPECT_TRUE(dumbParentCount < smartParentCount);
EXPECT_TRUE(dumbParentCount < avgParentCount);


EXPECT_TRUE(avgParentCount1 < smartParentCount1);
EXPECT_TRUE(dumbParentCount1 < smartParentCount1);
EXPECT_TRUE(dumbParentCount1 < avgParentCount1);
}



//int main() {
//
//    TestRun();
//}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
