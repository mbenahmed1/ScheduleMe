#ifndef SCHEDULEME_SIMULATED_ANNEALING_H
#define SCHEDULEME_SIMULATED_ANNEALING_H

#include <algorithm>
#include <random>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "instance.hpp"
#include "schedule_generator.hpp"
#include "neighborhoods.hpp"

namespace ScheduleMe
{

class SimulatedAnnealing
{

public:
    explicit SimulatedAnnealing(int time_limit, double alpha, double start_temp, unsigned int seed, bool verbose);

    std::vector<unsigned int> solve(Instance &instance);

    double get_alpha() const;

    double get_start_temp() const;

private:

    Neighborhoods nbh;

    double reanneal_temp(double t_i) const;

    double next_temp(double t_i, int i) const;

    static double euler(unsigned int c_s_dash, unsigned int c_s, double t_i);

    bool verbose;
    int time_limit;
    unsigned int seed;
    double alpha;
    double start_temp;

};


} // namespace ScheduleMe

#endif // SCHEDULEME_SIMULATED_ANNEALING_H
