import subprocess as sp
import multiprocessing
import argparse
import sys
import os
import re

def print_usage():
    print("Usage: $ python3 generate_results.py <csv_output_file>")

if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("ERROR: No csv output file given")
        print_usage()
        sys.exit()

    instance_dir = "j30"
    if len(sys.argv) > 2:
        if sys.argv[2] != "j30" and sys.argv[2] != "j60" and sys.argv[2] != "j120":
            print("ERROR: Wroong instance_dir given. Make sure to choose between j30,j60 and j120")
            sys.exit()
        instance_dir = sys.argv[2]

    csv_file = sys.argv[1]

    cpu_ct  = multiprocessing.cpu_count()

    neighborhoods       = ["swap","api","shift","random"]
    seeds               = [1,2,3,4,5]
    times               = [0.1, 0.5, 1.0, 5.0]
    n_processes         = cpu_ct
    n_processes         = 5
    solver_path         = os.path.abspath("../scheduler/build/solver")
    instances_path      = os.path.abspath("../instances/" + instance_dir)
    # instance_regex      = ".*\\.RCP"
    instance_regex      = "[A-Z].*_1\\.RCP"
    # instance_regex      = "[A-Z]+[5]|[1-9][5-9]_[0-9].RCP"
    solutions_path      = os.path.abspath("../solutions/" + instance_dir)

    instance_names = []
    for inst in sorted(os.listdir(instances_path)):
        if re.match(instance_regex, inst):
            instance_names.append(inst[:-4])
    nb_instances = len(instance_names)

    print(f"seeds               {seeds}")
    print(f"times               {times}")
    # print(f"sum                 {sum(times)}")
    print(f"n_processes         {n_processes}")
    print(f"solver_path         {solver_path}")
    print(f"instances_path      {instances_path}")
    print(f"instance_regex      {instance_regex}")
    print(f"nb_instances        {nb_instances}\n")

    f = open(csv_file, "w+")
    # f.write("instance_nr,nbh,time,seed,makespan\n")
    f.write("instance_nr,time,seed,makespan\n")
    for i in range(nb_instances):
        sys.stdout.write(f"\rProcessing iteration {i + 1} of {nb_instances} ...")
        sys.stdout.flush()
        inst = os.path.join(instances_path, instance_names[i] + ".RCP")

        # for nbh in neighborhoods:
        for t in times:
            p = []
            for s in seeds:
                sol = os.path.join(solutions_path, instance_names[i])
                time = int(t * 1000)
                command = f"{solver_path} {inst} {sol}_t{int(time)}_s{s}.sol {t} {s} -b"
                # command = f"{solver_path} {inst} {sol}_n{nbh}_t{int(time)}_s{s}.sol {t} {s} -n {nbh} -b"
                p.append(sp.Popen(command, shell=True, stdout=sp.PIPE))

            for j in range(len(seeds)):
                p[j].wait()
                makespan = p[j].communicate()[0][:-1].decode()
                f.write(f"{instance_names[i]},{time},{seeds[j]},{makespan}\n")
                # f.write(f"{instance_names[i]},{nbh},{time},{seeds[j]},{makespan}\n")

    print()
 
    f.close()

    # print(command)
    # p = sp.Popen(command, shell=True)

