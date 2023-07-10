//
// Created by Aneta Dufková on 08/11/2020.
//

#ifndef FIREFLY_H
#define FIREFLY_H
#include <vector>
#include <numeric>
#include <math.h>
#include <random>

using namespace std;

const double INIT_ATTRACTIVENESS = 1;
const double CURVE_SHARPNESS = 2;
const double ALPHA = 0.2;
const int ITERATIONS = 20;
const int NUMBER_OF_FIREFLIES = 60;

/**
 * @struct benchmark
 * @brief encapsulates information loaded from file and const values computed from them
 * @var benchmark::capacity - the knapsack capacity
 * @var benchmark::weights - the weights of the objects
 * @var benchmark::profits - the profits of each object
 * @var benchmark::solution -  the optimal selection of weights
 * @var benchmark::rhoMax - maximum ratio (profit/weight)
 * @var benchmark::lightAbs - light absorption given by number of objects and CURVE_SHARPNESS
 */
struct benchmark{
    int capacity;
    vector<int> weights;
    vector<int> profits;
    vector<int> solution;
    double rhoMax;
    int profit;
};

/**
 * @class Firefly
 *
 * Represents one firefly with its ID number, fitness function and solution.
 * The class contains also benchmark, it is used for computations in many functions.
 */
class Firefly {
    private:
        static int ID;
        int ffID;
        benchmark ben;
        vector<int> solution;
        int fitnessFunction;

        /**
         * Check constraints
         *
         * Checks if solution meets the constraint
         * (if sum of weights is not greater than backpack capacity).
         *
         * @param newSolution solution to be checked
         * @return true if the constraint is met, false otherwise
         */
        bool checkConstraint(vector<int> newSolution);

        /**
         * Repair solution which doesn't meet the constraints
         *
         * Takes solution and removes object with the smallest ratio profit/weight.
         *
         * @param newSolution solution to be repaired
         * @return index of object which should be removed
         */
        int repairSolution(vector<int> newSolution);

        /**
         * Compute attractiveness of firefly
         *
         * Computes attractiveness based on distance, light absorbtion.
         *
         * @param distance distance between two fireflies
         * @return attractiveness of firefly
         */
        double computeAttractiveness(double distance, double ff1, double ff2);

        /**
         * Evaluate fitness function
         *
         * Computes fitness function based on benchmark values and solution of the particular firefly.
         * Sets value of firefly fitnessFunction parameter.
         */
        void evaluateFF();

        /**
         * Compute distance
         *
         * Computes Hamming distance between firefly f1 and firefly f2.
         *
         * @param f2 firefly f2
         * @return distance between fireflies
         */
        int computeDistance(Firefly f2);

    public:
        /**
         * Getter for solution
         *
         * Returns value of solution
         *
         * @return solution (array of 0 and 1)
         */
        vector<int> getSolution();

        /**
         * Getter for fitness function
         *
         * Returns value of fitness function
         *
         * @return fitness function value
         */
        int getFitnessFunction();

        /**
         * Computes profit of the firefly
         *
         * Computes profit of the firefly
         * determined by chosen object saved in solution
         *
         * @return profit value
         */
        int getProfit();

        /**
         * Constructor
         *
         * Create new firefly, set solution, fitness function, ID and save benchmark used for computation.
         *
         * @param solution array of 0 and 1 (1 means the object is chosen)
         * @param benchmark benchmark with information about capacity, weights, profits etc.
         */
        Firefly(vector<int> solution, benchmark ben);
        /**
         * Move firefly i towards firefly j
         *
         * Changes firefly fitness solution and function by moving towards firefly j.
         *
         * @param f2 the firefly j
         */
        void move(Firefly f2);
        /**
         * Generate opposite solution.
         *
         * Transform solution of firefly to the opposite one.
         */
        void transformOpposite();
        /**
         * Print information about firefly.
         *
         * This function prints ID number, solution and fitness function of particular firefly.
         */
        void print();

};



#endif //FIREFLY_H
