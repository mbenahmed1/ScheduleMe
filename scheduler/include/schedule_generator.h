//
// Created by tim on 18.05.21.
//

#ifndef SCHEDULEME_SCHEDULE_GENERATOR_H
#define SCHEDULEME_SCHEDULE_GENERATOR_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "../../uosrcp/include/uosrcp/instance.h"
using namespace uosrcp;

class ScheduleGenerator
{
public:
    std::vector<unsigned int> serial_sgs(Instance &instance);

private:
    /**
     * Finds the activity with the shortest processing time.
     * Therefore it looks through the processing time of all activities and
     * returns the index of the activity with minimum processing time.
     * 
     * @param instance is the instance to look for the shortest activity.
     * @return the index of the shortest activity.
     */ 
    unsigned int spt(Instance &instance);
};


#endif //SCHEDULEME_SCHEDULE_GENERATOR_H
