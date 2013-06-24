#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"


TEST(getUniformRandomTest, getUniformRandom_OK_INT) {
         int min = 5;
         int max = 10;
         int randomNum = getUniformRandom(5,10);
         ASSERT_TRUE(randomNum>=5 && randomNum<=10);
}

TEST(getUniformRandomTest, getUniformRandom_OK_REAL) {
        double dmin = 5.0;
        double dmax = 10.0;
        double randomNum = getUniformRandom(5.0,10.0);
        ASSERT_TRUE(randomNum>=5.0 && randomNum<=10.0);
}

TEST(getUniformRandomTest, getUniformRandom_FAIL_INT) {
        int min = 5;
        int max = 10;
        int randomNum = getUniformRandom(5,10);
        ASSERT_FALSE(randomNum<5 || randomNum>10);
}

TEST(getUniformRandomTest, getUniformRandom_FAIL_REAL) {
       double dmin = 5.0;
       double dmax = 10.0;
       double randomNum = getUniformRandom(5.0,10.0);
       ASSERT_FALSE(randomNum<5.0 || randomNum>10.0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
