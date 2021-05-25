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

    ScheduleGenerator::earliest_start_schedule(instance, std::vector<unsigned int>(20));

    ResourceProfile rsc(instance);

    rsc.schedule_at(0, 1);
    rsc.schedule_at(9, 2);

    rsc.print_resource_profiles();


}

