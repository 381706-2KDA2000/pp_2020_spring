// Copyright 2020 Denisov Vladislav
#include "../../../modules/task_2/denisov_v_monte_karlo/monte_karlo.h"

#include <omp.h>
#include <functional>
#include <vector>
#include <random>
#include <iostream>

double getIntegralMonteCarloPar(const std::function<double(const std::vector<double>&)>& func,
    const std::vector<double>& lowLimInt, const std::vector<double>& highLimInt,
    const unsigned int numberPoints, const int seed) {

    // Exceptions
    if (numberPoints == 0)
        throw "The number of points must be greater than zero";
    if (lowLimInt.empty() || highLimInt.empty())
        throw "Integration limits are not set";
    if (lowLimInt.size() != highLimInt.size())
        throw "The dimensions for the integration limits do not match.";

    // Prepare random-generation
    unsigned int countDims = static_cast<unsigned int>(lowLimInt.size());
    std::vector<std::uniform_real_distribution<double> > randPoints(countDims);
    for (unsigned int i = 0; i < countDims; i++)
        randPoints[i] = std::uniform_real_distribution<double>(lowLimInt[i], highLimInt[i]);

    double result = 0.0;
#pragma omp parallel reduction(+: result)
{
    // Init
    unsigned int number_thread = static_cast<unsigned int>(omp_get_thread_num());
    std::mt19937 generator;
    generator = std::mt19937(seed + number_thread);

    // Monte-Carlo random
    std::vector<double> point(countDims);
    #pragma omp for
    for (int i = 0; i < numberPoints; i++) {
        for (int j = 0; j < countDims; j++)
            point[j] = randPoints[j](generator);
        result += func(point);
    }
}

    // Monte-Carlo calc
    for (unsigned int i = 0; i < countDims; i++) {
        result *= (highLimInt[i] - lowLimInt[i]);
    }
    result /= numberPoints;

    return result;
}

double getIntegralMonteCarloSeq(const std::function<double(const std::vector<double>&)>& func,
    const std::vector<double>& lowLimInt, const std::vector<double>& highLimInt,
    const unsigned int numberPoints, const int seed) {

    // Exceptions
    if (numberPoints == 0)
        throw "The number of points must be greater than zero";
    if (lowLimInt.empty() || highLimInt.empty())
        throw "Integration limits are not set";
    if (lowLimInt.size() != highLimInt.size())
        throw "The dimensions for the integration limits do not match.";

    // Init
    double result = 0.0;
    std::mt19937 generator;
    generator = std::mt19937(seed);
    unsigned int countDims = static_cast<unsigned int>(lowLimInt.size());

    // Prepare random-generation
    std::vector<std::uniform_real_distribution<double> > randPoints(countDims);
    for (unsigned int i = 0; i < countDims; i++)
        randPoints[i] = std::uniform_real_distribution<double>(lowLimInt[i], highLimInt[i]);

    // Monte-Carlo random
    std::vector<double> point(countDims);
    for (unsigned int i = 0; i < numberPoints; i++) {
        for (unsigned int j = 0; j < countDims; j++)
            point[j] = randPoints[j](generator);
        result += func(point);
    }

    // Monte-Carlo calc
    for (unsigned int i = 0; i < countDims; i++) {
        result *= (highLimInt[i] - lowLimInt[i]);
    }
    result /= numberPoints;

    return result;
}

