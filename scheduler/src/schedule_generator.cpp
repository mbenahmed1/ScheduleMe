//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.hpp"
#include <numeric>

namespace ScheduleMe
{

unsigned int
ScheduleGenerator::earliest_start_schedule(Instance &instance, const std::vector<unsigned int> &act_list, bool verbose)
{
    unsigned int makespan = 0;
    ResourceProfile rp(instance);
    instance.start_time = std::vector<unsigned int>(instance.n(), 0);

    for (unsigned int lambda = 0; lambda < act_list.size(); lambda++)
    {
        unsigned int j = act_list[lambda];
        unsigned int t = 0;

        // calculate start time for j
        for (unsigned int i = 0; i < instance.predecessors[j].size(); i++)
        {
            unsigned int sum = instance.start_time.at(instance.predecessors[j][i]) +
                               instance.processing_time.at(instance.predecessors[j][i]);
            if (sum > t)
            {
                t = sum;
            }
        }

        while (!rp.is_schedulable(t, j))
        {
            t += 1;
        }
        rp.schedule_at(t, j);
        instance.start_time.at(j) = t;

        unsigned int completion_time = t + instance.processing_time[j];
        if (completion_time > makespan)
        {
            makespan = completion_time;
        }

        //std::cout << "\n\n--------------------------------------------------------" << std::endl;
        //std::cout << "Added " << j << " at " << t << " p_i: " << instance.processing_time[j] <<  " r_i: " << instance.demands[j][0] << ", " << instance.demands[j][1] << ", " << instance.demands[j][2] << ", "<< instance.demands[j][3] << ", "<< std::endl;
        //rp.print_resource_profiles();
    }
    if (verbose)
    {
        rp.print_resource_profiles();
    }
    return makespan;
}

unsigned int ScheduleGenerator::spt(Instance &instance)
{
    return 0;
}

std::vector<unsigned int> ScheduleGenerator::generate_precedence_list(Instance &instance)
{
    std::vector<unsigned int> ret(instance.n());
    std::iota(ret.begin(), ret.end(), 0);
    return ret;
}

} // namespace ScheduleMe


