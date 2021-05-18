//
// Created by Marcel Hündorf on 18.05.21.
//

#include <uosrcp/io.h>

#include <fstream>
#include "../include/solution.h"

uosrcp::Solution::Solution(std::shared_ptr<Instance> instance) :
instance_ptr(instance),
start_times(instance->n(), 0){
}

void uosrcp::Solution::write(const std::string &filename) const {
    uosrcp::write_solution(start_times, filename);
}

uosrcp::Solution::Solution(uosrcp::Solution &solution) :
instance_ptr(solution.instance_ptr),
start_times(solution.start_times) {
}

bool uosrcp::Solution::init() const {
    // TODO: generate first Solution here
    return 1 /*can a solution be found?*/ ? true : false;
}

