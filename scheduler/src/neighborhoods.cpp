//
// Created by Marcel Hündorf on 27.05.21.
//

#include "../include/neighborhoods.hpp"
#include "../include/resource_profile.hpp"

namespace ScheduleMe
{

Neighborhoods::Neighborhoods(unsigned int seed) {
    std::srand(seed);
}

void Neighborhoods::swap(std::vector<unsigned int> &precedenceList, Instance& instance)
{
    // if(precedenceList.at(28) == 28)
    // {
    //     precedenceList.at(28) = 29;
    //     precedenceList.at(29) = 28;
    // }
    // else
    // {
    //     precedenceList.at(28) = 28;
    //     precedenceList.at(29) = 29;
    // }
    // return;

    unsigned int idx_a = 0;
    unsigned int idx_b = 0;
    while (idx_a == idx_b)
    {
        idx_a = static_cast<unsigned int>(rand()) % (precedenceList.size() - 2) + 1;
        idx_b = static_cast<unsigned int>(rand()) % (precedenceList.size() - 2) + 1;
    }

    if(swap_a_with_b(precedenceList, idx_a, idx_b, instance))
    {
        unsigned int temp = precedenceList.at(idx_a);
        precedenceList.at(idx_a) = precedenceList.at(idx_b);
        precedenceList.at(idx_b) = temp;
    }

}

void Neighborhoods::shift(std::vector<unsigned int> &precedenceList, Instance& instance)
{

}

void Neighborhoods::api(std::vector<unsigned int> &precedenceList, Instance& instance)
{

}

// void Neighborhoods::swap(std::vector<unsigned int> &precedenceList, Instance& instance) {

//     unsigned int idx_a = 0;
//     unsigned int idx_b = 0;
//     do {
//         while (idx_a == idx_b)
//         {
//             idx_a = static_cast<unsigned int>(rand()) % (precedenceList.size());
//             idx_b = static_cast<unsigned int>(rand()) % (precedenceList.size());
//         }
//     } while (!swap_a_with_b(precedenceList, idx_a, idx_b, instance));

//     unsigned int temp = precedenceList.at(idx_a);
//     precedenceList.at(idx_a) = precedenceList.at(idx_b);
//     precedenceList.at(idx_b) = temp;

// }

bool Neighborhoods::swap_a_with_b(std::vector<unsigned int> &precedenceList, unsigned int idx_a, unsigned int idx_b, Instance &instance)
{
    std::vector<unsigned int> copy = precedenceList;
    unsigned int temp = copy.at(idx_a);
    copy.at(idx_a) = copy.at(idx_b);
    copy.at(idx_b) = temp;
    return checkPrecedence(copy, instance);
}

bool Neighborhoods::checkPrecedence(std::vector<unsigned int> &precedenceList, Instance &instance)
{
    for (unsigned int i = 0; i < precedenceList.size() - 1; i++)
    {
        for (unsigned int j = 0; j < instance.successors[precedenceList[i]].size(); j++)
        {
            if (! (std::find(precedenceList.begin() + i, precedenceList.end(), instance.successors[precedenceList[i]][j]) != precedenceList.end()))
            {
                return false;
            }
        }
    }
    return true;
}

// bool Neighborhoods::swap_a_with_b(std::vector<unsigned int> &precedenceList, unsigned int idx_a, unsigned int idx_b,
//                              ScheduleMe::Instance& instance) {
//     // set a to be the idx that is scheduled first and b to the last one
//     unsigned int temp = idx_a;
//     idx_a = idx_a < idx_b ? idx_a : idx_b;
//     idx_b = temp < idx_b ? idx_b : temp;

//     /* a is scheduled first so to schedule it later there should be no problem
//     only b has to be proven to maintain the priorities (only activities between a and b has to be checked)

//      to do so sort the activities to be  checked and the predecessors of idx_b to then campare if they have any
//      element in common
//      */
//     std::vector<unsigned int> now_predecessors = std::vector<unsigned int>(precedenceList.begin() + idx_a + 1, precedenceList.begin() + idx_b);
//     std::sort(now_predecessors.begin(), now_predecessors.end());

//     unsigned int ptr1 = 0;
//     unsigned int ptr2 = 0;
//     bool isOk = true;
//     while (ptr1 < now_predecessors.size() && ptr2 < instance.predecessors_full.at(idx_b).size()) {
//         // std::cout << "ptr1: " << ptr1 << "\t";
//         // std::cout << "ptr2: " << ptr2 << std::endl;
//         if (now_predecessors.at(ptr1) == instance.predecessors_full.at(idx_b).at(ptr2)) {
//             isOk = false;
//             break;
//         }
//         else if (now_predecessors.at(ptr1) < instance.predecessors_full.at(idx_b).at(ptr2)) {
//             ptr1++;
//         } else {
//             ptr2++;
//         }
//     }
//     return isOk;
// }



} // namespace ScheduleMe

