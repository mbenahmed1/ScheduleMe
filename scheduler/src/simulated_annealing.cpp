#include "../include/simulated_annealing.hpp"

namespace ScheduleMe
{

SimulatedAnnealing::SimulatedAnnealing(double time_limit, double alpha, double t_i, int seed, bool verbose) : 
time_limit(time_limit), alpha(alpha), start_temp(start_temp), seed(seed), nbh(seed), verbose(verbose)
{
}

std::vector<unsigned int> SimulatedAnnealing::solve(Instance &instance)
{
    // Current, neighbor, best solution and target function value
    std::vector<unsigned int>       s, s_dash, s_best;
    int                             c_s, c_s_dash, c_s_best;

    // Current temperature and iteration
    double      t_i     = this->start_temp;
    int         i       = 0;

    // Init random generate with given seed
    srand(this->seed);

    // Generate an inital solution s
    s = ScheduleGenerator::generate_precedence_list(instance);
    c_s = ScheduleGenerator::earliest_start_schedule(instance, s);

    // Initial best solution is the first one
    c_s_best = c_s;
    s_best = s;

    // Simulated annealing while time limit is not exceeded
    time_t start_time = time(NULL);
    time_t time_spent = 0;
    while(time_spent < time_limit)
    {
        // Generate randomly a solution s_dash
        s_dash = nbh.swap(s, instance);
        c_s_dash = ScheduleGenerator::earliest_start_schedule(instance, s_dash);

        // If s_dash is better than the current solution or the threshold value 
        // allows a worse solution use s_dash in next step
        double random = (double) rand() / RAND_MAX;
        if(random < std::min(1.0, euler(c_s_dash, c_s, t_i)))
        {
            s = s_dash;

            // If s_dash is the new best solution
            if(c_s_dash < c_s_best)
            {
                c_s_best = c_s_dash;
                s_best = s_dash;
                // if (verbose)
                // {
                //     // TODO
                // }
            }
        }

        i++;
        t_i = next_temp(t_i, i);
        // reanneal_temp(t_i);

        time_spent = time(NULL) - start_time;
    }

    return s_best;
}

double SimulatedAnnealing::euler(int c_s_dash, int c_s, double t_i)
{
    return std::exp(- (c_s_dash - c_s) / t_i);
}

double SimulatedAnnealing::get_alpha() const
{
    return this->alpha;
}

double SimulatedAnnealing::get_start_temp() const
{
    return this->start_temp;
}

double SimulatedAnnealing::reanneal_temp(double t_i)
{
    if(t_i < 0.001)
    {
        t_i = this->start_temp;
        return t_i;
    }
    return t_i;
}

double SimulatedAnnealing::next_temp(double t_i, int i)
{
    return t_i * alpha;
    // return t_i / (1 + std::log(1 + i));
}

} // namespace ScheduleMe
