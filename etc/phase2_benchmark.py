"""
    Script to automate the evaluation for phase 2
    @author David Massanés 
"""

import subprocess as sp
import multiprocessing
import argparse
import sys
import os
import re
import time

YELLOW  = '\033[93m'
ENDC    = '\033[0m'

def print_progress(i, i_total, instance):
    sys.stdout.write(f"\rIteration {i:>5}/{i_total} | Instance {instance:>8}")
    sys.stdout.flush()

if __name__ == "__main__":
    RUN = "new"
    SOLVER_PATH         = os.path.abspath("../scheduler/build/solver")
    INSTANCES_DIR       = os.path.abspath("../benchmark_instances")
    SOLUTIONS_DIR       = os.path.abspath(f"../benchmark_solutions/{RUN}")
    RESULTS_DIR         = os.path.abspath(f"phase2_results")
    SEED_PATH           = os.path.join(INSTANCES_DIR, "seeds.txt")
    BEST_KNOWN_PATH     = os.path.join(INSTANCES_DIR, "RCPSP_BKS.csv")
    TIME                = 60
    CSV_PATH            = os.path.join(RESULTS_DIR, f"{RUN}_{TIME}.csv")

    INSTANCES       = [os.path.join(INSTANCES_DIR, f) for f in os.listdir(INSTANCES_DIR) if ".RCP" in f]
    INSTANCES       = sorted(INSTANCES, key=lambda x: int(x.split("/")[-1].split("_")[0][1:]))
    NB_INSTANCES    = len(INSTANCES)

    SEEDS = []
    with open(SEED_PATH, "r") as seed_file:
        for line in seed_file.readlines():
            SEEDS.append(int(line))
    SEEDS = sorted(SEEDS)
    NB_SEEDS = len(SEEDS)

    NEIGHBORHOODS = ["swap","api","shift","random"]
    NB_NEIGHBORHOODS = len(NEIGHBORHOODS)

    BEST_KNOWN_SOLUTIONS = {}
    with open(BEST_KNOWN_PATH, "r") as best_known_solutions_file:
        for line in best_known_solutions_file.readlines():
            split = line.split(",")
            found = False
            for instance in INSTANCES:
                if split[0] in instance:
                    found = True
                    break
            if not found:
                continue
            BEST_KNOWN_SOLUTIONS[split[0]] = int(split[1])

    print(f"{YELLOW}RUN                     {ENDC}  {RUN}")
    print(f"{YELLOW}SOLVER_PATH             {ENDC}  {SOLVER_PATH}")
    print(f"{YELLOW}INSTANCES_DIR           {ENDC}  {INSTANCES_DIR}")
    print(f"{YELLOW}SOLUTIONS_DIR           {ENDC}  {SOLUTIONS_DIR}")
    print(f"{YELLOW}RESULTS_DIR             {ENDC}  {RESULTS_DIR}")
    print(f"{YELLOW}SEED_PATH               {ENDC}  {SEED_PATH}")
    print(f"{YELLOW}BEST_KNOWN_PATH         {ENDC}  {BEST_KNOWN_PATH}")
    print(f"{YELLOW}CSV_PATH                {ENDC}  {CSV_PATH}")
    print(f"{YELLOW}TIME                    {ENDC}  {TIME}")
    print(f"{YELLOW}INSTANCES               {ENDC}  {INSTANCES}")
    print(f"{YELLOW}NB_INSTANCES            {ENDC}  {NB_INSTANCES}")
    print(f"{YELLOW}NEIGHBORHOODS           {ENDC}  {NEIGHBORHOODS}")
    print(f"{YELLOW}NB_NEIGHBORHOODS        {ENDC}  {NB_NEIGHBORHOODS}")
    print(f"{YELLOW}SEEDS                   {ENDC}  {SEED_PATH}")
    print(f"{YELLOW}NB_SEEDS                {ENDC}  {NB_SEEDS}")
    print(f"{YELLOW}BEST_KNOWN_SOLUTIONS    {ENDC}  {BEST_KNOWN_SOLUTIONS}")
    print()

    with open(CSV_PATH, "w+") as csv_file:
        csv_file.write("instance,bks,nbh,time,seed,makespan\n")

        i_total = NB_NEIGHBORHOODS * NB_INSTANCES
        i = 0

        for idx_inst in range(NB_INSTANCES):
            tmp = INSTANCES[idx_inst].split("/")[-1][:-4]
            instance = tmp
            bks = BEST_KNOWN_SOLUTIONS[instance]

            for idx_nbh in range(NB_NEIGHBORHOODS):
                nbh = NEIGHBORHOODS[idx_nbh]

                i += 1
                print_progress(i, i_total, instance)

                processes = []
                for idx_seed in range(NB_SEEDS):
                    seed = SEEDS[idx_seed]
                    sol_path = os.path.join(SOLUTIONS_DIR, f"{instance}_n{nbh}_t{TIME}_s{seed}.sol")
                    command = f"{SOLVER_PATH} {INSTANCES[idx_inst]} {sol_path} {TIME} {seed} -n {nbh}"
                    processes.append((sp.Popen(command, shell=True, stdout=sp.PIPE), seed))

                    if len(processes) >= 5:
                        for p in processes:
                            p[0].wait()
                            makespan = p[0].communicate()[0][:-1].decode()
                            csv_line = f"{instance},{bks},{nbh},{TIME},{p[1]},{makespan}\n"
                            csv_file.write(csv_line)
                        processes = []

