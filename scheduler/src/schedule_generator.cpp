//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.h"

std::vector<unsigned int> ScheduleGenerator::serial_sgs(Instance &instance) {

    int index = 0;
    for (unsigned long i = 0; i < instance.n(); i++)
    {
        for (unsigned long j = 0; j < instance.successors[i].size(); j++)
        {
            std::cout << i << " -> " << j << ": " << instance.successors[i][j] + 1 << std::endl;
        } 
        
    }
    return std::vector<unsigned int>();
}
