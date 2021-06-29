#ifndef SCHEDULEME_SIMULATED_ANNEALING_H
#define SCHEDULEME_SIMULATED_ANNEALING_H

#include <iomanip>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <iterator>
#include "instance.hpp"
#include "schedule_generator.hpp"
#include "neighborhoods.hpp"

namespace ScheduleMe
{

class SimulatedAnnealing
{

public:
    explicit SimulatedAnnealing(unsigned int seed, double time_limit, double start_temp, 
        double alpha, std::string neighborhood, bool benchmark);

    std::vector<unsigned int> solve(Instance &instance);

private:
    unsigned int seed;
    double time_limit;
    double start_temp;
    double alpha;

    std::string neighborhood;
    Neighborhoods nbh;

    std::mt19937 mt_rand;
    std::uniform_real_distribution<double> dis;

    bool benchmark;

    std::vector<unsigned int> solve_display(Instance &instance);

    std::vector<unsigned int> solve_benchmark(Instance &instance);

    void print_progress(unsigned int c_s_opt, unsigned int c_s, int step_count, 
        double temp, double time_spent, int approx_i, double alpha) const;
};

} // namespace ScheduleMe

#endif // SCHEDULEME_SIMULATED_ANNEALING_H
