//
// Created by tim on 18.05.21.
//

#include "../include/io.hpp"
#include "../include/solver.hpp"
#include "../include/schedule_generator.hpp"
#include "../include/simulated_annealing.hpp"

#include <getopt.h>
#include <iostream>
#include <string>

using namespace ScheduleMe;

static std::string         INSTANCE_PATH;
static std::string         SOLUTION_PATH;
static double              TIME_LIMIT;
static unsigned int        SEED;

static double              ALPHA               = 0.0;
static double              TEMPERATURE         = 0.0;
static std::string         NEIGHBORHOOD        = "swap";
static bool                NOHEUR              = false;
static bool                VERBOSE             = false;

bool parse_arguments(int, char **);

void print_usage();

void print_usage()
{
    std::cout << "Usage" << std::endl;
    std::cout << " ./solver <instance-path> <solution-path> <time-limit> <seed> [OPTION]...\n" << std::endl;

    std::cout << "OPTIONS:" << std::endl;
    std::cout << " --temp, -t           <double>, default=0.0 (initial temperature is calculated dependent on the given instance)" << std::endl;
    std::cout << " --alpha, -a          <double>, default=0.0 (alpha is calculated dependent on the approximately predicted total number of iterations)" << std::endl;
    std::cout << " --neighborhood, -n   <swap|api|shift|random>, default=swap" << std::endl;
    std::cout << " --noheur, -h         do not optimize the initial solution" << std::endl;
    std::cout << " --verbose, -v        print resource profile of solution" << std::endl;
}

bool parse_arguments(int argc, char **argv)
{
    //Specifying the expected options
    //The two options l and b expect numbers as argument
    static struct option long_options[] = {
            {"temp",            required_argument,      nullptr,    't'},
            {"alpha",           required_argument,      nullptr,    'a'},
            {"neighborhood",    required_argument,      nullptr,    'n'},
            {"noheur",          no_argument,            nullptr,    'h'},
            {"verbose",         no_argument,            nullptr,    'v'},
            {nullptr,           0,                      nullptr,    0}
    };

    if (argc < 5)
    {
        std::cerr << "Not enough command line arguments given!" << std::endl;
        print_usage();
        return false;
    }

    int long_index = 0;
    while (true)
    {
        int result = getopt_long(argc - 4, argv + 4, "t:a:n:hv", long_options, &long_index);

        if (result == -1)
        {
            break;
        }
        switch (result)
        {
            case 't' :
                TEMPERATURE = std::stod(optarg);
                break;
            case 'a' :
                ALPHA = std::stod(optarg);
                break;
            case 'n' :
                NEIGHBORHOOD = optarg;
                break;
            case 'h' :
                NOHEUR = true;
                break;
            case 'v' :
                VERBOSE = true;
                break;
            default  :
                print_usage();
                return false;
        }
    }

    INSTANCE_PATH   = argv[1];
    SOLUTION_PATH   = argv[2];
    if (! (TIME_LIMIT = std::stod(argv[3])))
    {
        std::cerr << "Could not parse time-limit!" << std::endl;
        print_usage();
        return false;
    }
    if (! (SEED = std::stoul(argv[4])))
    {
        std::cerr << "ERROR: Could not parse seed!" << std::endl;
        print_usage();
        return false;
    }
    if (TIME_LIMIT <= 0)
    {
        std::cerr << "ERROR: Invalid time-limit (<= 0)!" << std::endl;
        print_usage();
        return false;
    }
    if (TEMPERATURE < 0.0)
    {
        std::cerr << "ERROR: Invalid temperature (< 0.0)!" << std::endl;
        print_usage();
        return false;
    }
    if (ALPHA < 0.0)
    {
        std::cerr << "ERROR: Invalid alpha (< 0.0)!" << std::endl;
        print_usage();
        return false;
    }
    if (NEIGHBORHOOD != "swap" && NEIGHBORHOOD != "api" && NEIGHBORHOOD != "shift" && NEIGHBORHOOD != "random")
    {
        std::cerr << "ERROR: Invalid neighborhood!" << std::endl;
        print_usage();
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    if (!parse_arguments(argc, argv))
    {
        return EXIT_FAILURE;
    }

    Instance instance = read_instance(INSTANCE_PATH);

    if (NOHEUR)
    {
        std::vector<unsigned int> s = ScheduleGenerator::generate_precedence_list(instance);
        unsigned int c_s = ScheduleGenerator::earliest_start_schedule(instance, s);
        std::cout << "Initial makespan:" << std::right << std::setw(4) << c_s << std::endl;
        if (VERBOSE)
        {
            std::cout << std::endl;
            ScheduleGenerator::earliest_start_schedule(instance, s, true);
        }
    }
    else
    {
        SimulatedAnnealing sa = SimulatedAnnealing(TIME_LIMIT, ALPHA, TEMPERATURE, SEED, VERBOSE, NEIGHBORHOOD);
        std::vector<unsigned int> s = sa.solve(instance);
        if (VERBOSE)
        {
            std::cout << std::endl;
            ScheduleGenerator::earliest_start_schedule(instance, s, true);
        }
    }

    ScheduleMe::write_solution(instance.start_time, SOLUTION_PATH);

    instance.write_plotable("./test.lol");
}

