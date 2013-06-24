/*
 * agent.h
 *
 *  Created on: Jul 25, 2012
 *      Author: mike
 */

#ifndef AGENT_H_
#define AGENT_H_
#include <vector>
#include <string>

class Agent {
  private:
        double _brain; //In this version the brain is continuous
        double _indivLearning;
    double _obliqueLearning;
    double _learningBias;
    double _culture;
    double _parentCulture;
  public:
    Agent (double brain, double indivLearning, double horizLearning, double learningBias, double culture);
    Agent ();
    ~Agent() {;}
    void setBrain(double brain) {this->_brain = brain;}
    void setIndivLearning(double indivLearning) {this->_indivLearning = indivLearning;}
    void setObliqueLearning(double obliqueLearning) {this->_obliqueLearning = obliqueLearning;}
    void setLearningBias(double learningBias) {this->_learningBias = learningBias;}
    void setCulture(double culture) {this->_culture = std::min(culture,_brain);} //Caps culture at brain size
    void setParentCulture(double culture) {this->_parentCulture = culture;}

    double getBrain() {return this->_brain;}
        double getIndivLearning() {return this->_indivLearning;}
        double getObliqueLearning() {return this-> _obliqueLearning;}
        double getLearningBias() {return this->_learningBias;}
        double getCulture() {return this->_culture;}
        double getParentCulture() {return this->_parentCulture;}
        std::string std_output(void);
        void std_input(std::string input);
};


#endif /* AGENT_H_ */
