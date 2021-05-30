import subprocess
import multiprocessing
import argparse
import sys
import os
import re

if __name__ == "__main__":
    cpu_ct  = multiprocessing.cpu_count()

    times               = [i for i in range(1, 31) if i % 5 == 0 or i == 1]
    n_processes         = cpu_ct
    n_processes         = 1
    solver_path         = os.path.abspath("../scheduler/build/solver")
    instances_path      = os.path.abspath("../instances/j120/")
    instance_regex      = ".*\.RCP"
    instance_regex      = "[A-Z][1-9]_.*\.RCP"
    # instance_regex      = "[A-Z]+[5]|[1-9][5-9]_[0-9].RCP"

    instances = []
    for instance in os.listdir(instances_path):
        if re.match(instance_regex, instance):
            instances.append(instance)
    nb_instances = len(instances)

    print(f"times               {times}")
    print(f"sum                 {sum(times)}")
    print(f"n_processes         {n_processes}")
    print(f"solver_path         {solver_path}")
    print(f"instances_path      {instances_path}")
    print(f"instance_regex      {instance_regex}")
    print(f"nb_instances        {nb_instances}")


    # print(command)
    # sp = subprocess.Popen(command, shell=True)
    # sp_gensdp = subprocess.Popen(command, shell=True, stdout=logfile_gensdp, stderr=logfile_gensdp, start_new_session=True)

