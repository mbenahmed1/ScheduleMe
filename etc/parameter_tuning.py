import subprocess
import multiprocessing
import argparse
import sys

cpu_ct  = multiprocessing.cpu_count()   # Number of CPUs available on this system 

def parse_args():
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('-h', '--help', action='help', default=argparse.SUPPRESS,
                        help="Show help")
    parser.add_argument('-s', '--solver', dest="SOLVER", type=str, required=True,
                        help="<str> | Path to the executable of the solver")
    parser.add_argument('-p', '--processes', dest="PROCESSES", type=int, default=1, required=False,
                        help=("<int>, default=1 | Number of solver processes to be executed simultaneously. "
                              "If you want to use all CPUs on your "
                              "system (cpu_ct) set p <= 0 or p > cpu_ct")
                        )
    args = parser.parse_args()

    if args.PROCESSES <= 0 or args.PROCESSES > cpu_ct:
        args.processes = cpu_ct

    return args

if __name__ == "__main__":
    args = parse_args()

    print(args.SOLVER)
    print(args.PROCESSES)

