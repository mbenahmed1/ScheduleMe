#include "../include/simulated_annealing.hpp"

namespace ScheduleMe
{

SimulatedAnnealing::SimulatedAnnealing(int time_limit, double alpha, double start_temp, unsigned int seed, bool verbose) :
 nbh(seed), verbose(verbose), time_limit(time_limit), seed(seed),  alpha(alpha), start_temp(start_temp)
{
}

std::vector<unsigned int> SimulatedAnnealing::solve(Instance &instance)
{
    // Current, neighbor, best solution and target function value
    std::vector<unsigned int>       s, s_dash, s_opt;
    unsigned int                    c_s, c_s_dash, c_s_opt;

    // Current temperature and iteration
    double      t_i     = this->start_temp;
    int         i       = 0;

    // Init random generate with given seed
    srand(this->seed);

    // Generate an inital solution s
    s = ScheduleGenerator::generate_precedence_list(instance);
    c_s = ScheduleGenerator::earliest_start_schedule(instance, s);

    // Initial best solution is the first one
    c_s_opt = c_s;
    s_opt = s;

    // Simulated annealing while time limit is not exceeded
    //time_t start_time = time(nullptr);
    //time_t time_spent = 0;
    auto start = std::chrono::high_resolution_clock::now();
    long time_spent = 0;
    while(time_spent < time_limit * 1000)
    {
        // for ( unsigned int i = 0; i < s_dash.size(); i++)
        // {

        // }

        // Generate randomly a solution s_dash
        s_dash = s;
        // std::cout << (s_dash == s) << std::endl;
        nbh.swap(s_dash, instance);
        c_s_dash = ScheduleGenerator::earliest_start_schedule(instance, s_dash);
        // std::cout << (s_dash == s) << std::endl;

        // If s_dash is better than the current solution or the threshold value 
        // allows a worse solution use s_dash in next step
        double random = static_cast<double>(rand()) / RAND_MAX;
        if(random < std::min(1.0, euler(c_s_dash, c_s, t_i)))
        {
            // std::cout << (s == s_dash) << std::endl;
            s = s_dash;
            // std::cout << (s == s_dash) << std::endl;

            // If s_dash is the new best solution
            if(c_s_dash < c_s_opt)
            {
                c_s_opt = c_s_dash;
                // std::cout << (s_opt == s_dash) << std::endl;
                s_opt = s_dash;
                // std::cout << (s_opt == s_dash) << std::endl;
                // std::cout << (s_opt == s) << std::endl;
                // std::cout << (s == s_dash) << std::endl;
                // std::cout << (s_opt == s_dash) << std::endl;
                // if (verbose)
                // {
                //     // TODO
                // }
            }
        }

        i++;
        t_i = next_temp(t_i, i);
        // reanneal_temp(t_i);

        auto end = std::chrono::high_resolution_clock::now();
        time_spent = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    std::cout << "s makespan " << ScheduleGenerator::earliest_start_schedule(instance, s) << std::endl;
    std::cout << "s_dash makespan " << ScheduleGenerator::earliest_start_schedule(instance, s_dash) << std::endl;
    std::cout << "s_opt makespan " << ScheduleGenerator::earliest_start_schedule(instance, s_opt) << std::endl;

    std::cout << "c_s " << c_s << std::endl;
    std::cout << "c_s_dash " << c_s_dash << std::endl;
    std::cout << "c_s_opt " << c_s_opt << std::endl;

    return s_opt;
}

double SimulatedAnnealing::euler(unsigned int c_s_dash, unsigned int c_s, double t_i)
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

double SimulatedAnnealing::reanneal_temp(double t_i) const
{
    if(t_i < 0.001)
    {
        t_i = this->start_temp;
        return t_i;
    }
    return t_i;
}

double SimulatedAnnealing::next_temp(double t_i, int i) const
{
    return t_i * alpha;
    // return t_i / (1 + std::log(1 + i));
}

} // namespace ScheduleMe
