#pragma once

#ifndef UOSRCP_INSTANCE_H
#define UOSRCP_INSTANCE_H

#include <string>
#include <vector>


namespace ScheduleMe {

using Demand = unsigned int;
using Time = unsigned int;

struct Instance {
    Instance() = default;

    std::vector<unsigned int> resources;

    std::vector<unsigned int> processing_time;

    std::vector<unsigned int> start_time;

    std::vector<std::vector<unsigned int>> demands;

    std::vector<std::vector<unsigned int>> successors;

    std::vector<std::vector<unsigned int>> predecessors;

    std::size_t n() const { return processing_time.size(); }
    std::size_t r() const { return resources.size(); }
};


} // namespace uosrcp

#endif
