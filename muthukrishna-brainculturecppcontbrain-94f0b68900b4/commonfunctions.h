/*
 * commonfunctions.h
 *
 *  Created on: Jul 25, 2012
 *      Author: mike
 */

#ifndef COMMONFUNCTIONS_H_
#define COMMONFUNCTIONS_H_

#include <vector>
#include "agent.h"

double getMeanCommunityCulture(std::vector<Agent> subpop);

int numChildren(int N0, double r, double c, int minPop);

double getTotalCommunityCultureToPower(std::vector<Agent> subpop, double power);

double getTotalCommunityCultureFromSigmoid(std::vector<Agent> subpop, double meanCulture, double curvature);

double sigmoid(double value, double inflection, double curvature);

double carryingCapacity(int minPop, double culture);

double deathRate(double lambda, int rawbrain, double brain, double culture, double brainCost);

std::vector<double> getMeanPopulationParameters(std::vector< std::vector<Agent> > population);


#endif /* COMMONFUNCTIONS_H_ */
