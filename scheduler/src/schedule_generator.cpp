//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.hpp"

namespace ScheduleMe
{

void ScheduleGenerator::earliest_start_schedule(Instance &instance, const std::vector<unsigned int> &act_list)
{
    for (unsigned int lambda = 0; lambda < act_list.size(); lambda++)
    {
        unsigned int j = act_list[lambda];
        unsigned int t = 0;

        for (unsigned int i = 0; i < instance.predecessors[j].size(); i++)
        {   
            unsigned int sum = instance.start_time[instance.predecessors[j][i]] + instance.processing_time[instance.predecessors[j][i]];
            if (sum > t)
            {
                t = sum;
            }
        }

        // for (unsigned int i = 0; i < instance.r(); i++)
        // {
        //     instance.rsc_profile[i].push_back(instance.resources[i]);
        // }

        if (t >= instance.rsc_profile[0].size())
        {
            for (unsigned int i = 0; i < instance.rsc_profile.size(); i++)
            {
                for (unsigned int k = 0; k < instance.processing_time[j]; k++)
                {
                    instance.rsc_profile[i].push_back(instance.resources[i]);
                }
            }
        } 
        else
        {
            // do something
        }
    }
}
    
} // namespace ScheduleMe


