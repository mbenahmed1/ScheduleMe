//
// Created by tim on 18.05.21.
//

#include "../include/io.hpp"
#include "../include/solver.hpp"
#include "../include/schedule_generator.hpp"
#include "../include/resource_profile.hpp"

#include <iostream>

using namespace ScheduleMe;

int main(int argc, char** argv)
{
    std::cout << "Solver" << std::endl;

    Instance instance = read_instance("../../instances/j30/J301_1.RCP");

    std::vector<unsigned int> precedence_list;

    ScheduleGenerator::generate_precedence_list(instance, precedence_list);

    ScheduleGenerator::earliest_start_schedule(instance, precedence_list);

    ScheduleMe::write_solution(instance.start_time, "./test.sol");

    instance.write_plotable("./test.lol");

}

