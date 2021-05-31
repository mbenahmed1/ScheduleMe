import subprocess as sp
import multiprocessing
import argparse
import sys
import os
import re

def print_usage():
    print("Usage: $ python3 generate_results.py <csv_output_file> [<instance_dir>] [<neighborhood>]")
    print(" default <instance_dir>: j30")
    print(" default <neighborhood>: all (swap,api,shift,random)")

if __name__ == "__main__":

    print_usage()

    if len(sys.argv) < 2:
        print("ERROR: No csv output file given")
        sys.exit()

    instance_dir = "j30"
    if len(sys.argv) > 2:
        if sys.argv[2] != "j30" and sys.argv[2] != "j60" and sys.argv[2] != "j120":
            print("ERROR: Wrong instance_dir given. Make sure to choose between j30,j60 and j120")
            sys.exit()
        instance_dir = sys.argv[2]

    neighborhoods = ["swap","api","shift","random"]
    neighborhoods = ["swap","api"]
    if len(sys.argv) > 3:
        if sys.argv[3] != "swap" and sys.argv[3] != "api" and sys.argv[3] != "shift" and sys.argv[3] != "random":
            print("ERROR: Wrong neighborhood given. Make sure to choose between swap, api, shift and random")
            sys.exit()
        neighborhoods = [sys.argv[3]]

    opt = {
        "j30": os.path.abspath("../solutions/j30/opt.txt"),
        "j60": os.path.abspath("../solutions/j60/opt.txt"),
        "j120": os.path.abspath("../solutions/j120/opt.txt")
    }

    opt_file = opt[instance_dir]
    csv_file = sys.argv[1]

    cpu_ct  = multiprocessing.cpu_count()

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
        # instance_names.append(inst[:-4])
        if re.match(instance_regex, inst):
            instance_names.append(inst[:-4])
        # if re.match(instance_regex, inst) and "5" in inst:
        #     instance_names.append(inst[:-4])
    nb_instances = len(instance_names)

    print(f"seeds               {seeds}")
    print(f"times               {times}")
    print(f"neighborhoods       {neighborhoods}")
    print(f"n_processes         {n_processes}")
    print(f"solver_path         {solver_path}")
    print(f"instance_dir        {instance_dir}")
    print(f"instances_path      {instances_path}")
    print(f"instance_regex      {instance_regex}")
    print(f"nb_instances        {nb_instances}\n")

    f_opt = open(opt_file, "r")
    opts = {}
    i = 1
    j = 0
    while True:
        if j % 10 == 0 and j != 0:
            i += 1
            j = 0
        j += 1
        line = f_opt.readline()
        if not line:
            break
        makespan = line.split("\t")[0]
        if i not in opts.keys():
            opts[i] = {}
        if j not in opts[i].keys():
            opts[i][j] = makespan
        # print(i,j,makespan)
        # print(line, end="")
    f_opt.close()

    f_csv = open(csv_file, "w+")
    f_csv.write("instance_nr,opt,nbh,time,seed,makespan\n")
    for i in range(nb_instances):
        sys.stdout.write(f"\rProcessing iteration {i + 1} of {nb_instances} ...")
        sys.stdout.flush()
        inst_ids = instance_names[i].split("_")
        nr_i = inst_ids[0][1:]
        nr_j = int(inst_ids[1])
        if instance_dir == "j30" or instance_dir == "j60":
            nr_i = nr_i[2:]
        nr_i = int(nr_i)
        opt = opts[nr_i][nr_j]

        inst = os.path.join(instances_path, instance_names[i] + ".RCP")

        for nbh in neighborhoods:
            for t in times:
                p = []
                for s in seeds:
                    sol = os.path.join(solutions_path, instance_names[i])
                    time = int(t * 1000)
                    command = f"{solver_path} {inst} {sol}_n{nbh}_t{int(time)}_s{s}.sol {t} {s} -n {nbh} -b"
                    # command = f"{solver_path} {inst} {sol}_n{nbh}_t{int(time)}_s{s}.sol {t} {s} -n {nbh} -b -l {sol}_n{nbh}_t{int(time)}_s{s}.lol"
                    # print(command)
                    p.append(sp.Popen(command, shell=True, stdout=sp.PIPE))

                for j in range(len(seeds)):
                    p[j].wait()
                    makespan = p[j].communicate()[0][:-1].decode()
                    # print(instance_names[i], nr_i, nr_j, opt, nbh, t, seeds[j], makespan)
                    f_csv.write(f"{instance_names[i]},{opt},{nbh},{time},{seeds[j]},{makespan}\n")

    print()

    f_csv.close()

    # print(command)
    # p = sp.Popen(command, shell=True)

