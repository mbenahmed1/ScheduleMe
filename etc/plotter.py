import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import sys


class Act:
    def __init__(self, id, len, rsc, start):
        self.id = id
        self.len = len
        self.rsc = rsc
        self.start = start

class Rsc:
    def __init__(self, )

def print_usage():
    print('usage: $ python3 plotter.py <SOLUTION_PATH> <INSTANCE_PATH>')


def parse_files(sol_path: str, inst_path: str):

    acts = []
    sol_num_act = -1
    num_rsc = -1
    with open(sol_path) as s_file:
        lines = s_file.readlines()
        sol_num_act = lines[0]
        for line in lines[1:]:
            line.split()
            acts.append(Act(line[0], -1, -1, line[0]))

    inst_num_act = -1
    
    with open(inst_path) as i_file:
        lines = i_file.readlines()
        first_line = lines[0]
        first_line.split()
        inst_num_act = first_line[0]
        num_rsc = first_line[1]
        for line in lines[1:]:



if len(sys.argv) < 3:
    print_usage()
    sys.exit()
else:
    parse_files(sys.argv[1], sys.argv[2])


# define Matplotlib figure and axis
fig, ax = plt.subplots()

# create simple line plot
ax.plot([0, 10], [0, 10])

# add rectangle to plot
ax.add_patch(Rectangle((1, 1), 2, 6))

# display plot
plt.show()
