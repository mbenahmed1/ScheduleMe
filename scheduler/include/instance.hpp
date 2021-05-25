#pragma once

#ifndef UOSRCP_INSTANCE_H
#define UOSRCP_INSTANCE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


namespace ScheduleMe {

using Demand = unsigned int;
using Time = unsigned int;

struct Instance {
    Instance() = default;

    std::vector<unsigned int> resources;

    std::vector<unsigned int> processing_time;

    std::vector<unsigned int> start_time;

    std::vector<std::vector<unsigned int>> rsc_profile; 

    std::vector<std::vector<unsigned int>> demands;

    std::vector<std::vector<unsigned int>> successors;

    std::vector<std::vector<unsigned int>> predecessors;

    std::size_t n() const { return processing_time.size(); }
    
    std::size_t r() const { return resources.size(); }

    void write_plotable(std::string path)
    {
        std::ofstream s(path);
        if (s.is_open())
        {
            s << start_time.size() << ' ' << resources.size() << std::endl;

            for (unsigned int i = 0; i < resources.size(); i++)
            {
                s << resources[i] << ' ';
            }
            s << std::endl;

            for (unsigned int i = 0; i < start_time.size(); i++)
            {
                s << i << ' ' << start_time[i] << ' ' << processing_time[i] << ' ';
                for (unsigned int j = 0; j < resources.size(); j++)
                {
                    s << demands[i][j] << ' ';
                } 
                s << std::endl;
            }
        }
        else
        {
            std::cout << "Could not write plotable." << std::endl;
        }
        s.close();
    }
};


} // namespace uosrcp

#endif
