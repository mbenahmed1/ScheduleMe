//
// Created by tim on 18.05.21.
//
#include "../include/schedule_generator.h"

std::vector<unsigned int> ScheduleGenerator::sgs(Instance &instance) {

    for (unsigned long i = 0; i < instance.n(); i++)
    {
        for (unsigned long j = 0; j < instance.n(); j++)
        {
            std::cout << i << " -> " << j << ": " << instance.successors[i][j] << std::endl;
        }
    }
    return std::vector<unsigned int>();
}
