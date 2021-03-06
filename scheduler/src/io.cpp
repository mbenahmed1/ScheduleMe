#include "../include/io.hpp"

#include <fstream>
// #include <algorithm>

namespace ScheduleMe {

static void is_good_or_throw(std::istream& is, const char* msg)
{
    if (!is.good()) {
        throw Io_error(msg);
    }
}


Instance read_instance(std::istream& is)
{
    is_good_or_throw(is, "bad stream prior to read");

    auto instance = Instance();

    unsigned int n = 0;
    unsigned int r = 0;
    is >> n >> r;

    if (n == 0) {
        throw Io_error("n = 0");
    }

    is_good_or_throw(is, "bad stream after reading #jobs and #resources");

    instance.resources.resize(r, 0);

    for (unsigned int i = 0; i < r; i++) {
        is >> instance.resources[i];
        is_good_or_throw(is, "bad stream after reading resource availability");
    }
    
    instance.rsc_profile.resize(r);
    
    instance.processing_time.resize(n);
    instance.demands.resize(n);
    instance.successors.resize(n);

    for (unsigned int j = 0; j < n; j++) {

        is >> instance.processing_time[j];
        is_good_or_throw(is, "bad stream after reading processing time");

        auto& demands = instance.demands[j];
        demands.resize(r);

        for (std::size_t i = 0; i < r; i++) {

            is >> demands[i];
            is_good_or_throw(is, "bad stream after reading resource demand of job");
        }

        unsigned int nsucc = 0;
        is >> nsucc;
        is_good_or_throw(is, "bad stream after reading #successors");

        auto& succs = instance.successors[j];
        succs.resize(nsucc, 0);

        for (unsigned int i = 0; i < nsucc; i++) {

            is >> succs[i];
            succs[i]--;
            is_good_or_throw(is, "bad stream after reading successor");
        }
    }

    instance.predecessors.resize(n);
    for (unsigned long i = 0; i < instance.n(); i++)
    {
        for (unsigned long j = 0; j < instance.successors[i].size(); j++)
        {
            instance.predecessors[instance.successors[i][j]].push_back(i);
        }
    }

    instance.start_time = std::vector<unsigned int>(n, 0);

    instance.predecessors_full.resize(instance.n());
    instance.predecessors_full_rec(std::vector<unsigned int>(), 0);

    instance.successors_full.resize(instance.n());
    instance.successors_full_rec(std::vector<unsigned int>(), instance.n()-1);

    for (unsigned int i = 0; i < instance.n(); i++)
    {
        std::sort(instance.predecessors_full[i].begin(), instance.predecessors_full[i].end());
        std::sort(instance.successors_full[i].begin(), instance.successors_full[i].end());
    }

    /*for (int i = 0; i < instance.predecessors_full.size(); ++i) {
        std::cout << i << ": ";
        for (int j = 0; j < instance.predecessors_full.at(i).size(); ++j) {
            std::cout << instance.predecessors_full[i][j] << ", ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < instance.successors_full.size(); ++i) {
        std::cout << i << ": ";
        for (int j = 0; j < instance.successors_full.at(i).size(); ++j) {
            std::cout << instance.successors_full[i][j] << ", ";
        }
        std::cout << std::endl;
    }*/

    return instance;
}


Instance read_instance(const std::string& path)
{
    auto is = std::ifstream(path);

    if (!is.good()) {
        throw Io_error("could not open given path");
    }

    return read_instance(is);
}

std::vector<unsigned int> read_solution(const std::string& path)
{
    auto is = std::ifstream(path);

    if (!is.good()) {
        throw Io_error("could not open given solution file path");
    }

    return read_solution(is);
}

std::vector<unsigned int> read_solution(std::istream& is)
{
    std::size_t n = 0;
    is >> n;
    is_good_or_throw(is, "bad stream after reading number of activities");

    if (n >= 10000) {
        throw Io_error("number of activities in solution is rather large, not going to read it");
    }

    if (n == 0) {
        return {};
    }

    auto result = std::vector<unsigned int>(n);

    for (unsigned int i = 0; i < n; i++) {
        std::size_t j = 0;
        unsigned int start_time = 0;
        is >> j >> start_time;
        is_good_or_throw(is, "bad stream after reading (activity,start_time) pair");

        if (j == 0 || (j - 1) >= result.size()) {
            throw Io_error("read activity index out of range");
        }

        result[j-1] = start_time;
    }

    return result;
}

} // namespace uosrcp
