//
// Created by Marcel Hündorf on 27.05.21.
//

#include "../include/neighborhoods.hpp"

namespace ScheduleMe
{

Neighborhoods::Neighborhoods(unsigned int seed) : mt_rand(seed)
{
}

bool Neighborhoods::swap(std::vector<unsigned int> &precedence_list, Instance& instance)
{
    unsigned int idx_a = 0;
    unsigned int idx_b = 0;
    while (idx_a == idx_b)
    {
        idx_a = static_cast<unsigned int>(mt_rand()) % (precedence_list.size() - 2) + 1;
        idx_b = static_cast<unsigned int>(mt_rand()) % (precedence_list.size() - 2) + 1;
    }

    if(swap_a_with_b_fast(precedence_list, idx_a, idx_b, instance))
    {
        /*if (!swap_a_with_b(precedence_list, idx_a, idx_b, instance)) {
            std::cout << "sad" << std::endl;
            return false;
        }*/
        unsigned int temp = precedence_list.at(idx_a);
        precedence_list.at(idx_a) = precedence_list.at(idx_b);
        precedence_list.at(idx_b) = temp;
        return true;
    }
    return false;
}

bool Neighborhoods::api(std::vector<unsigned int> &precedence_list, Instance& instance)
{
    unsigned int idx_a = 0;
    unsigned int idx_b = 0;
    idx_a = static_cast<unsigned int>(mt_rand()) % (precedence_list.size() - 3) + 1;
    idx_b = idx_a + 1;

    /*if(swap_a_with_b_fast(precedence_list, idx_a, idx_b, instance))*/
    if (!is_b_successor_of_a(instance, precedence_list.at(idx_a), precedence_list.at(idx_b)))
    {
        unsigned int temp = precedence_list.at(idx_a);
        precedence_list.at(idx_a) = precedence_list.at(idx_b);
        precedence_list.at(idx_b) = temp;
        return true;
    }
    return false;
}

bool Neighborhoods::shift(std::vector<unsigned int> &precedence_list, Instance& instance)
{
    unsigned int idx_a = 0;
    unsigned int idx_b = 0;
    while (idx_a == idx_b)
    {
        idx_a = static_cast<unsigned int>(mt_rand()) % (precedence_list.size() - 2) + 1;
        idx_b = static_cast<unsigned int>(mt_rand()) % (precedence_list.size() - 2) + 1;
    }

    if (idx_a > idx_b)
    {
        unsigned int tmp = idx_a;
        idx_a = idx_b;
        idx_b = tmp;
    }
    //unsigned int ac = idx_a;
    //unsigned int bc = idx_b;


    std::vector<unsigned int> to_check = std::vector<unsigned int>(precedence_list.begin() + idx_a, precedence_list.begin() + idx_b +1);
    std::sort(to_check.begin(), to_check.end());
    if (check_later_schedule(instance, to_check, precedence_list.at(idx_a))) {
        std::vector<unsigned int> copy = precedence_list;
        for (; idx_a < idx_b; idx_a++)
        {
            unsigned int tmp = copy.at(idx_a);
            copy.at(idx_a) = copy.at(idx_a + 1);
            copy.at(idx_a + 1) = tmp;
        }
        precedence_list = copy;
        return true;
    }
    return false;
}

bool Neighborhoods::random(std::vector<unsigned int> &precedence_list, Instance& instance)
{
    switch (static_cast<unsigned int>(mt_rand()) % 3)
    {
    case 0:
        return swap(precedence_list, instance);
    case 1:
        return api(precedence_list, instance);
    case 2:
        return shift(precedence_list, instance);
    default:
        return api(precedence_list, instance);
    }
}

bool Neighborhoods::swap_a_with_b(std::vector<unsigned int> &precedence_list, unsigned int idx_a, unsigned int idx_b, Instance &instance)
{
    std::vector<unsigned int> copy = precedence_list;
    unsigned int temp = copy.at(idx_a);
    copy.at(idx_a) = copy.at(idx_b);
    copy.at(idx_b) = temp;
    return check_precedence(copy, instance);
}

bool Neighborhoods::check_precedence(std::vector<unsigned int> &precedence_list, Instance &instance)
{
    for (unsigned int i = 0; i < precedence_list.size() - 1; i++)
    {
        for (unsigned int j = 0; j < instance.successors[precedence_list[i]].size(); j++)
        {
            if (! (std::find(precedence_list.begin() + i, precedence_list.end(), instance.successors[precedence_list[i]][j]) != precedence_list.end()))
            {
                return false;
            }
        }
    }
    return true;
}

// void Neighborhoods::swap(std::vector<unsigned int> &precedence_list, Instance& instance) {

//     unsigned int idx_a = 0;
//     unsigned int idx_b = 0;
//     do {
//         while (idx_a == idx_b)
//         {
//             idx_a = static_cast<unsigned int>(rand()) % (precedence_list.size());
//             idx_b = static_cast<unsigned int>(rand()) % (precedence_list.size());
//         }
//     } while (!swap_a_with_b(precedence_list, idx_a, idx_b, instance));

//     unsigned int temp = precedence_list.at(idx_a);
//     precedence_list.at(idx_a) = precedence_list.at(idx_b);
//     precedence_list.at(idx_b) = temp;
// }

    bool Neighborhoods::swap_a_with_b_fast(std::vector<unsigned int> &precedence_list, unsigned int idx_a, unsigned int idx_b, Instance &instance) {
         // set a to be the idx that is scheduled first and b to the last one
         unsigned int temp = idx_a;
         idx_a = idx_a < idx_b ? idx_a : idx_b;
         idx_b = temp < idx_b ? idx_b : temp;

         /* a is scheduled first so to schedule it later there should be no problem
         only b has to be proven to maintain the priorities (only activities between a and b has to be checked)

          to do so sort the activities to be  checked and the predecessors of idx_b to then campare if they have any
          element in common
          */
         std::vector<unsigned int> to_check = std::vector<unsigned int>(precedence_list.begin() + idx_a, precedence_list.begin() + idx_b);
         std::sort(to_check.begin(), to_check.end());

        if (check_earlier_schedule(instance, to_check, precedence_list.at(idx_b)))
            return check_later_schedule(instance, to_check, precedence_list.at(idx_a));
        return false;
     }

    bool Neighborhoods::check_earlier_schedule(const Instance& instance, const std::vector<unsigned int> &precedence_list, unsigned int activity) {
        unsigned int ptr1 = 0;
        unsigned int ptr2 = 0;

        while (ptr1 < precedence_list.size() && ptr2 < instance.predecessors_full.at(activity).size()) {

            if (precedence_list.at(ptr1) == instance.predecessors_full.at(activity).at(ptr2)) {
                return false;
            }
            else if (precedence_list.at(ptr1) < instance.predecessors_full.at(activity).at(ptr2)) {
                ptr1++;
            } else {
                ptr2++;
            }
        }

        return true;
    }

    bool Neighborhoods::check_later_schedule(const Instance& instance, const std::vector<unsigned int> &precedence_list, unsigned int activity) {
        unsigned int ptr1 = 0;
        unsigned int ptr2 = 0;

        while (ptr1 < precedence_list.size() && ptr2 < instance.successors_full.at(activity).size()) {

            if (precedence_list.at(ptr1) == instance.successors_full.at(activity).at(ptr2)) {
                return false;
            }
            else if (precedence_list.at(ptr1) < instance.successors_full.at(activity).at(ptr2)) {
                ptr1++;
            } else {
                ptr2++;
            }
        }
        return true;
    }

    bool Neighborhoods::is_b_successor_of_a(const Instance &instance, unsigned int activity_a, unsigned int activity_b) {
        for (unsigned int i : instance.successors_full.at(activity_a)) {
            if (activity_b == i)
                return true;
        }
        return false;
    }

} // namespace ScheduleMe
