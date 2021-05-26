//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.hpp"
#include <numeric>

namespace ScheduleMe
{

void ScheduleGenerator::earliest_start_schedule(Instance &instance, const std::vector<unsigned int> &act_list)
{
    ResourceProfile rp(instance);

    for (unsigned int lambda = 0; lambda < act_list.size(); lambda++)
    {
        unsigned int j = act_list[lambda];
        unsigned int t = 0;

        // calculate start time for j
        for (unsigned int i = 0; i < instance.predecessors[j].size(); i++)
        {
            unsigned int sum = instance.start_time[instance.predecessors[j][i]] +
                               instance.processing_time[instance.predecessors[j][i]];
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
        //std::cout << "\n\n--------------------------------------------------------" << std::endl;
        //std::cout << "Added " << j << " at " << t << " p_i: " << instance.processing_time[j] <<  " r_i: " << instance.demands[j][0] << ", " << instance.demands[j][1] << ", " << instance.demands[j][2] << ", "<< instance.demands[j][3] << ", "<< std::endl;
        //rp.print_resource_profiles();
    }

    rp.print_resource_profiles();
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


