#ifndef SCHEDULEME_SIMULATED_ANNEALING_H
#define SCHEDULEME_SIMULATED_ANNEALING_H

#include <algorithm>
#include <random>
#include <vector>
#include <cstdlib>
#include "instance.hpp"
#include "schedule_generator.hpp"

namespace ScheduleMe
{

class SimulatedAnnealing
{

public:
    explicit SimulatedAnnealing(double time_limit, double alpha, double start_temp, int seed);

    int solve(Instance &instance);

    double get_alpha() const;

    double get_start_temp() const;

private:

    double reanneal_temp(double t_i);

    double next_temp(double t_i, int i);

    static double euler(int c_s_dash, int c_s, double t_i);

    double time_limit;
    int seed;
    double alpha;
    double start_temp;

};


} // namespace ScheduleMe

#endif // SCHEDULEME_SIMULATED_ANNEALING_H
