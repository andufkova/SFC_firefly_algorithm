#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include "Firefly.h"
using namespace std;

/**
 * Generate initial population
 *
 * Creates a population of new fireflies.
 *
 * @param count - number of fireflies
 * @param dimension - how many objects is possible to give to the knapsack
 * @param ben - benchmark with weights, profits of objects and knapsack capacity
 * @return array of new fireflies
 */
vector<Firefly> generateInitPop(int count, int dimension, benchmark ben){
    vector<Firefly> ff;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    double random;
    for(int i = 0; i < count; i++){
        vector<int> solution;
        for(int j = 0; j < dimension; j++){
            random = dis(gen);
            if(random < 0.5){
                solution.push_back(0);
            } else {
                solution.push_back(1);
            }
        }

        Firefly f = Firefly(solution, ben);

        ff.push_back(f);
    }
    return ff;
}

/**
 * Evaluate population
 *
 * Compare each firefly (its profit)
 * to the current best solution.
 *
 * @param pop - the whole population
 * @param best_solution - current best solution
 * @return new best solution
 */
Firefly evaluatePop(vector<Firefly> pop, Firefly best_solution){
    for(int i = 0; i < pop.size(); i++){
       if(pop[i].getProfit() > best_solution.getProfit()){
           best_solution = pop[i];
       }
    }
    return best_solution;
}

/**
 * Compute argmax
 *
 * Finds out the index of maximum value
 * (firefly with the highest fitness function).
 *
 * @param pop - population of fireflies
 * @return index of the highest fitness function
 */
int getArgMax(vector<Firefly> pop){
    int index = 0;
    Firefly best_solution = pop[0];
    for(int i = 0; i < pop.size(); i++){
        if(pop[i].getFitnessFunction() > best_solution.getFitnessFunction()){
            best_solution = pop[i];
            index = i;
        }
    }
    return index;
}

/**
 * Computes weight
 *
 * Computes sum of weights of objects which should be put to knapsac.
 *
 * @param weights - the weights of the objects
 * @param solution - which objects should be put to knapsack
 * @return sum of weights
 */
int weight(vector<int>weights, vector<int>solution){
    int sum = 0;
    for(int i = 0; i < weights.size(); i++){
        sum += weights[i] * solution[i];
    }
    return sum;
}

/**
 * Computes maximum ratio
 *
 * @param profits - the profits of each object
 * @param weights - the weights of the objects
 * @return maximum ratio
 */
double getRhoMax(vector<int> profits, vector<int> weights){
    double max = 0.0;
    for(int i = 0; i < profits.size(); i++){
        double tmp = profits[i]/weights[i];
        if(tmp > max){
            max = tmp;
        }
    }
    return max;
}

int main(int argc, char** argv) {

    // file structure (values separated by space)
    // 1st line: knapsack capacity
    // 2nd line: the weights of the objects
    // 3rd line: the profits of each object
    // 4th line: the optimal selection [0, 1, ...]

    if(argc == 1){
        cout << "Please run the code with parameter: .fa/ benchmarkfile" << endl;
    } else {
        ifstream file(argv[1]);
        int n = 0;
        int g = 0;

        for(int i = 2; i < argc; i++){
            if(argv[i][0] == 'n'){
                string tmp;
                tmp = argv[i];
                n = atoi(tmp.substr(2).c_str());
            } else if(argv[i][0] == 'g'){
                string tmp;
                tmp = argv[i];
                g = atoi(tmp.substr(2).c_str());
            } else {
                cout << "Unsupported argument." << endl;
            }
        }

        if (file.is_open()) {
            string line;
            vector<vector <string> > tmpV;
            for (int num_lines = 0; num_lines < 4 && getline(file, line); num_lines++) {

                vector<string> tokens;
                istringstream iss(line);
                copy(istream_iterator<string>(iss),
                     istream_iterator<string>(),
                     back_inserter(tokens));

                tmpV.push_back(tokens);
            }

            if(tmpV[1].size() == tmpV[2].size() && tmpV[0].size() == 1){
                benchmark ben;
                ben.capacity = atoi(tmpV[0].at(0).c_str());
                for(int i = 1; i < tmpV.size(); i++){
                    for(int j = 0; j < tmpV[i].size(); j++){
                        switch(i){
                            case 1:
                                ben.weights.push_back(atoi(tmpV[i].at(j).c_str()));
                                break;
                            case 2:
                                ben.profits.push_back(atoi(tmpV[i].at(j).c_str()));
                                break;
                            case 3:
                                ben.solution.push_back(atoi(tmpV[i].at(j).c_str()));
                                break;
                        }
                    }
                }

                ben.rhoMax = getRhoMax(ben.profits, ben.weights);

                if(n == 0){
                    n = NUMBER_OF_FIREFLIES;
                }

                // generate init pop
                vector<Firefly> firefly_pop = generateInitPop(n, ben.weights.size(), ben);
                Firefly best_solution = firefly_pop[0];

                best_solution = evaluatePop(firefly_pop, best_solution);

                // MAIN LOOP
                int counter = 0;
                bool run = true;
                if(g == 0){
                    g = ITERATIONS;
                }
                while(counter < g && run){
                    int arg_max = getArgMax(firefly_pop);
                    for(int i = 0; i < firefly_pop.size(); i++){
                        for(int j = 0; j < firefly_pop.size(); j++){


                            if(firefly_pop[j].getFitnessFunction() > firefly_pop[i].getFitnessFunction()){
                                // move
                                firefly_pop[i].move(firefly_pop[j]);
                            }
                        }
                    }

                    best_solution = evaluatePop(firefly_pop, best_solution);
                    //cout << "BEST SOLUTION AFTER LOOP "  << counter << endl;
                    //best_solution.print();


                    if(best_solution.getSolution() == ben.solution){
                        run = false;
                        //cout << "SOLUTION FOUND!" << endl;
                    }
                    counter++;

                    firefly_pop[arg_max].transformOpposite();
                }

                cout << "BEST SOLUTION found after loop " << counter << endl;
                best_solution.print();

            } else {
                cout << "The file does not meet the required format." << endl;
            }
            file.close();
        } else {
            cout << "The file " << argv[1] << " can not be opened." << endl;
        }
    }





    return 0;
}
