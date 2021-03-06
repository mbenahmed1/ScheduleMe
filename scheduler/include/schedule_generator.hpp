//
// Created by tim on 18.05.21.
//

#ifndef SCHEDULEME_SCHEDULE_GENERATOR_H
#define SCHEDULEME_SCHEDULE_GENERATOR_H

#include "instance.hpp"
#include "resource_profile.hpp"
#include <iostream>
#include <algorithm>
#include <stack>

namespace ScheduleMe
{

class ScheduleGenerator
{
public:

    /**
     * Calculates an earliest start schedule from a given instance.
     * 
     * @param instance reference to the instance.
     */
    static unsigned int
    earliest_start_schedule(Instance &instance, const std::vector<unsigned int> &act_list, bool verbose = false);


    static std::vector<unsigned int> generate_precedence_list(Instance &instance);

    /**
     * Performs topological sort on given instance.
     * @param instance reference to the instance.
     * @return sorted vector
     */
    static std::vector<unsigned int> topological_sort(Instance &instance);

    /**
     * Util function that gets called by the topological sort function.
     */
    static void sort_util(Instance &instance, unsigned int v, bool visited[], std::stack<unsigned int>& stack);

private:
    /**
     * Finds the activity with the shortest processing time.
     * Therefore it looks through the processing time of all activities and
     * returns the index of the activity with minimum processing time.
     * 
     * @param instance is the instance to look for the shortest activity.
     * @return the index of the shortest activity.
     */
    static unsigned int spt(Instance &instance);

};

} // namespace ScheduleMe

#endif //SCHEDULEME_SCHEDULE_GENERATOR_H