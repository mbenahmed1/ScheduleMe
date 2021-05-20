//
// Created by tim on 18.05.21.
//

#ifndef SCHEDULEME_SCHEDULE_GENERATOR_H
#define SCHEDULEME_SCHEDULE_GENERATOR_H

#include <vector>
#include <iostream>
#include "../../uosrcp/include/uosrcp/instance.h"
using namespace uosrcp;

class ScheduleGenerator
{
public:
    std::vector<unsigned int> serial_sgs(Instance &instance);

};


#endif //SCHEDULEME_SCHEDULE_GENERATOR_H
