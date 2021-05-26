//
// Created by tim on 18.05.21.
//

#ifndef SCHEDULEME_SCHEDULE_GENERATOR_H
#define SCHEDULEME_SCHEDULE_GENERATOR_H

#include "instance.hpp"
#include "resource_profile.hpp"
#include <iostream>
#include <algorithm>

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
    static void earliest_start_schedule(Instance &instance, const std::vector<unsigned int> &act_list);

    static void generate_precedence_list(Instance &instance, std::vector<unsigned int> &list);

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

    static void generate_precedence_list_rec(Instance &instance, std::vector<unsigned int> &list, unsigned int act);
};

} // namespace ScheduleMe

#endif //SCHEDULEME_SCHEDULE_GENERATOR_H