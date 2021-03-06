#pragma once

#ifndef UOSRCP_INSTANCE_H
#define UOSRCP_INSTANCE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

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

    std::vector<std::vector<unsigned int>> predecessors_full;

    std::vector<std::vector<unsigned int>> successors_full;

    std::vector<std::vector<unsigned int>> predecessors;

    std::size_t n() const { return processing_time.size(); }
    
    std::size_t r() const { return resources.size(); }

    void predecessors_full_rec(std::vector<unsigned int> predecessor_list, unsigned int index)
    {
        if (predecessors_full[index].size() != 0)
        {
            for (unsigned int i = 0; i < predecessor_list.size(); i++)
            {
                if (! (std::find(predecessors_full[index].begin(), predecessors_full[index].end(), predecessor_list[i]) != predecessors_full[index].end()) )
                {
                    predecessors_full[index].push_back(predecessor_list[i]);
                }
            }
        }
        else
        {
            predecessors_full[index] = predecessor_list;
        }

        predecessor_list.push_back(index);

        for (unsigned int i = 0; i < successors[index].size(); i++)
        {
            predecessors_full_rec(predecessor_list, successors[index][i]);
        }
    }

    void successors_full_rec(std::vector<unsigned int> successor_list, unsigned int index)
    {
        if (successors_full[index].size() != 0)
        {
            for (unsigned int i = 0; i < successor_list.size(); i++)
            {
                if (! (std::find(successors_full[index].begin(), successors_full[index].end(), successor_list[i]) != successors_full[index].end()) )
                {
                    successors_full[index].push_back(successor_list[i]);
                }
            }
        }
        else
        {
            successors_full[index] = successor_list;
        }

        successor_list.push_back(index);

        for (unsigned int i = 0; i < predecessors[index].size(); i++)
        {
            successors_full_rec(successor_list, predecessors[index][i]);
        }
    }

    void write_plotable(std::string path)
    {   
        path = path + ".plot";
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
