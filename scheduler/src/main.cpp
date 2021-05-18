//
// Created by Marcel Hündorf on 18.05.21.
//

#include <iostream>
#include <filesystem>

#include <uosrcp/io.h>
#include "../include/solution.h"
namespace fs = std::__fs::filesystem;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: ./solution_test <instance_path>" << std::endl;
        exit(2);
    }
    fs::path in = argv[1];
    auto inst_ptr = std::make_shared<uosrcp::Instance>(uosrcp::read_instance(in.string()));
    uosrcp::Solution solution(inst_ptr);
    fs::path out = "../../solutions/";
    std::string filename = in.filename();
    std::string delimiter = ".";
    std::string name = filename.substr(0, filename.find(delimiter));
    out += name;
    out += ".SOL";
    solution.write(out.string());
}