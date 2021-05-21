//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.h"

std::vector<unsigned int> ScheduleGenerator::serial_sgs(Instance &instance) {

    std::vector<unsigned int> E_1;

    for (unsigned long i = 0; i < instance.n(); i++)
    {
        if (instance.predecessors[i].size() < 1)
        {
            E_1.push_back(i);
        }
    }

    unsigned int j;
    unsigned int t;
    while(true)
    {
        j = spt(instance);
        
        t = 0;
        // for (unsigned long i = 0; i < instance.predecessors[j].size(); i++)
        // {
        //     if ()
        // }

        if (true)
        {
            break;
        }
    }

    return std::vector<unsigned int>();
}

unsigned int ScheduleGenerator::spt(Instance &instance)
{   
    std::vector<unsigned int>::iterator result = std::min_element(instance.processing_time.begin(), instance.processing_time.end());
    return std::distance(instance.processing_time.begin(), result);
}
