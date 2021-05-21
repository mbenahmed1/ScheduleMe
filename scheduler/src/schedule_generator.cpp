//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.h"

std::vector<unsigned int> ScheduleGenerator::serial_sgs(Instance &instance) {

    for (unsigned long i = 0; i < instance.n(); i++)
    {
        for (unsigned long j = 0; j < instance.predecessors[i].size(); j++)
        {
            std::cout << "Predecessors of " << i + 1 << ": " << instance.predecessors[i][j] + 1 << std::endl;

        } 
        
    }
    return std::vector<unsigned int>();
}
