
#include "../scheduler/include/io.hpp"

#include <algorithm>
#include <iostream>


int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "usage: ./validator <instance> <solution>" << std::endl;
        exit(2);
    }

    try {

        const auto instance = ScheduleMe::read_instance(argv[1]);
        const auto solution = ScheduleMe::read_solution(argv[2]);

        if (instance.n() != solution.size()) {
            std::cerr << "instance has " << instance.n() << " activites, but solution has "
                      << solution.size() << std::endl;
            exit(1);
        }


        // Setup resource profiles
        auto make_span = solution.back();
    
        auto profiles = std::vector<std::vector<unsigned int>>(instance.r());
        for (unsigned int r = 0; r < instance.r(); r++) {
            profiles[r].resize(make_span, instance.resources[r]);
        }

        auto has_errors = false;

        for (unsigned int i = 0; i < instance.n(); i++) {

            const auto si = solution[i];
            const auto pi = instance.processing_time[i];

            // Check precedence constraints.
            for (const auto j : instance.successors[i]) {
                if (si + pi > solution[j]) {
                    ::fprintf(stderr,
                      "activity %u starts at time %u, but predecessor %u ends at %u\n",
                      j + 1,
                      solution[j],
                      i + 1,
                      si + pi);

                    has_errors = true;
                }
            }

            // Consume resources
            for (unsigned int r = 0; r < instance.r(); r++) {
                auto demand = instance.demands[i][r];
                for (unsigned int t = si; t < si + pi; t++) {
                    if (profiles[r][t] < demand) {
                        ::fprintf(stderr, "resource %u is exceeded at time %u\n", r + 1, t);
                        has_errors = true;
                    }

                    profiles[r][t] -= std::min(profiles[r][t], demand);
                }
            }
        }

        return has_errors * 1;

    } catch (const std::exception& ex) {
        std::cerr << "caught exception: " << ex.what() << std::endl;
        exit(1);
    }
}
