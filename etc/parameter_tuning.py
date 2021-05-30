import subprocess as sp
import multiprocessing
import argparse
import sys
import os
import re

if __name__ == "__main__":
    cpu_ct  = multiprocessing.cpu_count()

    seeds               = [0]
    times               = [i / 2 for i in range(1, 11)]
    n_processes         = cpu_ct
    n_processes         = 1
    solver_path         = os.path.abspath("../scheduler/build/solver")
    instances_path      = os.path.abspath("../instances/j120")
    instance_regex      = ".*\.RCP"
    instance_regex      = "[A-Z][1-9]_.*\.RCP"
    # instance_regex      = "[A-Z]+[5]|[1-9][5-9]_[0-9].RCP"
    solutions_path      = os.path.abspath("../solutions/j120")

    solutions = []
    instances = []
    for inst in os.listdir(instances_path):
        if re.match(instance_regex, inst):
            inst_path   = os.path.join(instances_path, inst)
            sol_path    = os.path.join(solutions_path, inst[:-4] + ".sol")
            instances.append(inst_path)
            solutions.append(sol_path)
    if len(instances) != len(solutions):
        print("len(instances) != len(solutions)")
        sys.exit(-1)
    nb_instances = len(instances)

    print(f"times               {times}")
    print(f"sum                 {sum(times)}")
    print(f"n_processes         {n_processes}")
    print(f"solver_path         {solver_path}")
    print(f"instances_path      {instances_path}")
    print(f"instance_regex      {instance_regex}")
    print(f"nb_instances        {nb_instances}")

    for i in range(nb_instances):
        for t in times:
            # ./solver <instance-path> <solution-path> <time-limit> <seed> [OPTION]...
            command = f"{solver_path} {instances[i]} {solutions[i]} {t}"
            # print(command)

    # print(command)
    # p = sp.Popen(command, shell=True)

