//
// Created by Marcel Hündorf on 27.05.21.
//

#include "../include/neighborhoods.hpp"
#include "../include/resource_profile.hpp"

Neighborhoods::Neighborhoods(unsigned int seed) {
    std::srand(seed);
}

std::vector<unsigned int> Neighborhoods::swap(std::vector<unsigned int> precedenceList, ScheduleMe::Instance& instance) {

    unsigned int idx_a;
    unsigned int idx_b;
    do {
        idx_a = static_cast<unsigned int>(rand() % (precedenceList.size()));
        idx_b = static_cast<unsigned int>(rand() % (precedenceList.size() - 1));
        if (idx_a == idx_b) {
            idx_b++;
        }
    } while (!swap_a_with_b(precedenceList, idx_a, idx_b, instance));

    unsigned int temp = precedenceList.at(idx_a);
    precedenceList.at(idx_a) = precedenceList.at(idx_b);
    precedenceList.at(idx_b) = temp;
    return precedenceList;

}

bool Neighborhoods::swap_a_with_b(std::vector<unsigned int> &precedenceList, unsigned int idx_a, unsigned int idx_b,
                             ScheduleMe::Instance& instance) {
    // set a to be the idx that is scheduled first and b to the last one
    unsigned int temp = idx_a;
    idx_a = idx_a < idx_b ? idx_a : idx_b;
    idx_b = temp < idx_b ? idx_b : temp;

    /* a is scheduled first so to schedule it later there should be no problem
    only b has to be proven to maintain the priorities (only activities between a and b has to be checked)

     to do so sort the activities to be  checked and the predecessors of idx_b to then campare if they have any
     element in common
     */
    std::vector<unsigned int> now_predecessors = std::vector<unsigned int>(precedenceList.begin() + idx_a + 1, precedenceList.begin() + idx_b);
    std::sort(now_predecessors.begin(), now_predecessors.end());

    unsigned int ptr1 = 0;
    unsigned int ptr2 = 0;
    bool isOk = true;
    while (ptr1 < now_predecessors.size() && ptr2 < instance.predecessors_full.at(idx_b).size()) {
        // std::cout << "ptr1: " << ptr1 << "\t";
        // std::cout << "ptr2: " << ptr2 << std::endl;
        if (now_predecessors.at(ptr1) == instance.predecessors_full.at(idx_b).at(ptr2)) {
            isOk = false;
            break;
        }
        else if (now_predecessors.at(ptr1) < instance.predecessors_full.at(idx_b).at(ptr2)) {
            ptr1++;
        } else {
            ptr2++;
        }
    }
    return isOk;
}
