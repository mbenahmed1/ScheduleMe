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

/*    if (argc > 2)
    {
        // instance and solution path
        INSTANCE_PATH = argv[1];
        SOLUTION_PATH = argv[2];

        if (argc > 3)
        {
            TIME_LIMIT = std::stoi(argv[3]);
        }
        if (argc > 4)
        {
            SEED = std::stoi(argv[4]);
        }
        if (argc > 5)
        {
            for (int i = 6; i < argc; i++)
            {

            }
        }
    }
    else
    {
        std::cout << "usage: ./solver <instance_path> <solution_path> [OPTIONS]" << std::endl;
        return false;
    }
    return true;*/

using namespace ScheduleMe;

std::string INSTANCE_PATH;
std::string SOLUTION_PATH;
int TIME_LIMIT = 1;
int SEED = 1;
double ALPHA = 0.9;
double TEMPERATURE = 1.0;
bool NOHEUR = false;
bool VERBOSE = false;

bool parse_arguments(int, char **);

void print_usage();

void print_usage()
{
    std::cout << "usage" << std::endl;
}

bool parse_arguments(int argc, char **argv)
{
    //Specifying the expected options
    //The two options l and b expect numbers as argument
    static struct option long_options[] = {
            {"timelimit", required_argument, 0,       't'},
            {"temp",      required_argument, 0,       'c'},
            {"seed",      required_argument, 0,       's'},
            {"in",        required_argument, 0,       'i'},
            {"out",       required_argument, 0,       'o'},
            {"alpha",     required_argument, 0,       'a'},
            {"noheur",    no_argument,       0,       'n'},
            {"verbose",   no_argument,       0,       'v'},
            {nullptr, 0,                     nullptr, 0}
    };

    int long_index = 0;
    while (true)
    {
        int result = getopt_long(argc, argv, "t:c:s:i:o:a:nv", long_options, &long_index);

        if (result == -1)
        {
            break;
        }
        switch (result)
        {
            case 't' :
                TIME_LIMIT = std::stoi(optarg);
                break;
            case 'c' :
                TEMPERATURE = std::stod(optarg);
                break;
            case 's' :
                SEED = std::stoi(optarg);
                break;
            case 'i' :
                INSTANCE_PATH = optarg;
                break;
            case 'o' :
                SOLUTION_PATH = optarg;
                break;
            case 'a' :
                ALPHA = std::stod(optarg);
                break;
            case 'n' :
                NOHEUR = true;
                break;
            case 'v' :
                VERBOSE = true;
                break;
            default  :
                print_usage();
                break;
        }
    }

    if (INSTANCE_PATH.empty() || SOLUTION_PATH.empty() || TIME_LIMIT <= 0 || ALPHA <= 0 || TEMPERATURE < 0)
    {
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

    // for (int i = 0; i < instance.n(); i++)
    // {
    //     std::cout << i << " ";
    //     for (int j = 0; j < instance.predecessors_full[i].size(); j++)
    //     {
    //         std::cout << instance.predecessors_full[i][j] << ",";
    //     }
    //     std::cout << std::endl;
    // }

    if (NOHEUR)
    {
        std::vector<unsigned int> s = ScheduleGenerator::generate_precedence_list(instance);
        for (int i = 0; i < s.size(); i++)
        {
            std::cout << s[i] << ",";
        }
        std::cout << std::endl;
        int c_s = ScheduleGenerator::earliest_start_schedule(instance, s, VERBOSE);
        std::cout << "nh makespan: " << c_s << std::endl;
    }
    else
    {
        SimulatedAnnealing sa = SimulatedAnnealing(TIME_LIMIT, ALPHA, TEMPERATURE, SEED, VERBOSE);
        std::vector<unsigned int> s = sa.solve(instance);
        for (int i = 0; i < s.size(); i++)
        {
            std::cout << s[i] << ",";
        }
        std::cout << std::endl;
        int c_s = ScheduleGenerator::earliest_start_schedule(instance, s, VERBOSE);
        std::cout << "sa makespan: " << c_s << std::endl;
    }

    ScheduleMe::write_solution(instance.start_time, SOLUTION_PATH);

    instance.write_plotable("./test.lol");
}

