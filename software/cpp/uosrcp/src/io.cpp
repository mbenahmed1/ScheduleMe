#include <uosrcp/io.h>

#include <fstream>


namespace uosrcp {

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
