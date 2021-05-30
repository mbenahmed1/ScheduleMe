//
// Created by Marcel Hündorf on 27.05.21.
//

#ifndef SCHEDULER_NEIGHBORHOODS_HPP
#define SCHEDULER_NEIGHBORHOODS_HPP

#include <vector>
#include <random>
#include <algorithm>
#include "instance.hpp"

namespace ScheduleMe {

class Neighborhoods {
    static bool swap_a_with_b(std::vector<unsigned int> &precedence_list, unsigned int idx_a, unsigned int idx_b, Instance& instance);

    std::mt19937 mt_rand;

    std::uniform_real_distribution<double> dis;

public:
    Neighborhoods(unsigned int seed);

    bool swap(std::vector<unsigned int> &precedence_list, Instance& instance);

    bool api(std::vector<unsigned int> &precedence_list, Instance& instance);

    bool shift(std::vector<unsigned int> &precedence_list, Instance& instance);

    bool random(std::vector<unsigned int> &precedence_list, Instance& instance);

    static bool check_precedence(std::vector<unsigned int> &precedence_list, Instance &instance);

};


} // namespace ScheduleMe

#endif //SCHEDULER_NEIGHBORHOODS_HPP
