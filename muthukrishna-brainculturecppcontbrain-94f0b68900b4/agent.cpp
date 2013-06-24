/*
 * agent.cpp
 *
 *  Created on: Jul 25, 2012
 *      Author: mike
 */
#include "agent.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// When commiting, use message:
// Updated functions with comments, agentTests passing

using namespace std;

// warning: if double value can be written as an integer, it is converted to an integer in output
std::string Agent::std_output() {
        std::ostringstream output;
        output << _brain << "," << _indivLearning << "," << _obliqueLearning << "," << _learningBias << "," << _culture << ";";
        return output.str();
}


// TEST WITH culture values greater than brain size <-- throw exception
void Agent::std_input(std::string input) {
        std::vector<std::string> splitStrings;
        std::istringstream f(input);
        std::string s;
        while(getline(f,s,',')) {
                splitStrings.push_back(s);
        }

        _brain = atof(splitStrings[0].c_str());
        _indivLearning = atof(splitStrings[1].c_str());
        _obliqueLearning = atof(splitStrings[2].c_str());
        _learningBias = atof(splitStrings[3].c_str());
        _culture = atof(splitStrings[4].c_str());
}



Agent::Agent(double b, double il, double ol, double lb, double c) {
    if (il<0 or il>1) {
        throw new std::string("Bad indivLearning value (il):");
    } else if (ol<0 or ol>1) {
        throw new std::string("Bad oblique learning value (ol):");
    } else if (b<0) {
        throw new std::string("Bad brain value (b):");
    } else if (c<0) {
        throw new std::string("Bad culture value (c)");
    }
     else {
                _brain = b;
                _indivLearning = il;
                _obliqueLearning = ol;
                _learningBias = lb;
                _culture = c;
    }
}

Agent::Agent() {
    _brain = 1;
    _indivLearning = 0;
    _obliqueLearning = 0;
    _learningBias = 0;
    _culture = 0;
}
