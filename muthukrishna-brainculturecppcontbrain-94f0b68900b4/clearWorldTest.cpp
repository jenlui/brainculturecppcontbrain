#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

TEST(clearWorldTest, clearWorld_OK) {

int b = 2;
double il = 0.5;
double ol = 0.5;
double lb = 0.5;
double initC = 0.5;

//initialize world vector
std::vector<std::vector<Agent> > world;

// initialize subpopulation vector
std::vector<Agent> subpop1;
std::vector<Agent> subpop2;

// create agents to put in subpop
Agent testAgent1 = Agent(b, il, ol, lb, initC);
Agent testAgent2 = Agent(b, il, ol, lb, initC);
Agent testAgent3 = Agent(b, il, ol, lb, initC);

// insert agents into subpop vectors
subpop1.push_back(testAgent1);
subpop1.push_back(testAgent2);
subpop1.push_back(testAgent3);
subpop2.push_back(testAgent1);
subpop2.push_back(testAgent2);
subpop2.push_back(testAgent3);

//insert subpops into world
world.push_back(subpop1);
world.push_back(subpop2);

clearWorld(world);
ASSERT_TRUE(world[0].empty());
ASSERT_TRUE(world[1].empty());
//ASSERT_TRUE(world.empty());
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

