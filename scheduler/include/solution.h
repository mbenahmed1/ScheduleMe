//
// Created by Marcel Hündorf on 18.05.21.
//

#ifndef UOSRCP_SOLUTION_H
#define UOSRCP_SOLUTION_H

#include <string>
#include <vector>
#include <uosrcp/instance.h>
#include <memory>

namespace uosrcp {

    class Solution {
        std::shared_ptr<Instance> instance_ptr;
        std::vector<unsigned int> start_times;

    public:
        Solution(std::shared_ptr<Instance> instance);
        Solution(Solution& solution);
        void write(const std::string& filename) const;
        bool init() const;

    };



} // namespace uosrcp

#endif //UOSRCP_SOLUTION_H
