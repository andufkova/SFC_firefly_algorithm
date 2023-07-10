//
// Created by Aneta Dufková on 08/11/2020.
//

#include <iostream>
#include "Firefly.h"
using namespace std;

int Firefly::ID = 0;

vector<int> Firefly::getSolution(){
    return this->solution;
}

int Firefly::getFitnessFunction(){
    return this->fitnessFunction;
}

bool Firefly::checkConstraint(vector<int> newSolution) {
    int sum = inner_product(begin(newSolution), end(newSolution), begin(this->ben.weights), 0.0);
    if(sum > this->ben.capacity){
        return false;
    } else return true;
}

int Firefly::repairSolution(vector<int> newSolution) {
    vector<double> ratios;
    for(int i = 0; i < this->ben.weights.size(); i++){
        ratios.push_back(this->ben.profits[i]/this->ben.weights[i]);
    }
    double min = 100.0;
    int index = -1;
    for(int i = 0; i < ratios.size(); i++){
        if(ratios[i] < min && newSolution[i] == 1){
            index = i;
            min = ratios[i];
        }
    }

    return index;
}

Firefly::Firefly(vector<int> solution, benchmark ben) {
    this->ben = ben;
    // check if the solution meet the constraints
    while(!checkConstraint(solution)){
        int index = repairSolution(solution);
        solution[index] = 0;
    }
    this->solution = solution;
    this->ffID = Firefly::ID;
    Firefly::ID++;
    this->evaluateFF();
}

void Firefly::evaluateFF() {
    double ff = inner_product(begin(this->solution), end(this->solution), begin(this->ben.profits), 0.0);
    double tmp = inner_product(begin(this->solution), end(this->solution), begin(this->ben.weights), 0.0);
    tmp -= this->ben.capacity;
    tmp *= this->ben.rhoMax;
    tmp *= tmp;
    ff -= tmp;
    this->fitnessFunction = (int)ff;
}

int Firefly::getProfit(){
    int tmp = inner_product(begin(this->solution), end(this->solution), begin(this->ben.profits), 0);
    return tmp;
}

int Firefly::computeDistance(Firefly f2) {
    int sum = 0;
    for(int i = 0; i < this->solution.size(); i++){
        if(this->solution[i] != f2.solution[i]){
            sum++;
        }
    }
    return sum;
}

double Firefly::computeAttractiveness(double distance, double ff1, double ff2) {
    double lenghtscale = abs(ff1-ff2)/distance;
    double exponent = -lenghtscale * pow(distance, CURVE_SHARPNESS);
    double tmp = INIT_ATTRACTIVENESS * pow(exp(1), exponent);
    return tmp;
}


void Firefly::move(Firefly f2) {
    // move firefly i towards firefly j
    int distance = this->computeDistance(f2);
    double attractiveness = this->computeAttractiveness(distance, this->fitnessFunction, f2.fitnessFunction);
    vector<int> f2_solution = f2.solution;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    // beta step
    vector<int> newSolution;
    for(int i = 0; i < this->solution.size(); i++){
        if(this->solution[i] == f2_solution[i]){
            newSolution.push_back(f2_solution[i]);
        } else {

            double random = dis(gen);
            if(random < attractiveness){
                newSolution.push_back(f2_solution[i]);
            } else {
                newSolution.push_back(this->solution[i]);
            }
        }
    }
    // alpha step

    for(int i = 0; i < newSolution.size(); i++){
        double random = dis(gen);
        if (random < ALPHA){
            newSolution[i] = 1 - newSolution[i];
        }
    }


    while(!checkConstraint(newSolution)){
        int index = repairSolution(newSolution);
        newSolution[index] = 0;
    }

    this->solution = newSolution;
    this->evaluateFF();


}

void Firefly::transformOpposite() {
    for(int i = 0; i < this->solution.size(); i++){
        this->solution[i] = 1 - this->solution[i];
    }
    while(!checkConstraint(this->solution)){
        int index = repairSolution(this->solution);
        this->solution[index] = 0;
    }
    this->evaluateFF();
}

void Firefly::print(){
    printf("*** Firefly number %d\n", this->ffID);
    printf("*** solution: ");
    for(int i = 0; i < this->solution.size(); i++){
        printf("%d", this->solution[i]);
    }
    printf("\n");
    printf("*** profit: %d\n\n", this->getProfit());
}