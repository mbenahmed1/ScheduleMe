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
    explicit SimulatedAnnealing(double time_limit, double alpha, double start_temp, unsigned int seed, bool verbose, std::string neighborhood);

    std::vector<unsigned int> solve(Instance &instance);
   
    double get_alpha() const;

    double get_start_temp() const;

private:

    Neighborhoods nbh;

    double reanneal_temp(double t_i) const;

    double next_temp(double t_i, int i) const;

    static double euler(double c_s_dash, double c_s, double t_i);

    void print_progress(unsigned int c_opt, unsigned int c_s, int step_count, double temp, double time_spent) const;

    bool verbose;
    double time_limit;
    unsigned int seed;
    double alpha;
    double start_temp;
    std::mt19937 mt_rand;
    std::uniform_real_distribution<double> dis;
    std::string neighborhood;

};


} // namespace ScheduleMe

#endif // SCHEDULEME_SIMULATED_ANNEALING_H
