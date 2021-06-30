//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.hpp"
#include <numeric>
#include <stack>

namespace ScheduleMe
{

unsigned int
ScheduleGenerator::earliest_start_schedule(Instance &instance, const std::vector<unsigned int> &act_list, bool verbose)
{
    ResourceProfile rp(instance);
    instance.start_time = std::vector<unsigned int>(instance.n(), 0);

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
            t = rp.get_next_jump_point(t + 1);
        }
        rp.schedule_at(t, j);
        instance.start_time[j] = t;

        //std::cout << "\n\n--------------------------------------------------------" << std::endl;
        //std::cout << "Added " << j << " at " << t << " p_i: " << instance.processing_time[j] <<  " r_i: " << instance.demands[j][0] << ", " << instance.demands[j][1] << ", " << instance.demands[j][2] << ", "<< instance.demands[j][3] << ", "<< std::endl;
        //rp.print_resource_profiles();
    }
    if (verbose)
    {
        rp.print_resource_profiles();
    }
    return instance.start_time[instance.n() - 1];
}

unsigned int ScheduleGenerator::spt(Instance &instance)
{
    return 0;
}

std::vector<unsigned int> ScheduleGenerator::generate_precedence_list(Instance &instance)
{
    return topological_sort(instance);
}

std::vector<unsigned int> ScheduleGenerator::topological_sort(Instance &instance)
{
    std::stack<unsigned int> stack;

    bool* visited = new bool[instance.n()];
    for (unsigned int i = 0; i < instance.n(); i++)
    {
        visited[i] = false;
    }

    for (unsigned int i = 0; i < instance.n(); i++)
    {
        if (!visited[i])
        {
            sort_util(instance, i, visited, stack);
        }
    }

    std::vector<unsigned int> sol;
    while (!stack.empty())
    {
        sol.push_back(stack.top());
        stack.pop();
    }
    return sol;
}

void ScheduleGenerator::sort_util(Instance &instance, unsigned int v, bool visited[], std::stack<unsigned int>& stack)
{
    visited[v] = true;

    std::vector<unsigned int>::iterator i;
    for (i = instance.successors[v].begin(); i != instance.successors[v].end(); ++i)
    {
        if (!visited[*i])
        {
            sort_util(instance, *i, visited, stack);
        }
    }
    stack.push(v);
}

} // namespace ScheduleMe


