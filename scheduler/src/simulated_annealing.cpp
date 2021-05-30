#include "../include/simulated_annealing.hpp"

namespace ScheduleMe
{

SimulatedAnnealing::SimulatedAnnealing(double time_limit, double alpha, double start_temp, unsigned int seed, bool verbose, std::string neighborhood) :
nbh(seed), verbose(verbose), time_limit(time_limit), seed(seed),  alpha(alpha), start_temp(start_temp), mt_rand(seed), dis(0.0,1.0), neighborhood(neighborhood)
{
}

std::vector<unsigned int> SimulatedAnnealing::solve(Instance &instance)
{
    std::cout   << std::fixed << std::setprecision(5);

    // auto start = std::chrono::high_resolution_clock::now();
    double time_spent = 0;
    time_limit *= 1000;

    // Current, neighbor, best solution and target function value
    std::vector<unsigned int>       s, s_dash, s_opt, tmp;
    unsigned int                    c_s, c_s_dash, c_s_opt;
    double                          min, random;

    // Current temperature and iteration
    double          t_i         = this->start_temp;
    int             i           = 0;
    int             approx_i    = -1;

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

    // Approximate alpha and number of total iterations if alpha is not given
    tmp = s;
    if  (alpha == 0.0) 
    {
        double total = 0.0;
        if (neighborhood == "random")
        {
            auto before = std::chrono::high_resolution_clock::now();
            nbh.swap(tmp, instance);
            nbh.api(tmp, instance);
            nbh.shift(tmp, instance);
            auto after_nbh = std::chrono::high_resolution_clock::now();
            c_s = ScheduleGenerator::earliest_start_schedule(instance, tmp);
            auto after_ess = std::chrono::high_resolution_clock::now();
            total = std::chrono::duration<double, std::milli>(after_nbh - before).count() / 3.0 + 
                            std::chrono::duration<double, std::milli>(after_ess - after_nbh).count();

            // std::cout << total / 1000 << std::endl;
        }
        else
        {
            auto before = std::chrono::high_resolution_clock::now();
            (nbh.*make_neighbor)(tmp, instance);
            c_s = ScheduleGenerator::earliest_start_schedule(instance, tmp);
            auto after = std::chrono::high_resolution_clock::now();
            total = std::chrono::duration<double, std::milli>(after - before).count();

            // std::cout << total / 1000 << std::endl;
        }
        approx_i = time_limit / total;
        alpha = std::pow(0.00001 / t_i, 1.0 / approx_i);
    }

    std::cout   << "Initial makespan:" << std::right << std::setw(4) << c_s 
                << std::right << std::setw(39) << " | Total aprx:" << std::right << std::setw(9) << approx_i
                << " | Time limit:" << std::right << std::setw(10) << time_limit / 1000.0
                << " | Initial Tmp: " << std::right << std::setw(9) << t_i
                << " | Alpha: " << std::right << std::setw(7) << alpha << std::endl;

    // Initial best solution is the first one
    s_opt = s;
    c_s_opt = c_s;

    s_dash = s;
    c_s_dash = c_s;

    auto start = std::chrono::high_resolution_clock::now();

    // Simulated annealing while time limit is not exceeded
    //time_t start_time = time(nullptr);
    //time_t time_spent = 0;
    while(time_spent < time_limit)
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
        print_progress(c_s_opt, c_s, i, t_i, time_spent);
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
        time_spent = std::chrono::duration<double, std::milli>(end - start).count();
    }

    std::cout << std::endl;
    return s_opt;
}

double SimulatedAnnealing::euler(double c_s_dash, double c_s, double t_i)
{
    // std::cout << - (c_s_dash - c_s) << std::endl;
    // std::cout << std::exp(- (c_s_dash - c_s) / t_i) << std::endl;
    // std::cout << 1 / std::exp((c_s_dash - c_s) / t_i) << std::endl;
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

void SimulatedAnnealing::print_progress(unsigned int c_s_opt, unsigned int c_s, int step_count, double temp, double time_spent) const
{
    std::cout   << "Optimal makespan:" << std::right << std::setw(4) << c_s_opt
                << " | Current makespan: " << std::right << std::setw(4) << c_s
                << " | Iteration: " << std::right << std::setw(9) << step_count
                << " | Time spent:" << std::right << std::setw(10) << static_cast<double>(time_spent) / 1000.0
                << " | Temperature:" << std::right << std::setw(10) << temp << "\r" << std::flush;
}

} // namespace ScheduleMe
