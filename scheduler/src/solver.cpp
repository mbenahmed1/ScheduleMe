//
// Created by tim on 18.05.21.
//

#include "../include/solver.h"
#include "../include/schedule_generator.h"
#include "../../uosrcp/include/uosrcp/instance.h"
#include "../../uosrcp/include/uosrcp/io.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Solver" << std::endl;

    uosrcp::Instance instance = uosrcp::read_instance("../../instances/j30/J301_1.RCP");

    ScheduleGenerator sgs;

    sgs.serial_sgs(instance);
}