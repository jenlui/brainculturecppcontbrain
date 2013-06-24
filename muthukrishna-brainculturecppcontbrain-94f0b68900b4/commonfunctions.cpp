#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "commonfunctions.h"
#include "agent.h"
#include <math.h>

using namespace std;

// returns the mean community culture of all agents in a subpopulation
double getMeanCommunityCulture(vector<Agent> subpop) {
        double total = 0;
        for(vector<Agent>::iterator it = subpop.begin(); it != subpop.end(); ++it) {
            total += it->getCulture();
        }
        int popsize = subpop.size();
        if (popsize == 0) {
                return 0;
        } else {
                return total / popsize;
        }
}

// determines carrying capacity for a given population cultural mean
// and given maximum population size when no adaptive knowledge has been acquired
double carryingCapacity(int scale, int minPop, double culture) {
        return (scale*log(culture + 1) + minPop);
}

// warning: rounding takes place from double to int
// function determines the number of children that can be supported by??
int numChildren(int N0, double r, double c, int minPop) {
        /*Carrying capacity as a function of culture
                  Malthusian growth ratemultiprocessing.Process
                  N0 = current population size
                  r = generational growth rate
                  c = culture value*/
                return N0 + r*N0*(1- N0/carryingCapacity(minPop,minPop,c));
}

// takes the total mean culture value of a population to a certain power
double getTotalCommunityCultureToPower(vector<Agent> subpop, double power) {
        double total = 0;
        for(vector<Agent>::iterator it = subpop.begin(); it != subpop.end(); ++it) {
                total += pow(it->getCulture(), power);
        }

        return total;
}


double getTotalCommunityCultureFromSigmoid(vector<Agent> subpop, double meanCulture, double curvature) {
        /* Mean culture is the inflection point
         * Curvature controls how curved the sigmoid is. Less curved = more linear
         */
        double total = 0;
        for(vector<Agent>::iterator it = subpop.begin(); it != subpop.end(); ++it) {
                total += sigmoid(it->getCulture(), meanCulture, curvature);
        }

        return total;
}

double sigmoid(double value, double inflection, double curvature) {
    /*if (value < 0) {
    throw new std::string("culture value put into sigmoid should not be negative. Value entered was ");}
    if (curvature < 0) {
    throw new std::string("fecundity curvature should not be negative. Value entered was ");}
    if (inflection < 0) {
    throw new std::string("cultural mean (inflection) should not be negative. Value of cultural mean was ");}
*/
        return 1/(1+exp(-curvature*(value-inflection)));
}


double deathRate(double lambda, int rawbrain, double brain, double culture, double brainCost) {
        /* Death rate as a function of your culture and brain size.
         * Chance of dying ranges from 0 to 1.
         * Death rate increases with higher values of b.
         * Death rate decreases with higher values of c.
         * Brains cost the most when brainCost is low and the least, when brain cost is high.
         * --- e.g. if brainCost is 100, then every brain slot increases maxDeath by 1%.
         * --- maxDeath = 1/brainCost
         */
        if (rawbrain <= 0) {
                return 1;
        } else {
                //double maxDeath = brain/brainCost; //linear growth in maxDeath
                double maxDeath = exp((brain-1)/100)/brainCost; //each brain slot exponentially more costly
                return maxDeath*exp(-lambda*culture/brain);
        }
}

vector<double> getMeanPopulationParameters(vector< vector<Agent> > population) {
        double brainTotal = 0;
        double cultureTotal = 0;
        double iLearningTotal = 0;
        double oLearningTotal = 0;
        double learningBiasTotal = 0;
        int popSize = 0;

        vector<vector<Agent> >::iterator subpop_it = population.begin();
        vector<vector<Agent> >::iterator subpop_end = population.end();
        for (; subpop_it != subpop_end; ++subpop_it) {
                vector<Agent>::iterator a_it = subpop_it->begin();
                vector<Agent>::iterator a_end = subpop_it->end();
                for (; a_it != a_end; ++a_it) {
                        brainTotal += a_it->getBrain();
                        cultureTotal += a_it->getCulture();
                        iLearningTotal += a_it->getIndivLearning();
                        oLearningTotal += a_it->getObliqueLearning();
                        learningBiasTotal += a_it->getLearningBias();
                        popSize++;
                }
        }
        vector<double> output;
        output.push_back(brainTotal/popSize);
        output.push_back(cultureTotal/popSize);
        output.push_back(iLearningTotal/popSize);
        output.push_back(oLearningTotal/popSize);
        output.push_back(learningBiasTotal/popSize);

        return output;
}
