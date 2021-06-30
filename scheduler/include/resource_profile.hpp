//
// Created by tim on 25.05.21.
//

#ifndef SCHEDULER_RESOURCE_PROFILE_HPP
#define SCHEDULER_RESOURCE_PROFILE_HPP

#include "instance.hpp"
#include <iostream>
#include <vector>

namespace ScheduleMe
{

class ResourceProfile
{
private:

    Instance &instance;

    std::vector<std::vector<unsigned int>> profiles;

    std::vector<bool> jump_points;

public:

    /**
     * Constructor.
     *
     * @param instance reference to the instance for which the resource profiles should be set up
     */
    ResourceProfile(Instance &instance);

    unsigned int get_next_jump_point(unsigned int time);
    
    /**
     * Calculates the available capacity.
     *
     * @param time time at which the capacity should be evaluated
     * @param resource resource index for which the capacity should be calculated
     * @return the free resource capacity at the given time
     */
    unsigned int get_available_capacity(unsigned int time, unsigned int resource) const;

    /**
     * Calculates how many resources are used up.
     *
     * @param time time at which the capacity should be evaluated
     * @param resource resource index for which the capacity should be calculated
     * @return the already used capacity at the given time
     */
    unsigned int get_used_capacity(unsigned int time, unsigned int resource) const;

    /**
     * Determines whether the given activity can be scheduled at the given time
     * by checking all resource constraints.
     *
     * @param time start time at which the activity should be scheduled
     * @param activity activity index
     * @return true if the activity satisfies all resource constraints, else false
     */
    bool is_schedulable(unsigned int time, unsigned int activity) const;

    /**
     * Schedules the activity at the given time.
     *
     * @param time start time at which the activity should be scheduled
     * @param activity activity index
     */
    void schedule_at(unsigned int time, unsigned int activity);

    /**
     * Displays all resource profiles on stdout.
     */
    void print_resource_profiles() const;

};

}

#endif //SCHEDULER_RESOURCE_PROFILE_HPP
