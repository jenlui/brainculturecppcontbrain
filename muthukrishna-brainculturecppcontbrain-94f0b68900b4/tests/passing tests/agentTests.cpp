#include "gtest/gtest.h"
#include  "agent.h"
#include "agent.cpp"
#include <vector>
#include <string>

TEST(AgentTest, setFunctionTests) {

    double b = 5.0;
    double il = 0.75;
    double ol = 0.67;
    double lb = 0.43;
    double C = 0.5;

Agent testAgent = Agent(b,il,ol,lb,C);

 testAgent.setBrain(b);
 testAgent.setIndivLearning(il);
 testAgent.setObliqueLearning(ol);
 testAgent.setLearningBias(lb);
 testAgent.setCulture(0.99);

 ASSERT_EQ(b, testAgent.getBrain());
 ASSERT_EQ(il, testAgent.getIndivLearning());
 ASSERT_EQ(ol, testAgent.getObliqueLearning());
 ASSERT_EQ(lb, testAgent.getLearningBias());
 ASSERT_EQ(0.99, testAgent.getCulture());


}

TEST(AgentTest, setFunctionTests_CapCultureAtBrainSize) {

    double b = 5.0;
    double il = 0.75;
    double ol = 0.67;
    double lb = 0.43;
    double C = 0.5;

Agent testAgent = Agent(b,il,ol,lb,C);
testAgent.setCulture(7.0);
ASSERT_EQ(5.0, testAgent.getCulture());
}

TEST(AgentTest, NoParamAgentConstructorWorks_PASS) {

    Agent testAgent = Agent();

    ASSERT_EQ(1, testAgent.getBrain());
    ASSERT_EQ(0, testAgent.getIndivLearning());
    ASSERT_EQ(0, testAgent.getObliqueLearning());
    ASSERT_EQ(0, testAgent.getLearningBias());
    // indirect test that culture is empty
    //ASSERT_EQ(0, testAgent.getTotalCulture());

}

TEST(AgentTest, ConstructAgentWithParameters_AllOkay) {

    double b = 3;
    double il = 1;
    double ol = 1;
    double lb = 1;
    double C = 1;


    Agent testAgent = Agent(b, il, ol, lb, C);
    ASSERT_EQ(3, testAgent.getBrain());
    ASSERT_EQ(1, testAgent.getIndivLearning());
    ASSERT_EQ(1, testAgent.getObliqueLearning());
    ASSERT_EQ(1, testAgent.getLearningBias());

}


TEST(AgentTest, ConstructAgentWithParameters_FAIL_BrainSize) {


    double b = -1;
    double il = 1;
    double ol = 1;
    double lb = 1;
    double initC = 0;

    EXPECT_ANY_THROW( {Agent testAgent = Agent(b, il, ol, lb, initC);} );
}


TEST(AgentTest, ConstructAgentWithParameters_FAIL_IL) {


    double b = 3;
    double il = 20.4;
    double ol = 1;
    double lb = 1;
    double initC = 1;

    EXPECT_ANY_THROW({
                     Agent testAgent = Agent(b, il, ol, lb, initC);
                     });

}


TEST(AgentTest, ConstructAgentWithParameters_FAIL_OL) {

    double b = 3;
    double il = 1;
    double ol = 23;
    double lb = 1;
    double initC = 0.5;

    EXPECT_ANY_THROW({
                       Agent testAgent = Agent(b, il, ol, lb, initC);
                       });

}

/*
//lb has range of -inf to inf

TEST(AgentTest, ConstructAgentWithParameters_FAIL_LB) {

    int b = 3;
    double il = 0.5;
    double ol = 0.9;
    double lb = -1.0;
    double initC = 0.5;
    EXPECT_ANY_THROW( {
                     Agent testAgent = Agent(b,il,ol,lb,initC);
    });

}
*/

TEST(AgentTest, ConstructAgentWithParameters_FAIL_InitC) {
    double b = 3;
    double il = 0.5;
    double ol = 0.9;
    double lb = 1.0;
    double initC = -5;

    EXPECT_ANY_THROW({
                     Agent testAgent = Agent(b,il,ol,lb,initC);
                     };);

}

TEST(AgentTest, std_input_partOne) {
//
//    int b = 3;
//    double il = 0.5;
//    double ol = 0.9;
//    double lb = 1.0;
//    double initC = 1.0;


    Agent testAgent;

    testAgent.std_input("3.0,0.5,0.9,1.0,1.0;");

    ASSERT_EQ(3,testAgent.getBrain());
    ASSERT_EQ(0.5, testAgent.getIndivLearning());
    ASSERT_EQ(0.9, testAgent.getObliqueLearning());
    ASSERT_EQ(1.0, testAgent.getLearningBias());
    ASSERT_EQ(1, testAgent.getCulture());


}


// this test has no double values that can also be written as an integer (for example, "1.0", "2.0")
TEST(AgentTest, std_output_partOne) {

    double b = 3;
    double il = 0.5;
    double ol = 0.9;
    double lb = 1;
    double initC = 0.5;



    Agent testAgent = Agent(b, il, ol, lb, initC);

    ASSERT_EQ("3,0.5,0.9,1,0.5;", testAgent.std_output());


}



TEST(AgentTest, std_input_std_output_TEST) {

    double b = 3;
    double il = 0.5;
    double ol = 0.9;
    double lb = 1.0;
    double initC = 1.0;


    Agent testAgent = Agent();
    testAgent.std_input("3,0.5,0.9,1,1;");



    ASSERT_EQ("3,0.5,0.9,1,1;",testAgent.std_output());


}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
