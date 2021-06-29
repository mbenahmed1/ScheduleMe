#include "../include/simulated_annealing.hpp"

namespace ScheduleMe
{

SimulatedAnnealing::SimulatedAnnealing(unsigned int seed, double time_limit, double start_temp,
    double alpha, std::string neighborhood, bool benchmark) :
        seed(seed), time_limit(time_limit), start_temp(start_temp), alpha(alpha), neighborhood(neighborhood),
        nbh(seed), mt_rand(seed), dis(0.0,1.0), benchmark(benchmark)
{}

std::vector<unsigned int> SimulatedAnnealing::solve(Instance &instance)
{
    if (benchmark)
    {
        return solve_benchmark(instance);
    }
    return solve_display(instance);
}

std::vector<unsigned int> SimulatedAnnealing::solve_display(Instance &instance)
{
    auto start = std::chrono::high_resolution_clock::now();
    double time_spent = 0;

    // Current, neighbor, best solution and target function value
    std::vector<unsigned int>       s, s_dash, s_opt, tmp;
    unsigned int                    c_s, c_s_dash, c_s_opt;
    double                          min, random;

    int i           = 0;
    int approx_i    = -1;

    // Choose neighborhood and get function pointer to the corresponding function
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

    // Generate an inital solution s
    s = ScheduleGenerator::generate_precedence_list(instance);
    c_s = ScheduleGenerator::earliest_start_schedule(instance, s);
    s_opt = s;
    c_s_opt = c_s;
    s_dash = s;
    c_s_dash = c_s;

    // Calculate initial temperature
    unsigned int lb1 = 0;
    double t_i = this->start_temp;
    if (t_i == 0.0)
    {
        for (unsigned int i = 0; i < instance.r(); i++)
        {
            double sum = 0.0;
            for (unsigned int j = 0; j < instance.n(); j++)
            {
                sum += instance.demands[j][i] * instance.processing_time[j];
            }
            sum = std::ceil(sum / instance.resources[i]);
            if (sum > lb1)
            {
                lb1 = sum;
            }
        }
        t_i = c_s - lb1;
    }

    std::cout << std::fixed << std::setprecision(5);
    std::cout   << "Initial makespan:" << std::right << std::setw(4) << c_s
                << std::right << std::setw(85)
                << " | Time limit:" << std::right << std::setw(10) << time_limit / 1000.0
                << " | Initial Tmp: " << std::right << std::setw(9) << t_i << std::endl;

    // Simulated annealing while time limit is not exceeded
    while(time_spent < time_limit)
    {
        // Randomly generate a solution s_dash
        s_dash = s;
        while(!(nbh.*make_neighbor)(s_dash, instance)) {}
        c_s_dash = ScheduleGenerator::earliest_start_schedule(instance, s_dash);

        // If s_dash is better than the current solution or the threshold value 
        // allows a worse solution use s_dash in next step
        random = dis(mt_rand);
        min = std::min(1.0, std::exp(- (c_s_dash - c_s) / t_i));
        print_progress(c_s_opt, c_s, i, t_i, time_spent, approx_i, alpha);
        if(random < min)
        {
            s = s_dash;
            c_s = c_s_dash;
            if(c_s_dash < c_s_opt)
            {
                c_s_opt = c_s_dash;
                s_opt = s_dash;
            }
        }

        i++;
        t_i *= alpha;

        // Update alpha
        if (i % 100 == 0)
        {
            double crt_time = std::chrono::duration<double, std::milli>(
                std::chrono::high_resolution_clock::now() - start).count();
            approx_i = (i * time_limit) / crt_time;
            alpha = std::pow(0.001 / t_i, 1.0 / approx_i);
        }

        // Calc time spent
        auto end = std::chrono::high_resolution_clock::now();
        time_spent = std::chrono::duration<double, std::milli>(end - start).count();
    }

    std::cout << std::endl;
    return s_opt;
}

std::vector<unsigned int> SimulatedAnnealing::solve_benchmark(Instance &instance)
{
    auto start = std::chrono::high_resolution_clock::now();
    double time_spent = 0;

    // Current, neighbor, best solution and target function value
    std::vector<unsigned int>       s, s_dash, s_opt, tmp;
    unsigned int                    c_s, c_s_dash, c_s_opt;
    double                          min, random;

    int i           = 0;
    int approx_i    = -1;

    // Choose neighborhood and get function pointer to the corresponding function
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

    // Generate an inital solution s
    s = ScheduleGenerator::generate_precedence_list(instance);
    c_s = ScheduleGenerator::earliest_start_schedule(instance, s);
    s_opt = s;
    c_s_opt = c_s;
    s_dash = s;
    c_s_dash = c_s;

    // Calculate initial temperature
    unsigned int lb1 = 0;
    double t_i = this->start_temp;
    if (t_i == 0.0)
    {
        for (unsigned int i = 0; i < instance.r(); i++)
        {
            double sum = 0.0;
            for (unsigned int j = 0; j < instance.n(); j++)
            {
                sum += instance.demands[j][i] * instance.processing_time[j];
            }
            sum = std::ceil(sum / instance.resources[i]);
            if (sum > lb1)
            {
                lb1 = sum;
            }
        }
        t_i = c_s - lb1;
    }

    // Simulated annealing while time limit is not exceeded
    while(time_spent < time_limit)
    {
        // Randomly generate a solution s_dash
        s_dash = s;
        while(!(nbh.*make_neighbor)(s_dash, instance)) {}
        c_s_dash = ScheduleGenerator::earliest_start_schedule(instance, s_dash);

        // If s_dash is better than the current solution or the threshold value 
        // allows a worse solution use s_dash in next step
        random = dis(mt_rand);
        min = std::min(1.0, std::exp(- (c_s_dash - c_s) / t_i));
        if(random < min)
        {
            s = s_dash;
            c_s = c_s_dash;
            if(c_s_dash < c_s_opt)
            {
                c_s_opt = c_s_dash;
                s_opt = s_dash;
            }
        }

        i++;
        t_i *= alpha;

        // Update alpha
        if (i % 100 == 0)
        {
            double crt_time = std::chrono::duration<double, std::milli>(
                std::chrono::high_resolution_clock::now() - start).count();
            approx_i = (i * time_limit) / crt_time;
            alpha = std::pow(0.001 / t_i, 1.0 / approx_i);
        }

        // Calc time spent
        auto end = std::chrono::high_resolution_clock::now();
        time_spent = std::chrono::duration<double, std::milli>(end - start).count();
    }

    std::cout << c_s_opt;
    std::cout << std::endl;
    return s_opt;
}

void SimulatedAnnealing::print_progress(unsigned int c_s_opt, unsigned int c_s, int step_count, 
    double temp, double time_spent, int approx_i, double alpha) const
{
    std::cout   << "Optimal makespan:" << std::right << std::setw(4) << c_s_opt
                << " | Current makespan: " << std::right << std::setw(4) << c_s
                << " | Iteration: " << std::right << std::setw(9) << step_count
                << " | Approx It: " << std::right << std::setw(9) << approx_i
                << " | Time spent:" << std::right << std::setw(10) << static_cast<double>(time_spent) / 1000.0
                << " | Temperature:" << std::right << std::setw(10) << temp
                << " | Alpha:" << std::right << std::setw(10) << alpha << "\r" << std::flush;
}

} // namespace ScheduleMe
