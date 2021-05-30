#include "../include/simulated_annealing.hpp"

namespace ScheduleMe
{

SimulatedAnnealing::SimulatedAnnealing(long time_limit, double alpha, double start_temp, unsigned int seed, bool verbose, std::string neighborhood) :
nbh(seed), verbose(verbose), time_limit(time_limit), seed(seed),  alpha(alpha), start_temp(start_temp), mt_rand(seed), dis(0.0,1.0), neighborhood(neighborhood)
{
}

std::vector<unsigned int> SimulatedAnnealing::solve(Instance &instance)
{
    auto start = std::chrono::high_resolution_clock::now();
    long time_spent = 0;

    // Current, neighbor, best solution and target function value
    std::vector<unsigned int>       s, s_dash, s_opt;
    unsigned int                    c_s, c_s_dash, c_s_opt;
    double                          min, random;

    // Current temperature and iteration
    double      t_i     = this->start_temp;
    int         i       = 0;

    // Generate an inital solution s
    s = ScheduleGenerator::generate_precedence_list(instance);
    c_s = ScheduleGenerator::earliest_start_schedule(instance, s);

    // Calculate initial temperature
    unsigned int lb1 = 0;
    if (start_temp == 0.0)
    {
        for (unsigned int i = 0; i < instance.r(); i++)
        {
            double sum = 0.0;
            for (unsigned int j = 0; j < instance.n(); j++)
            {
                sum += instance.demands[j][i] * instance.processing_time[j];
            }
            sum = std::ceil(sum / instance.resources[i]);
            // std::cout << sum << std::endl;
            if (sum > lb1)
            {
                lb1 = sum;
            }
        }
        t_i = c_s - lb1;
    }
    std::cout << "Initial temperature: " << t_i << std::endl;
    std::cout << "Initial makespan:" << std::right << std::setw(4) << c_s << std::endl;

    // Which neighborhood shall we use
    bool (Neighborhoods::*make_neighbor)(std::vector<unsigned int>&, Instance&);
    if (neighborhood == "swap")
    {
        make_neighbor = &Neighborhoods::swap;
    }
    else if(neighborhood == "api")
    {
        make_neighbor = &Neighborhoods::api;
    }
    else if(neighborhood == "shift")
    {
        make_neighbor = &Neighborhoods::shift;
    }
    else if(neighborhood == "random")
    {
        make_neighbor = &Neighborhoods::random;
    }

    // Initial best solution is the first one
    s_opt = s;
    c_s_opt = c_s;

    s_dash = s;
    c_s_dash = c_s;

    // Simulated annealing while time limit is not exceeded
    //time_t start_time = time(nullptr);
    //time_t time_spent = 0;
    while(time_spent < time_limit * 1000)
    {

        // Generate randomly a solution s_dash
        // c_s_dash = ScheduleGenerator::earliest_start_schedule(instance, s_dash);
        s_dash = s;
        while(!(nbh.*make_neighbor)(s_dash, instance)) {}
        c_s_dash = ScheduleGenerator::earliest_start_schedule(instance, s_dash);

        // If s_dash is better than the current solution or the threshold value 
        // allows a worse solution use s_dash in next step
        random = dis(mt_rand);
        min = std::min(1.0, euler(c_s_dash, c_s, t_i));
        print_progress(c_s_opt, c_s, i, t_i, time_spent, min);
        if(random < min)
        {
            s = s_dash;
            c_s = c_s_dash;

            // If s_dash is the new best solution
            if(c_s_dash < c_s_opt)
            {
                c_s_opt = c_s_dash;
                s_opt = s_dash;
            }
        }

        i++;
        t_i = next_temp(t_i, i);
        // t_i = reanneal_temp(t_i);

        auto end = std::chrono::high_resolution_clock::now();
        time_spent = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    std::cout << std::endl;
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

void SimulatedAnnealing::print_progress(unsigned int c_s_opt, int step_count, double temp, long time_spent) const
{
    // double progress = static_cast<double>(c_s_opt);
    // int barWidth = 50;
    // std::cout << "[";
    // int pos = static_cast<int>(barWidth * progress);
    // for (int i = 0; i < barWidth; ++i)
    // {
    //     if (i < pos)
    //     {
    //         std::cout << "=";
    //     }
    //     else if (i == pos)
    //     {
    //         std::cout << ">";
    //     }
    //     else
    //     {
    //         std::cout << " ";
    //     }
    // }
    // std::cout << "] " << double(progress * 100.0) << " %\r";

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Step " << step_count << ", time spent " << time_spent << " t_i: " << temp << " c_s_opt: " << c_s_opt << "\r";
    std::cout.flush();
}

} // namespace ScheduleMe
