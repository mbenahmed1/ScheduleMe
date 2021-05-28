import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import seaborn as sns
import sys
from operator import itemgetter
from matplotlib.pyplot import cycler
import numpy as np
import matplotlib
import random
import re

class Act:
    def __init__(self, _id: int, start: int, proc: int, demand: int, y_pos: int):
        self._id = _id
        self.start = start
        self.proc = proc
        self.demand = demand
        self.y_pos = y_pos
    def __str__(self):
        return(f'ID: {self._id} start: {self.start} proc: {self.proc} demand: {self.demand} y_pos: {self.y_pos}')


def check_rect_feaseability(act: Act, rsc_cap: int) -> bool:
    rect = Rectangle((act.start, act.y_pos), act.proc, act.demand)
    return not ((rect.get_y() + rect.get_height()) > rsc_cap)

def check_y_pos(act: Act, acts: list, cap: int) -> bool:
    rect = Rectangle((act.start, act.y_pos), act.proc, act.demand)
    for i in range(len(acts)):
        other_rect = Rectangle((acts[i].start, acts[i].y_pos), acts[i].proc, acts[i].demand)
        if other_rect != rect:
            if check_overlap(other_rect, rect):
                #print('Geht nicht !!!')
                x, y = rect.get_xy()
                y += 1
                rect.set_y(y)
                if check_overlap(other_rect, rect):
                    #print('Höher nicht !!!')
                    return False
    return True

def check_y_pos_2(act: Act, acts: list, cap: int) -> bool:
    rect = Rectangle((act.start, act.y_pos), act.proc, act.demand)
    for i in range(len(acts)):
        other_rect = Rectangle((acts[i].start, acts[i].y_pos), acts[i].proc, acts[i].demand)
        if other_rect != rect:
            if check_overlap(other_rect, rect):
                return False
    return True

def plot_acts(acts: list, plotted_acts: list, cap: int, start_id: int):

    #print(f'len {len(acts)} s_id {start_id}')
    # new_rect = Rectangle((acts[start_id].start, acts[start_id].y_pos), acts[start_id].proc, acts[start_id].demand, facecolor="grey", edgecolor='black')
    
    print(start_id, acts[start_id].y_pos)
    if check_y_pos_2(acts[start_id], plotted_acts, cap):
        plotted_acts.append(acts[start_id])
        print(start_id)
        for act in plotted_acts:
            print(act)
        if start_id < (len(acts) - 1):
            if not plot_acts(acts, plotted_acts, cap, start_id + 1):
                plotted_acts.pop()
                acts[start_id].y_pos += 1
                if check_rect_feaseability(acts[start_id], cap):
                    return plot_acts(acts, plotted_acts, cap, start_id)

        return True
        #print("+-0")
    else:
        print(start_id, "y not")
        acts[start_id].y_pos += 1
        if check_rect_feaseability(acts[start_id], cap):
            print(start_id, "feasable")
            # plotted_acts.append(acts[start_id])
            return plot_acts(acts, plotted_acts, cap, start_id)
            #print("+1")
        else:
            print(start_id, "infeasable")
            acts[start_id].y_pos = 0
            #print("-1")
            return False
    
    
    # if len(plotted_acts) != len(acts):
    #     if start_id < (len(acts) - 1):
    #         plot_acts(acts, plotted_acts, cap, start_id + 1)
    #         print("next")


def check_overlap(rect_a: Rectangle, rect_b: Rectangle) -> bool:

    a_x_bottom_left, a_y_bottom_left = rect_a.get_xy()
    b_x_bottom_left, b_y_bottom_left = rect_b.get_xy()

    a_x_bottom_right = a_x_bottom_left + rect_a.get_width()
    b_x_bottom_right = b_x_bottom_left + rect_b.get_width()
    a_y_bottom_right = a_y_bottom_left
    b_y_bottom_right = b_y_bottom_left

    a_x_top_right = a_x_bottom_right
    b_x_top_right = b_x_bottom_right
    a_y_top_right = a_y_bottom_right + rect_a.get_height()
    b_y_top_right = b_y_bottom_right + rect_b.get_height()

    a_x_top_left = a_x_bottom_left
    b_x_top_left = b_x_bottom_left
    a_y_top_left = a_y_bottom_left + rect_a.get_height()
    b_y_top_left = b_y_bottom_left + rect_b.get_height()

    return not (a_x_top_right <= b_x_bottom_left or a_x_bottom_left >= b_x_top_right or a_y_top_right <= b_y_bottom_left or a_y_bottom_left >= b_y_top_right)

def check_overlap_rsc(rect_a: Rectangle, rects_in_rsc: list) -> bool:
    for rect in rects_in_rsc:
        return check_overlap(rect_a, rect)
    return False

def check_x_conf(rect_a: Rectangle, rect_b: Rectangle) -> bool:
    x_a, y_a = rect_a.get_xy()
    x_b, y_b = rect_b.get_xy()

    w_a = rect_a.get_width()
    w_b = rect_b.get_width()

    dist = []
    dist.append(abs(x_a - x_b + w_b))
    dist.append(abs(x_a - x_b))
    dist.append(abs(x_b - x_a + w_a))
    dist.append(abs(x_b - x_a))

    span = max(dist)

    return (span - w_a - w_b) < 0
    

def print_usage():
    print('usage: $ python3 plotter.py <PLOTFILE_PATH> ')




def parse_files(sol_path: str):
    caps = []
    num_act = -1
    num_rsc = -1
    end_times = []

    with open(sol_path) as s_file:
        lines = s_file.readlines()
        first = lines[0]
        first = first.split()
        num_act = int(first[0])
        num_rsc = int(first[1])

        colors = plt.cm.Pastel2(np.linspace(0, 1, num_act))

        second = lines[1]
        second = second.split()
        for act in range(num_rsc):
            caps.append(int(second[act]))

        rects = {}
        for rsc in range(num_rsc):
            rects[rsc] = []

        fig, ax = plt.subplots(num_rsc)

        feaseable = True

        sorted_lines = sorted(
            lines[2:], key=lambda x: x.split()[1], reverse=True)

        _ys = []

        for line in sorted_lines:
            line = line.split()
            _id = int(line[0])
            start = int(line[1])
            proc = int(line[2])

            for i, demand in enumerate(line[3:]):
                if int(demand) != 0:

                    rect = Rectangle((start, 0), proc, int(
                        demand), facecolor=colors[_id], edgecolor='black')

                    end_times.append(start + proc)

                    _y0 = 0
                    _y = _y0

                    for r in rects[i]:
                        if r != rect:
                            while check_overlap(r, rect):
                                _y += 1
                                rect.set_y(_y)
  
                    rects[i].append(rect)

                    rect_list = {str(_id): rect}
                    ax[i].add_artist(rect_list[str(_id)])
                    rx, ry = rect_list[str(_id)].get_xy()
                    cx = rx + rect_list[str(_id)].get_width()/2.0
                    cy = ry + rect_list[str(_id)].get_height()/2.0
                    ax[i].annotate(str(_id), (cx, cy), color='black',
                                   fontsize=8, ha='center', va='center')
                    ax[i].title.set_text(f'Resource {i}')

        y_range = range(0, max(caps), 1)
        x_range = range(0, max(end_times), 1)
        for i, a in enumerate(ax):
            # a.grid()
            a.set(xlim=(-0.1, max(end_times) + 1), ylim=(-0.1,
                                                         max(caps) + 1), yticks=y_range, xticks=x_range)
            a.axline((-0.1, caps[i]), (max(end_times),
                                       caps[i]), ls='--', lw=1.5, color='black')


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print_usage()
        sys.exit()
    else:
        parse_files(sys.argv[1])
        

        # neuer versuch
        caps = []
        end_times = []
        rects = []

        with open(sys.argv[1]) as s_file:
            lines = s_file.readlines()
            first = lines[0]
            first = first.split()
            num_act = int(first[0])
            num_rsc = int(first[1])
            
            fig, ax = plt.subplots(num_rsc)

            second = lines[1]
            second = second.split()
            for i, rsc in enumerate(second):
                caps.append(int(rsc))
            
            acts = {}
            plotted_acts = {}
            for rsc in range(num_rsc):
                acts[rsc] = []
                plotted_acts[rsc] = []
            
            sorted_lines = sorted(lines[2:], key=lambda x: x.split()[1], reverse=False)
            
            for line in sorted_lines:
                line = line.split()
                _id = int(line[0])
                start = int(line[1])
                proc = int(line[2])
            
                for rsc in range(num_rsc):
                    demand = int(line[rsc + 3])
                    if demand > 0:
                        act = Act(_id, start, proc, demand, 0)
                        acts[rsc].append(act)
            
            for rsc in range(num_rsc):
                plot_acts(acts[rsc], plotted_acts[rsc], caps[rsc], 0)
                break;

            for rsc in range(num_rsc):
                for act in plotted_acts[rsc]:
                    new_rect = Rectangle((act.start, act.y_pos), act.proc, act.demand, facecolor='white', edgecolor='black')
                    
                    end_times.append(act.start + act.proc)

                    print(act)

                    ax[rsc].add_artist(new_rect)
                    rx, ry = new_rect.get_xy()
                    cx = rx + new_rect.get_width()/2.0
                    cy = ry + new_rect.get_height()/2.0
                    ax[rsc].annotate(act._id, (cx, cy), color='black',
                                   fontsize=8, ha='center', va='center')
            
            y_range = range(0, max(caps), 1)
            x_range = range(0, max(end_times), 1)
            for i, a in enumerate(ax):
                # a.grid()
                a.set(xlim=(-0.1, max(end_times) + 1), ylim=(-0.1,
                                                            max(caps) + 1), yticks=y_range, xticks=x_range)
                a.axline((-0.1, caps[i]), (max(end_times),
                                        caps[i]), ls='--', lw=1.5, color='black')

                


    plt.show()
