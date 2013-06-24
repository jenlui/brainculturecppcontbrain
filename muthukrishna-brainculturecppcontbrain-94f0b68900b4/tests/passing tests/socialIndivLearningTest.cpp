#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "agent.h"
#include "agent.cpp"
#include "commonfunctions.cpp"
#include "commonfunctions.h"
#include "main.cpp"
#include <iostream>

TEST(socialIndivLearningTest, createIndivLearnerOK) {

double parentBrain = 1000;
double kidBrain = 900;

double iLearning = 0.0;
double oLearning = 0.0;
double biasLearning = 0.0;
double culture = 1.0;
double lossyLearningValue = 0.9;
double adaptKnowledgeDev = 0.1;
double indivLearningMean = 0.5;

Agent parent = Agent(parentBrain,iLearning, oLearning, biasLearning,culture);
Agent newborn = Agent(kidBrain, iLearning, oLearning, biasLearning,0.5);

double socialLearn = 0.00001;
double before;

if (socialLearn > newborn.getIndivLearning()) {
				//We re-roll the dice to allow some oblique learning
				double shouldILearnFromTeacher = randZeroOne();
				if (shouldILearnFromTeacher < newborn.getObliqueLearning()) {
					/*double totalCommunityCulture =
							getTotalCommunityCultureFromSigmoid(P[p],
									meanCommunityCultureValues[p],
									newborn.getLearningBias());*/
					//Select random teacher
					Agent teacher = parent;
					//Perform imperfect learning, but only update if better than what you have
					double myCulture = getNormalRandom(
							lossyLearningValue * teacher.getCulture(),
							adaptKnowledgeDev);
					if (myCulture > newborn.getCulture()) {
						newborn.setCulture(myCulture);
					}
				} //TODO check that no more learning for social learners who aren't oblique learners

			} else { //Individual learners
				//Perform individual learning, but only update if better than what you have
				before = newborn.getCulture();

				double myCulture = getNormalRandom(
						indivLearningMean*newborn.getBrain(),
						adaptKnowledgeDev);
				if (myCulture > newborn.getCulture()) {
					newborn.setCulture(myCulture);
				}
			}

ASSERT_TRUE(newborn.getCulture() >= before);

}

TEST(socialIndivLearningTest, createSocialLearnerOK) {

double parentBrain = 1000;
double kidBrain = 900;

double iLearning = 0.0;
double oLearning = 1.0;
double biasLearning = 0.0;
double culture = 1.0;
double lossyLearningValue = 0.9;
double adaptKnowledgeDev = 0.1;
double indivLearningMean = 0.5;

Agent parent = Agent(parentBrain,iLearning, oLearning, biasLearning,culture);
Agent newborn = Agent(kidBrain, iLearning, oLearning, biasLearning,0.5);

double socialLearn = 1.0;
double before;

if (socialLearn > newborn.getIndivLearning()) {
				//We re-roll the dice to allow some oblique learning
				double shouldILearnFromTeacher = randZeroOne();
				if (shouldILearnFromTeacher < newborn.getObliqueLearning()) {
					/*double totalCommunityCulture =
							getTotalCommunityCultureFromSigmoid(P[p],
									meanCommunityCultureValues[p],
									newborn.getLearningBias());*/
					//Select random teacher
					Agent teacher = parent;
					//Perform imperfect learning, but only update if better than what you have
					before = newborn.getCulture();
					
					double myCulture = getNormalRandom(
							lossyLearningValue * teacher.getCulture(),
							adaptKnowledgeDev);
					if (myCulture > newborn.getCulture()) {
						newborn.setCulture(myCulture);
					}
				} //TODO check that no more learning for social learners who aren't oblique learners

			} else { //Individual learners
				//Perform individual learning, but only update if better than what you have

				double myCulture = getNormalRandom(
						indivLearningMean*newborn.getBrain(),
						adaptKnowledgeDev);
				if (myCulture > newborn.getCulture()) {
					newborn.setCulture(myCulture);
				}
			}

ASSERT_TRUE(newborn.getCulture() >= before);

}

TEST(socialIndivLearningTest, createSocialLearner_KidCultureIsBigger) {

double parentBrain = 1000;
double kidBrain = 800;

double iLearning = 0.0;
double oLearning = 1.0;
double biasLearning = 0.0;
double culture = 1.0;
double lossyLearningValue = 0.9;
double adaptKnowledgeDev = 0.1;
double indivLearningMean = 0.5;

Agent parent = Agent(parentBrain,iLearning, oLearning, biasLearning,0.5);
Agent newborn = Agent(kidBrain, iLearning, oLearning, biasLearning,1.0);

double socialLearn = 1.0;
double before;

if (socialLearn > newborn.getIndivLearning()) {
				//We re-roll the dice to allow some oblique learning
				double shouldILearnFromTeacher = randZeroOne();
				if (shouldILearnFromTeacher < newborn.getObliqueLearning()) {
					/*double totalCommunityCulture =
							getTotalCommunityCultureFromSigmoid(P[p],
									meanCommunityCultureValues[p],
									newborn.getLearningBias());*/
					//Select random teacher
					Agent teacher = parent;
					//Perform imperfect learning, but only update if better than what you have
					before = newborn.getCulture();
					
					double myCulture = getNormalRandom(
							lossyLearningValue * teacher.getCulture(),
							adaptKnowledgeDev);
					if (myCulture > newborn.getCulture()) {
						newborn.setCulture(myCulture);
					}
				} //TODO check that no more learning for social learners who aren't oblique learners

			} else { //Individual learners
				//Perform individual learning, but only update if better than what you have

				double myCulture = getNormalRandom(
						indivLearningMean*newborn.getBrain(),
						adaptKnowledgeDev);
				if (myCulture > newborn.getCulture()) {
					newborn.setCulture(myCulture);
				}
			}

ASSERT_TRUE(newborn.getCulture() == before);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

