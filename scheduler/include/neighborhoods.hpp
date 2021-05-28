//
// Created by Marcel Hündorf on 27.05.21.
//

#ifndef SCHEDULER_NEIGHBORHOODS_HPP
#define SCHEDULER_NEIGHBORHOODS_HPP

#include <vector>
#include <algorithm>
#include "instance.hpp"

namespace ScheduleMe {

class Neighborhoods {
    static bool swap_a_with_b(std::vector<unsigned int> &precedenceList, unsigned int idx_a, unsigned int idx_b, ScheduleMe::Instance& instance);


public:
    Neighborhoods(unsigned int seed);

    void swap(std::vector<unsigned int> &precedenceList, ScheduleMe::Instance& instance);

    void shift(std::vector<unsigned int> &precedenceList, ScheduleMe::Instance& instance);

    void api(std::vector<unsigned int> &precedenceList, Instance& instance);

    static bool checkPrecedence(std::vector<unsigned int> &precedenceList, Instance &instance);

};


} // namespace ScheduleMe

#endif //SCHEDULER_NEIGHBORHOODS_HPP
