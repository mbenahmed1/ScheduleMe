//
// Created by Marcel Hündorf on 27.05.21.
//

#ifndef SCHEDULER_NEIGHBORHOODS_HPP
#define SCHEDULER_NEIGHBORHOODS_HPP

#include <vector>
#include <algorithm>
#include "instance.hpp"

class Neighborhoods {
    static bool swap_a_with_b(std::vector<unsigned int> &precedenceList, unsigned int idx_a, unsigned int idx_b, ScheduleMe::Instance& instance);

public:
    Neighborhoods(unsigned int seed);

    std::vector<unsigned int> swap(std::vector<unsigned int> precedenceList, ScheduleMe::Instance& instance);
};



#endif //SCHEDULER_NEIGHBORHOODS_HPP
