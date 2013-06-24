#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>

#include <stdio.h>
#include <stdlib.h>
#include "commonfunctions.h"
#include "agent.h"
#include <boost/random.hpp>
#include <boost/math/distributions/binomial.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <omp.h>
#include <assert.h>
#include <sys/stat.h>

//Changed to link brain mean to brain sd.

using namespace std;

#define MAX_GENERATIONS 1000000
#define RECORD_EVERY 100 //Every how many generations should be recorded?
typedef boost::minstd_rand base_generator_type;
base_generator_type gen(time(0));
boost::uniform_real<> zeroOneDist(0, 1);
boost::variate_generator<base_generator_type&, boost::uniform_real<> > randZeroOne(
		gen, zeroOneDist);
std::random_device rd;
	  std::mt19937 genmt(rd());




template<typename N> N getUniformRandom(N min, N max) {
	if (boost::is_integral<N>::value) {
		boost::uniform_int<> dist(min, max);
		boost::variate_generator<base_generator_type&, boost::uniform_int<> > random(
				gen, dist);
		return random();
	} else {
		boost::uniform_real<> dist(min, max);
		boost::variate_generator<base_generator_type&, boost::uniform_real<> > random(
				gen, dist);
		return random();
	}
}

/*template<typename N> N getBinomRandom(int trials, double prob) {
	boost::math::binomial_distribution<int, double> dist(trials, prob);
	boost::variate_generator<base_generator_type&, boost::math::binomial_distribution<int, double> > random(
			gen, dist);
	return random();
}*/

template<typename N> N getNormalRandom(N mean, N sd) {
	//gen.seed(static_cast<unsigned int>(time(0)));
	boost::normal_distribution<> dist(mean, sd);
	boost::variate_generator<base_generator_type&, boost::normal_distribution<> > random(
			gen, dist);
	return random();

}

void recordPopulation(vector<vector<Agent> >& P, ostringstream& dataOutput) {
	vector<vector<Agent> >::iterator subpop_it = P.begin();
	vector<vector<Agent> >::iterator subpop_end = P.end();
	for (; subpop_it != subpop_end; ++subpop_it) {
		vector<Agent>::iterator a_it = subpop_it->begin();
		vector<Agent>::iterator a_end = subpop_it->end();
		for (; a_it != a_end; ++a_it) {
			string temp = a_it->std_output();
			dataOutput << temp;
			dataOutput << "+"; //Char to split up agents
		}
		dataOutput << endl;
	}

}

void clearWorld(vector<vector<Agent> >& world) {
	int worldSize = world.size();
	for (int i = 0; i < worldSize; i++) {
		world[i].clear();
	}
}

void saveWorldParameters(string& outputFolder, string& code,
		int& minGenerations, int& numSubpopulations, int& minPopsize,
		double& fecundityCurve, double& growthRateR, double& brainMutationRate,
		double& brainMutationDev, double& adaptKnowledgeDev,
		double& obliqueLearningDev, double& learningBiasDev,
		double indivLearningDev, double indivLearningMean,
		double& migrationRate, double& brainCost, double& deathRateLambda,
		double& initBrain, double& initAdaptKnowledge, double initIndivLearning,
		double& initObliqueLearn, double& initLearningBias,
		double& initPopSize) {
	string worldFileName = outputFolder + "/World" + code + ".txt";
	ostringstream worldOutput; //Save you the hassle of converting everything
	ofstream txtFile;
	txtFile.open(worldFileName.c_str());
	worldOutput << "#General Parameters" << endl;
	worldOutput << "MinGenerations=" << minGenerations << endl;
	worldOutput << "MaxGenerations=" << MAX_GENERATIONS << endl;
	worldOutput << "NumSubpopulations=" << numSubpopulations << endl;
	worldOutput << "#Birth Parameters" << endl;
	worldOutput << "MinimumPopsize=" << minPopsize << endl;
	worldOutput << "FecundityCurvature=" << fecundityCurve << endl;
	worldOutput << "GrowthRateR=" << growthRateR << endl;
	worldOutput << "BrainMutationRate=" << brainMutationRate << endl;
	worldOutput << "brainMutationDev=" << brainMutationDev << endl;
	worldOutput << "#Learning Parameters" << endl;
	worldOutput << "AdaptiveKnowledgeDeviation=" << adaptKnowledgeDev << endl;
	worldOutput << "LossyLearningValue=" << adaptKnowledgeDev << endl;
	worldOutput << "ObliqueLearningDeviation=" << obliqueLearningDev << endl;
	worldOutput << "LearningBiasDeviation=" << learningBiasDev << endl;
	worldOutput << "IndivLearningDeviation=" << indivLearningDev << endl;
	worldOutput << "IndivLearningMean=" << indivLearningMean << endl;
	worldOutput << "#Migration Parameters" << endl;
	worldOutput << "MigrationRate=" << migrationRate << endl;
	worldOutput << "#Selection Parameters" << endl;
	worldOutput << "MaxBrainCost=" << brainCost << endl;
	worldOutput << "DeathRateLambda=" << deathRateLambda << endl;
	worldOutput << "#Initial Values" << endl;
	worldOutput << "InitBrain=" << initBrain << endl;
	worldOutput << "InitAdaptKnowledge=" << initAdaptKnowledge << endl;
	worldOutput << "InitIndivLearning=" << initIndivLearning << endl;
	worldOutput << "InitObliqueLearn=" << initObliqueLearn << endl;
	worldOutput << "InitLearningBias=" << initLearningBias << endl;
	worldOutput << "InitPopSize=" << initPopSize << endl;
	txtFile << worldOutput.str();
	txtFile.close();
}

double mutateGeneticTrait(double geneticMutationRate, double deviation,
		double parentTraitValue, bool allowNegative, bool gt1) {
	//Assume individual learning bias is a genetic trait from the parent
	double trait = -1;
	double rand = randZeroOne();
	if (rand < geneticMutationRate) {
		trait = getNormalRandom(parentTraitValue, deviation);
		if (!gt1) {
			trait = min(trait, 1.0);
		}
		if (!allowNegative) {
			trait = max(0.0, trait);
		}
	} else {
		trait = parentTraitValue;
	}
	return trait;
}

Agent selectAgentBasedOnCulture(vector<vector<Agent> >& population,
		int subpopNum, double meanCommunityCulture, double curvature,
		double totalCommunityCulture) {

	Agent agent = Agent();
	double randAgentValue = getUniformRandom(0.0, totalCommunityCulture);
	double cumulative = 0;
	int k = -1;
	while (cumulative <= randAgentValue) {
		k++;
		// insert condition for when k is over the number of agents in the subpop?
		agent = population.at(subpopNum).at(k);
		cumulative += sigmoid(agent.getCulture(), meanCommunityCulture,
				curvature);
	}
	return agent;
}

double mutateGeneticTrait(double geneticMutationRate, double deviation,
		double parentTraitValue) {
	return mutateGeneticTrait(geneticMutationRate, deviation, parentTraitValue,
			false, false);
}

Agent learnAdaptiveKnowledge(Agent newborn, Agent parent,
		double lossyLearningValue, double adaptKnowledgeDev,
		double indivLearningMean) {

	double socialLearn = randZeroOne();
	if (socialLearn > newborn.getIndivLearning()) {
		//Learn from your parent
		double mean = lossyLearningValue * parent.getCulture();
		double std = adaptKnowledgeDev*mean;
		newborn.setCulture(
				getNormalRandom(mean,
						std));
	} else {
		//Learn by yourself
		double mean = indivLearningMean * newborn.getBrain();
		double std = adaptKnowledgeDev*mean;
		newborn.setCulture(
				getNormalRandom(mean,
						std));
	}

	return newborn;
}

void initializePopulation(int numSubpopulations, int initPopSize,
		double initBrain, double initIndivLearning, double initObliqueLearn,
		double initLearningBias, double initAdaptKnowledge,
		vector<vector<Agent> >& P) {
	/* check parameters */
	if (numSubpopulations < 0) {
		throw "negative subpopulation!";
	}
	if (initPopSize < 0) {
		throw "initial population was negative!";
	}
	if (initBrain < 0) {
		throw "brain size negative!";
	}
	if (initIndivLearning < 0) {
		throw "initIndivLearning negative!";
	}
	if (initObliqueLearn < 0) {
		throw "initObliqueLearn negative!";
	}
	if (initAdaptKnowledge < 0) {
		throw "initAdaptKnowledge negative!";
	}
	if (initAdaptKnowledge > initBrain) {
		throw "initAdaptKnowledge is bigger than initBrain";
	}

	/*Set up initial population*/
	#pragma omp parallel for
	for (int p = 0; p < numSubpopulations; p++) {
		vector<Agent> subpop;
		for (int a = 0; a < initPopSize; a++) {
			subpop.push_back(
					Agent(initBrain, initIndivLearning, initObliqueLearn,
							initLearningBias, initAdaptKnowledge));
		}
		P[p] = subpop;
	}
}

void birthStage(int numSubpopulations, vector<vector<Agent> >& P,
		vector<double>& meanCommunityCultureValues, double growthRateR,
		int minPopsize, double fecundityCurve, double geneticMutationRate,
		double brainMutationDev, double indivLearningDev,
		double obliqueLearningDev, double learningBiasDev,
		double lossyLearningValue, double adaptKnowledgeDev,
		double indivLearningMean, vector<vector<Agent> >& NP) {
	#pragma omp parallel for
	for (int p = 0; p < numSubpopulations; p++) {
		vector<Agent> subpop;
		if (P[p].size() > 0) {
			double meanCommunityCulture = getMeanCommunityCulture(P[p]);
			meanCommunityCultureValues[p] = meanCommunityCulture;
			int numKids = numChildren(P[p].size(), growthRateR,
					meanCommunityCulture, minPopsize);
			//Each child is randomly assigned a genetic parent with a bias toward those with more culture
			double totalCommunityCulture = getTotalCommunityCultureFromSigmoid(
					P[p], meanCommunityCulture, fecundityCurve);

			//Calculate number of kids for each parent
			for (int i = 0; i < P[p].size(); i++) {
				Agent parent = P[p][i];
				double sigmoidCulture = sigmoid(parent.getCulture(), meanCommunityCulture,
						fecundityCurve);
				double prob = sigmoidCulture/totalCommunityCulture;
				std::binomial_distribution<> d(numKids, prob);
				int myKids = d(genmt);

				for (int kid = 0; kid < myKids; kid++) {
					double brain = mutateGeneticTrait(geneticMutationRate,
							brainMutationDev*parent.getBrain(), parent.getBrain(), false, true);
					//Assume these are genetic traits from the parent
					double iLearning = mutateGeneticTrait(geneticMutationRate,
							indivLearningDev, parent.getIndivLearning());
					double oLearning = mutateGeneticTrait(geneticMutationRate,
							obliqueLearningDev, parent.getObliqueLearning());
					double biasLearning = mutateGeneticTrait(
							geneticMutationRate, learningBiasDev,
							parent.getLearningBias(), true, true);
					Agent newborn = Agent(brain, iLearning, oLearning,
							biasLearning, 0.0);
					newborn.setParentCulture(parent.getCulture());
					//Learn adaptive knowledge (Culture)
					Agent educatedNewborn = learnAdaptiveKnowledge(newborn,
							parent, lossyLearningValue, adaptKnowledgeDev,
							indivLearningMean);
					subpop.push_back(educatedNewborn);
				}
			}
		}

		NP[p] = subpop;
	}

}

void learningStage(int numSubpopulations, vector<vector<Agent> >& NP,
		vector<vector<Agent> >& P, vector<double> meanCommunityCultureValues,
		double lossyLearningValue, double adaptKnowledgeDev,
		double indivLearningMean) {
	//Learn from imperfectly from random parent (teacher?) with replacement
	//LearningBias determines how good you are at picking a good cultural parent
	#pragma omp parallel for
	for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < NP[p].size(); a++) {
			double socialLearn = randZeroOne();
			if (socialLearn > NP[p][a].getIndivLearning()) {
				//We re-roll the dice the allow some oblique learning
				double shouldILearnFromTeacher = randZeroOne();
				if (shouldILearnFromTeacher < NP[p][a].getObliqueLearning()) {
					double totalCommunityCulture =
							getTotalCommunityCultureFromSigmoid(P[p],
									meanCommunityCultureValues[p],
									NP[p][a].getLearningBias());
					//Select random teacher
					Agent teacher = selectAgentBasedOnCulture(P, p,
							meanCommunityCultureValues[p],
							NP[p][a].getLearningBias(), totalCommunityCulture);
					//Perform imperfect learning, but only update if better than what you have
					double mean = lossyLearningValue * teacher.getCulture();
					double std = adaptKnowledgeDev*mean;
					double myCulture = getNormalRandom(mean,std);
					if (myCulture > NP[p][a].getCulture()) {
						NP[p][a].setCulture(myCulture);
					}
				} else {
					//Learn from parent
					double mean = lossyLearningValue * NP[p][a].getParentCulture();
					double std = adaptKnowledgeDev*mean;
					double myCulture = getNormalRandom(mean,std);
					if (myCulture > NP[p][a].getCulture()) {
						NP[p][a].setCulture(myCulture);
					}
				}

			} else { //Individual learners
				//Perform individual learning, but only update if better than what you have
				double mean = indivLearningMean * NP[p][a].getBrain();
				double std = adaptKnowledgeDev*mean;
				double myCulture = getNormalRandom(mean,std);
				if (myCulture > NP[p][a].getCulture()) {
					NP[p][a].setCulture(myCulture);
				}
			}
		}
	}
}

void migrateStage(vector<vector<Agent> >& P, vector<vector<Agent> >& NP,
		int numSubpopulations, double migrationRate) {
	/*Replace parents with kids*/
	//clearWorld(P);
	//P.clear();
	P = NP; //Joke intended
	clearWorld(NP); //An empty world to migrate to
	for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < P[p].size(); a++) {
			int newHome;
			double shouldIMigrate = randZeroOne();
			if (shouldIMigrate < migrationRate) {
				newHome = getUniformRandom(0, numSubpopulations - 1);
				//Make sure its different to current home
				while (newHome == p) {
					newHome = getUniformRandom(0, numSubpopulations - 1);
				}
			} else {
				newHome = p; //New home = old home
			}
			NP[newHome].push_back(P[p][a]);
		}

	}

}

void selectionStage(vector<vector<Agent> >& P, vector<vector<Agent> >& NP,
		int numSubpopulations, double deathRateLambda, double brainCost) {
	/*Replace old world with new migrated world */
	//clearWorld(P);
	//P.clear();
	P = NP;
	clearWorld(NP); //An empty world for the survivors
	#pragma omp parallel for
	for (int p = 0; p < numSubpopulations; p++) {
		for (int a = 0; a < P[p].size(); a++) {
			double percentChanceDying = deathRate(deathRateLambda,
					P[p][a].getBrain(), P[p][a].getBrain(),
					P[p][a].getCulture(), brainCost);
			double shouldILive = randZeroOne();
			if (shouldILive > percentChanceDying) {
				//I live!
				NP[p].push_back(P[p][a]);
			}
		}

	}

}

void checkForEquilibria(int generationCount, vector<vector<Agent> >& P,
		double& oldBrainMean, double& oldCultureMean, double& oldILMean,
		double& oldOLMean, double& oldLBMean, int& equilibriaCount) {
	/*6.CHECK FOR EQUILIBRIA*/
	//Checks every 400 generations
	if (generationCount % 400 == 0) {
		vector<double> popParam = getMeanPopulationParameters(P);
		double currentBrainMean = popParam[0];
		double currentCultureMean = popParam[1];
		double currentILearningMean = popParam[2];
		double currentOLearningMean = popParam[3];
		double currentLearningBiasMean = popParam[4];
		double threshold = 0.1;
		if ((fabs(currentBrainMean - oldBrainMean) > threshold)
				|| (fabs(currentCultureMean - oldCultureMean) > threshold)
				|| (fabs(currentILearningMean - oldILMean) > threshold)
				|| (fabs(currentOLearningMean - oldOLMean) > threshold)
				|| (fabs(currentLearningBiasMean - oldLBMean) > threshold)) {
			equilibriaCount = 0;
		}
		oldBrainMean = currentCultureMean;
		oldCultureMean = currentCultureMean;
		oldILMean = currentILearningMean;
		oldOLMean = currentOLearningMean;
		oldLBMean = currentLearningBiasMean;
	}

}

void simulation(string code, string outputFolder, int minGenerations,
		int numSubpopulations, int minPopsize, double fecundityCurve,
		double growthRateR, double geneticMutationRate, double brainMutationDev,
		double adaptKnowledgeDev, double lossyLearningValue,
		double obliqueLearningDev, double learningBiasDev,
		double indivLearningDev, double indivLearningMean, double migrationRate,
		double brainCost, double deathRateLambda, double initBrain,
		double initAdaptKnowledge, double initIndivLearning,
		double initObliqueLearn, double initLearningBias, double initPopSize) {


	//Select random parent
	//gen.seed(static_cast<unsigned int>(time(0)));

	cout << "Create new world: " + code << endl;
	saveWorldParameters(outputFolder, code, minGenerations, numSubpopulations,
			minPopsize, fecundityCurve, growthRateR, geneticMutationRate,
			brainMutationDev, adaptKnowledgeDev, obliqueLearningDev,
			learningBiasDev, indivLearningDev, indivLearningMean, migrationRate,
			brainCost, deathRateLambda, initBrain, initAdaptKnowledge,
			initIndivLearning, initObliqueLearn, initLearningBias, initPopSize);

	vector<vector<Agent> > P(numSubpopulations); //Population vector of subpopulations, which are vectors of agents

	/*Set up initial population*/
	initializePopulation(numSubpopulations, initPopSize, initBrain,
			initIndivLearning, initObliqueLearn, initLearningBias,
			initAdaptKnowledge, P);

	/*Record initial population*/
	string dataFileName = outputFolder + "/brainCultureData" + code + ".csv";
	ofstream csvFile;
	csvFile.open(dataFileName.c_str());
	ostringstream dataOutput;
	dataOutput << "Generation,0" << endl;
	recordPopulation(P, dataOutput);

	/*At each time go through the following sequence: (1) BIRTH (including DEATH of
	 PARENTS); (2) LEARNING; (3) MIGRATION; (4) SELECTION*/
	int generationCount = 0;
	/*int equilibriaCount = 0;*/ //Number of time steps we've been at equilibria
//	double oldCultureMean = initAdaptKnowledge;
//	double oldBrainMean = initBrain;
//	double oldILMean = initIndivLearning;
//	double oldOLMean = initObliqueLearn;
//	double oldLBMean = initLearningBias;
//	int extinctCount = 0;
	while (generationCount < MAX_GENERATIONS) {
		generationCount++;
		/*equilibriaCount++;*/
		//cout << code << ":Generation:" << generationCount << endl;
		if (generationCount % RECORD_EVERY == 0) {
			dataOutput << "Generation," << generationCount << endl;
		}

		/*1.BIRTH*/
		//cout << code << ":BIRTH" << endl;
		vector<vector<Agent> > NP(numSubpopulations); //New population of kids
		vector<double> meanCommunityCultureValues(numSubpopulations);
		birthStage(numSubpopulations, P, meanCommunityCultureValues,
				growthRateR, minPopsize, fecundityCurve, geneticMutationRate,
				brainMutationDev, indivLearningDev, obliqueLearningDev,
				learningBiasDev, lossyLearningValue, adaptKnowledgeDev,
				indivLearningMean, NP);

		/*2.LEARNING*/
		//cout << code << ":LEARNING" << endl;
		//Learn from imperfectly from random parent with replacement
		//LearningBias determines how good you are at picking a good cultural parent
		learningStage(numSubpopulations, NP, P, meanCommunityCultureValues,
				lossyLearningValue, adaptKnowledgeDev, indivLearningMean);

		/*3.MIGRATION*/
		//cout << code << ":MIGRATION" << endl;
		/*Replace parents with kids*/
		migrateStage(P, NP, numSubpopulations, migrationRate);

		/*4.SELECTION*/
		//cout << code << ":SELECTION" << endl;
		/*Replace old world with new migrated world */
		//clearWorld(P);
		//P.clear();
		selectionStage(P, NP, numSubpopulations, deathRateLambda, brainCost);

		/*Replace old world with world of survivors */
		//P.clear();
		P = NP;
		//clearWorld(NP);
		//NP.clear();

		/*5.RECORD DATA*/
		if (generationCount % RECORD_EVERY == 0) {
			recordPopulation(P, dataOutput);
		}

		/*6.CHECK FOR EQUILIBRIA*/
		//Checks every 400 generations
		/*checkForEquilibria(generationCount, P, oldBrainMean, oldCultureMean,
				oldILMean, oldOLMean, oldLBMean, equilibriaCount);
				*/
	}

	csvFile << dataOutput.str();
	csvFile.close();
}

int main(int argc, char* argv[]) {
	//General Parameters
	int minGenerations = 1000;
	int numSubpopulations = 50;

	//Birth Parameters
	int minPopsize = 10; //Maximum popsize when no adaptive knowledge
	double fecundityCurvature = 10;
	double growthRateR = 0.04 * 20; //should vary between 0.005 and 0.04 multiplied by generation length
	double geneticMutationRate = 0.005;
	double brainMutationDev = .1;

	//Learning Parameters
	double adaptKnowDeviation = 0.1;
	double lossyLearningValue = .9; //0 = Complete loss, 1 = No loss
	double obliqueLearningDeviation = 0.1;
	double learningBiasDeviation = 0.5;
	double indivLearningDeviation = 0.1;
	double indivLearningMean = 0.5;

	//Migration Parameters
	double migrationRate = 0.1;

	//Selection Parameters
	double maxBrainCost = 100;
	double deathRateLambda = 1.0;

	//Initial values
	double initBrain = 1.0;
	double initAdaptKnowledge = 0.0;
	double initIndivLearning = 1.0; //0 = complete social learner, 1 = complete individual learner
	double initObliqueLearning = 0.0;
	double initLearningBias = 0.0;
	double initPopSize = 10;

	//omp_set_num_threads(12);
	string code = "DebugRun";
	string outputFolder;

	//fecundityCurvature = atof(argv[1]);
	string outputName = argv[1];
	int iteration = atof(argv[2]);


	//Code for grabbing arguments from commandline
	outputFolder = string("./data/ForFinalPaper/") + outputName;
	mkdir(outputFolder.c_str(), 0777); //creating a directory
	cout << outputFolder << endl;
	fecundityCurvature = atof(argv[3]);
	lossyLearningValue = atof(argv[4]);
	indivLearningMean = atof(argv[5]);
	migrationRate = atof(argv[6]);
	deathRateLambda = atof(argv[7]);
	ostringstream codestream;
	codestream << "iteration=" << iteration << "fecundCurve="
			<< fecundityCurvature << "lossyLearning=" << lossyLearningValue
			<< "maxBrainCost=" << maxBrainCost << "indivLearningMean="
			<< indivLearningMean << "migrationRate=" << migrationRate
			<< "deathRateLambda=" << deathRateLambda;
	code = codestream.str();
	simulation(code, outputFolder, minGenerations, numSubpopulations,
			minPopsize, fecundityCurvature, growthRateR, geneticMutationRate,
			brainMutationDev, adaptKnowDeviation, lossyLearningValue,
			obliqueLearningDeviation, learningBiasDeviation,
			indivLearningDeviation, indivLearningMean, migrationRate,
			maxBrainCost, deathRateLambda, initBrain, initAdaptKnowledge,
			initIndivLearning, initObliqueLearning, initLearningBias,
			initPopSize);
	//End code

	//int iteration = 6;
	//initIndivLearning = 0;
	//Non-extinct Range
//	outputFolder = string("./data/ForFinalPaper/") + "NonExtinct";
//	mkdir(outputFolder.c_str(), 0777); //creating a directory
//
//#pragma omp for collapse(5)
//for (int fc = 0; fc <= 10; fc++) {
//		for (int ll = 90; ll <= 100; ll+= 5) {
//			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
//			for (int ilm = 1; ilm <= 6; ilm++) {
//				for (int mr = 10; mr <= 20; mr +=
//						5) {
//					for (int drl = 0; drl <= 4;
//							drl += 1) {
//						fecundityCurvature = (double)fc/10.0;
//						lossyLearningValue = (double)ll/100.0;
//						indivLearningMean = (double)ilm/10.0;
//						migrationRate = (double)mr/100.0;
//						deathRateLambda = drl;
//						//for (int iteration = 0; iteration < runs; iteration++) {
//						ostringstream codestream;
//						codestream << "iteration=" << iteration
//								<< "fecundCurve=" << fecundityCurvature
//								<< "lossyLearning=" << lossyLearningValue
//								<< "maxBrainCost=" << maxBrainCost
//								<< "indivLearningMean=" << indivLearningMean
//								<< "migrationRate=" << migrationRate
//								<< "deathRateLambda=" << deathRateLambda;
//						code = codestream.str();
//						simulation(code, outputFolder, minGenerations,
//								numSubpopulations, minPopsize,
//								fecundityCurvature, growthRateR,
//								geneticMutationRate, brainMutationDev,
//								adaptKnowDeviation, lossyLearningValue,
//								obliqueLearningDeviation, learningBiasDeviation,
//								indivLearningDeviation, indivLearningMean,
//								migrationRate, maxBrainCost, deathRateLambda,
//								initBrain, initAdaptKnowledge,
//								initIndivLearning, initObliqueLearning,
//								initLearningBias, initPopSize);
//						}
//					}
//				//}
//			}
//		}
//	}
//
//	outputFolder = string("./data/ForFinalPaper/") + "StartSocialNonExtinct";
//	mkdir(outputFolder.c_str(), 0777); //creating a directory
//	initIndivLearning = 0;
//#pragma omp for collapse(5)
//	for (int fc = 6; fc <= 6;
//			fc++) {
//		for (int ll = 100; ll <= 100;
//				ll += 5) {
//			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
//			for (int ilm = 3; ilm <= 4;
//					ilm += 1) {
//				for (int mr = 10; mr <= 10; mr +=
//						5) {
//					for (int drl = 3; drl <= 4;
//							drl += 1) {
//						fecundityCurvature = (double)fc/10.0;
//												lossyLearningValue = (double)ll/100.0;
//												indivLearningMean = (double)ilm/10.0;
//												migrationRate = (double)mr/100.0;
//												deathRateLambda = drl;
//						//for (int iteration = 0; iteration < runs; iteration++) {
//						ostringstream codestream;
//						codestream << "iteration=" << iteration
//								<< "fecundCurve=" << fecundityCurvature
//								<< "lossyLearning=" << lossyLearningValue
//								<< "maxBrainCost=" << maxBrainCost
//								<< "indivLearningMean=" << indivLearningMean
//								<< "migrationRate=" << migrationRate
//								<< "deathRateLambda=" << deathRateLambda;
//						code = codestream.str();
//						simulation(code, outputFolder, minGenerations,
//								numSubpopulations, minPopsize,
//								fecundityCurvature, growthRateR,
//								geneticMutationRate, brainMutationDev,
//								adaptKnowDeviation, lossyLearningValue,
//								obliqueLearningDeviation, learningBiasDeviation,
//								indivLearningDeviation, indivLearningMean,
//								migrationRate, maxBrainCost, deathRateLambda,
//								initBrain, initAdaptKnowledge,
//								initIndivLearning, initObliqueLearning,
//								initLearningBias, initPopSize);
//						//}
//					}
//				}
//			}
//		}
//	}

//	//Fecund Range
//	outputFolder = string("./data/ForFinalPaper/") + "FecundRangeContTest";
//	mkdir(outputFolder.c_str(),0777);//creating a directory
//	for (fecundityCurvature = 0; fecundityCurvature <= 1; fecundityCurvature += .1) {
//		for (lossyLearningValue = 0.9; lossyLearningValue <= .9; lossyLearningValue += 0.1) {
//			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
//			for (indivLearningMean = 0.2; indivLearningMean <= .6; indivLearningMean +=0.1) {
//				for (migrationRate = .1; migrationRate <= .1; migrationRate+=0.1) {
//					for (deathRateLambda = 0; deathRateLambda <=4; deathRateLambda += 1) {
//						//for (int iteration = 0; iteration < runs; iteration++) {
//						ostringstream codestream;
//						codestream << "iteration=" << iteration << "fecundCurve=" << fecundityCurvature << "lossyLearning=" << lossyLearningValue
//								<< "maxBrainCost=" << maxBrainCost << "indivLearningMean=" << indivLearningMean
//								<< "migrationRate=" << migrationRate << "deathRateLambda=" << deathRateLambda;
//						code = codestream.str();
//						simulation(code, outputFolder,
//								minGenerations, numSubpopulations,
//								minPopsize, fecundityCurvature, growthRateR, geneticMutationRate, brainMutationDev,
//								adaptKnowDeviation, lossyLearningValue, obliqueLearningDeviation, learningBiasDeviation,
//								indivLearningDeviation, indivLearningMean,
//								migrationRate,
//								maxBrainCost, deathRateLambda,
//								initBrain, initAdaptKnowledge, initIndivLearning,
//								initObliqueLearning, initLearningBias, initPopSize);
//						//}
//				}
//				}
//			}
//		}
//	}
//	//}

//	//Learning Efficiency
//	outputFolder = string("./data/ForFinalPaper/") + "LearningEfficiencyCont2";
//	mkdir(outputFolder.c_str(),0777);//creating a directory
//	for (fecundityCurvature = 0; fecundityCurvature <= 1; fecundityCurvature += .2) {
//		for (lossyLearningValue = 0.6; lossyLearningValue <= 1; lossyLearningValue += 0.1) {
//			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
//			for (indivLearningMean = 0.1; indivLearningMean <= .7; indivLearningMean +=0.1) {
//				for (migrationRate = .1; migrationRate <= .1; migrationRate+=0.1) {
//					for (deathRateLambda = 0; deathRateLambda <=4; deathRateLambda += 1) {
//						//for (int iteration = 0; iteration < runs; iteration++) {
//							ostringstream codestream;
//							codestream << "iteration=" << iteration << "fecundCurve=" << fecundityCurvature << "lossyLearning=" << lossyLearningValue
//									<< "maxBrainCost=" << maxBrainCost << "indivLearningMean=" << indivLearningMean
//									<< "migrationRate=" << migrationRate << "deathRateLambda=" << deathRateLambda;
//							code = codestream.str();
//							simulation(code, outputFolder,
//									minGenerations, numSubpopulations,
//									minPopsize, fecundityCurvature, growthRateR, geneticMutationRate, brainMutationDev,
//									adaptKnowDeviation, lossyLearningValue, obliqueLearningDeviation, learningBiasDeviation,
//									indivLearningDeviation, indivLearningMean,
//									migrationRate,
//									maxBrainCost, deathRateLambda,
//									initBrain, initAdaptKnowledge, initIndivLearning,
//									initObliqueLearning, initLearningBias, initPopSize);
//							//}
//				}
//				}
//			}
//		}
//	}
//	//}
//	//Death Rate Lambda
//	outputFolder = string("./data/ForFinalPaper/") + "DeathRateLambdaCont2";
//	mkdir(outputFolder.c_str(),0777);//creating a directory
//	for (fecundityCurvature = 0; fecundityCurvature <= 1; fecundityCurvature+=.2) {
//		for (lossyLearningValue = 0.9; lossyLearningValue <= .9; lossyLearningValue += 0.05) {
//			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
//			for (indivLearningMean = 0.1; indivLearningMean <= .7; indivLearningMean +=0.1) {
//				for (migrationRate = .1; migrationRate <= .1; migrationRate+=0.1) {
//					for (deathRateLambda = 0; deathRateLambda <=4; deathRateLambda += .5) {
//						//for (int iteration = 0; iteration < runs; iteration++) {
//						ostringstream codestream;
//						codestream << "iteration=" << iteration << "fecundCurve=" << fecundityCurvature << "lossyLearning=" << lossyLearningValue
//								<< "maxBrainCost=" << maxBrainCost << "indivLearningMean=" << indivLearningMean
//								<< "migrationRate=" << migrationRate << "deathRateLambda=" << deathRateLambda;
//						code = codestream.str();
//						simulation(code, outputFolder,
//								minGenerations, numSubpopulations,
//								minPopsize, fecundityCurvature, growthRateR, geneticMutationRate, brainMutationDev,
//								adaptKnowDeviation, lossyLearningValue, obliqueLearningDeviation, learningBiasDeviation,
//								indivLearningDeviation, indivLearningMean,
//								migrationRate,
//								maxBrainCost, deathRateLambda,
//								initBrain, initAdaptKnowledge, initIndivLearning,
//								initObliqueLearning, initLearningBias, initPopSize);
//						//}
//				}
//				}
//			}
//		}
//	}
//	//}
//
//	//Migration
////	outputFolder = string("./data/ForFinalPaper/") + "Migration100Cont2";
////	mkdir(outputFolder.c_str(),0777);//creating a directory
////	for (fecundityCurvature = 0; fecundityCurvature <= 1; fecundityCurvature++) {
////		for (lossyLearningValue = 0.9; lossyLearningValue <= .9; lossyLearningValue += 0.05) {
////			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
////			for (indivLearningMean = 0.1; indivLearningMean <= .7; indivLearningMean +=0.1) {
////				for (migrationRate = 0; migrationRate <= .5; migrationRate+=0.05) {
////					for (deathRateLambda = 3; deathRateLambda <=4; deathRateLambda += 1) {
////						//for (int iteration = 0; iteration < runs; iteration++) {
////						ostringstream codestream;
////						codestream << "iteration=" << iteration << "fecundCurve=" << fecundityCurvature << "lossyLearning=" << lossyLearningValue
////								<< "maxBrainCost=" << maxBrainCost << "indivLearningMean=" << indivLearningMean
////								<< "migrationRate=" << migrationRate << "deathRateLambda=" << deathRateLambda;
////						code = codestream.str();
////						simulation(code, outputFolder,
////								minGenerations, numSubpopulations,
////								minPopsize, fecundityCurvature, growthRateR, geneticMutationRate, brainMutationDev,
////								adaptKnowDeviation, lossyLearningValue, obliqueLearningDeviation, learningBiasDeviation,
////								indivLearningDeviation, indivLearningMean,
////								migrationRate,
////								maxBrainCost, deathRateLambda,
////								initBrain, initAdaptKnowledge, initIndivLearning,
////								initObliqueLearning, initLearningBias, initPopSize);
////						//}
////				}
////				}
////			}
////		}
////	}
//	//}*/
//
////	outputFolder = string("./data/ForFinalPaper/") + "StartSocial2";
////	mkdir(outputFolder.c_str(),0777);//creating a directory
////	initIndivLearning = 0;
////	for (fecundityCurvature = 1; fecundityCurvature <= 1; fecundityCurvature++) {
////		for (lossyLearningValue = 0.9; lossyLearningValue <= .9; lossyLearningValue += 0.05) {
////			//for (maxBrainCost = 100; maxBrainCost <= 100; maxBrainCost +=40) {
////			for (indivLearningMean = 0.2; indivLearningMean <= .2; indivLearningMean +=0.1) {
////				for (migrationRate = 0.1; migrationRate <= .1; migrationRate+=0.05) {
////					for (deathRateLambda = 3; deathRateLambda <=3; deathRateLambda += 1) {
////						//for (int iteration = 0; iteration < runs; iteration++) {
////							ostringstream codestream;
////							codestream << "iteration=" << iteration << "fecundCurve=" << fecundityCurvature << "lossyLearning=" << lossyLearningValue
////									<< "maxBrainCost=" << maxBrainCost << "indivLearningMean=" << indivLearningMean
////									<< "migrationRate=" << migrationRate << "deathRateLambda=" << deathRateLambda;
////							code = codestream.str();
////							simulation(code, outputFolder,
////									minGenerations, numSubpopulations,
////									minPopsize, fecundityCurvature, growthRateR, geneticMutationRate, brainMutationDev,
////									adaptKnowDeviation, lossyLearningValue, obliqueLearningDeviation, learningBiasDeviation,
////									indivLearningDeviation, indivLearningMean,
////									migrationRate,
////									maxBrainCost, deathRateLambda,
////									initBrain, initAdaptKnowledge, initIndivLearning,
////									initObliqueLearning, initLearningBias, initPopSize);
////							//}
////				}
////				}
////			}
////		}
////	}
//	//}

	return 0;
}
