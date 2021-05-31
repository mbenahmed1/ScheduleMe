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

    /**
     * checks, if the idx a can be swapped with idx b of the precedence list in a fast way
     *
     * @param idx_a the first index of the precedence list to be swapped
     * @param idx_b the second index of the precedence list to be swapped
     * @return true if there are no conflicts, false else
     */
    static bool swap_a_with_b_fast(std::vector<unsigned int> &precedence_list, unsigned int idx_a, unsigned int idx_b, Instance& instance);

    /**
     * checks, if the activity can be done earlier
     *
     * @param instance the instance to be checked for
     * @param precedence_list_part the part of the precedence list, that is in between the old and the new idx of the
     *                             precedence list. Must be in sorted order!!!
     * @param activity the idx of the activity
     * @return true if there are no conflicts, false else
     */
    static bool check_earlier_schedule(const Instance& instance, const std::vector<unsigned int> &precedence_list_part, unsigned int activity);

    /**
     * checks, if the activity can be done later
     *
     * @param instance the instance to be checked for
     * @param precedence_list_part the part of the precedence list, that is in between the old and the new idx of the
     *                             precedence list. Must be in sorted order!!!
     * @param activity the idx of the activity
     * @return true if there are no conflicts, false else
     */
    static bool check_later_schedule(const Instance& instance, const std::vector<unsigned int> &precedence_list_part, unsigned int activity);

    static bool is_b_successor_of_a(const Instance& instance, unsigned int activity_a, unsigned int activity_b);




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
