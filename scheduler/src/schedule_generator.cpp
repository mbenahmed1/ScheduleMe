//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.hpp"

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
    }

    rp.print_resource_profiles();
}

unsigned int ScheduleGenerator::spt(Instance &instance)
{
    return 0;
}

void ScheduleGenerator::generate_precedence_list(Instance &instance, std::vector<unsigned int> &list)
{
    for (unsigned int i = 0; i < instance.n(); i++)
    {
        if (instance.predecessors[i].empty())
        {
            generate_precedence_list_rec(instance, list, i);
        }
    }
}

void ScheduleGenerator::generate_precedence_list_rec(Instance &instance, std::vector<unsigned int> &list, unsigned int act)
{
    if (std::find(list.begin(), list.end(), act) == list.end())
    {
        list.push_back(act);

        for (unsigned int i = 0; i < instance.successors[act].size(); i++)
        {
            generate_precedence_list_rec(instance, list, instance.successors[act][i]);
        }
    }
}


} // namespace ScheduleMe


