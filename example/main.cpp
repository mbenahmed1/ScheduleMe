#include <iostream>
#include <numeric>

#include <uosrcp/io.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: ./uosrcp-example <instance>" << std::endl;
        exit(1);
    }

    auto instance = uosrcp::read_instance(argv[1]);

    for (unsigned int r = 0; r < instance.r(); r++) {
        std::cout << "res " << r << " has " << instance.resources[r]
            << " available units" << std::endl;
    }

    std::cout << std::endl;

    for (unsigned int i = 0; i < instance.n(); i++) {

        std::cout << "i = " << i << std::endl;

        std::cout << "  pi " << instance.processing_time[i] << std::endl;
        std::cout << "  R[i,k] = ";

        for (const auto rik : instance.demands[i]) {
            std::cout << rik << " ";
        }

        std::cout << std::endl << "  SUC[i] = ";

        for (const auto succid : instance.successors[i]) {
            std::cout << succid << " ";
        }

        std::cout << std::endl;
    }

    /* Assume we had a solution.. */

    auto solution = std::vector<unsigned int>(instance.n());
    std::iota(std::begin(solution), std::end(solution), 100);

    // Write it to stdout
    uosrcp::write_solution(solution, std::cout);
    
    // Write it to a file
    uosrcp::write_solution(solution, "sol.txt");
}
