""" plotter.py

This file contains a plotter for rcpsp solutions. It reads files in the following format:


n r
[cap]^r
id start proc [demand]^r
...
...

with 
    n:=         number of activities
    r:=         number of resources
    cap:=       capacity per resource

and each line after the second representing an activity with

    id:=        the id
    start:=     starting time
    proc:=      processing time
    demand:=    the demand of resource r of activity id

Example:

3 2
5 10
1 1 3 1 0
2 0 2 0 3
3 3 3 0 2
"""

import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import sys
from matplotlib.pyplot import cycler
import numpy as np
import matplotlib
import time


class Act:
    """Class that represents an Activity."""

    def __init__(self, _id: int, start: int, proc: int, demand: int, y_pos: int):
        """Inits an instance of Act class.

        Args:
            _id:                id of the Activity.
            start:              starting time.
            proc:               processing time.
            demand:             demand of a certain resource.
            y_pos:              y position.
        """
        self._id = _id
        self.start = start
        self.proc = proc
        self.demand = demand
        self.y_pos = y_pos

    def __str__(self):
        """Creates a string of a given Activity.

        Return:
            String that represents the Activities attributes.
        """
        return(f'ID: {self._id} start: {self.start} proc: {self.proc} demand: {self.demand} y_pos: {self.y_pos}')


def check_rect_feaseability(act: Act, rsc_cap: int) -> bool:
    """Checks if Activity has resource feaseable position.

    Args:
        act:                Activity to check.
        rsc_cap:            Capacity of the corresponding resource.
    Return:
        True if feaseable, False if not.
    """
    rect = Rectangle((act.start, act.y_pos), act.proc, act.demand)
    return not ((rect.get_y() + rect.get_height()) > rsc_cap)


def check_y_pos(act: Act, acts: list) -> bool:
    """Checks if the y position of Activities overlaps any other given Activities.

    Args:
        act:                Activity of which to check the overlap.
        acts:               List of Activities to check against.
    Returns:
        True if Activity does not overlap with any of the other Activities.
        False if not.
    """
    rect = Rectangle((act.start, act.y_pos), act.proc, act.demand)
    for i in range(len(acts)):
        other_rect = Rectangle(
            (acts[i].start, acts[i].y_pos), acts[i].proc, acts[i].demand)
        if other_rect != rect:
            if check_overlap(other_rect, rect):
                return False
    return True


def plot_acts(acts: list, plotted_acts: list, cap: int, start_id: int) -> None:
    """Recursively calls itself to serach for feaseable Rectangle positions.

    Args:
        acts:               List of all activities of one resource.
        plotted_acts:       List of all activities that are scheduled.
        cap:                Maximum capacity of the resource.
        start_id:           ID of the chosen activity.
    """
    if check_y_pos(acts[start_id], plotted_acts):
        plotted_acts.append(acts[start_id])
        if start_id < (len(acts) - 1):
            if not plot_acts(acts, plotted_acts, cap, start_id + 1):
                plotted_acts.pop()
                acts[start_id].y_pos += 1
                if check_rect_feaseability(acts[start_id], cap):
                    return plot_acts(acts, plotted_acts, cap, start_id)
                else:
                    acts[start_id].y_pos = 0
                    return False
        return True
    else:
        acts[start_id].y_pos += 1
        if check_rect_feaseability(acts[start_id], cap):
            return plot_acts(acts, plotted_acts, cap, start_id)
        else:
            acts[start_id].y_pos = 0
            return False


def check_overlap(rect_a: Rectangle, rect_b: Rectangle) -> bool:
    """Checks if two Rectangles overlap eachother.

    Args:
        rect_a:             First Rectangle to check.
        rect_b:             Second Rectangle to check.
    Return:
        True if Rectangles overlap, False if not.
    """
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


def print_usage() -> None:
    """Prints the usage to console."""
    print('usage: $ python3 plotter.py <PLOTFILE_PATH> ')


def parse_files(sol_path: str) -> None:
    """Parses the given path and starts the plotting process.

    Args:
        sol_path:               Path to the solution file.
    """
    caps = []
    end_times = []
    rects = []

    with open(sol_path) as s_file:
        lines = s_file.readlines()
        first = lines[0]
        first = first.split()
        num_act = int(first[0])
        num_rsc = int(first[1])
        colors = plt.cm.Pastel2(np.linspace(0, 1, num_act))
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
        sorted_lines = sorted(
            lines[2:], key=lambda x: x.split()[1], reverse=False)

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
        start = time.time_ns()

        for rsc in range(num_rsc):
            acts[rsc] = sorted(acts[rsc], key=lambda x: x.start, reverse=False)

        for rsc in range(num_rsc):
            plot_acts(acts[rsc], plotted_acts[rsc], caps[rsc], 0)

        secs = (time.time_ns() - start) / 1000000000.0
        print(f'Found plot in {round(secs, 3)} s.')

        for rsc in range(num_rsc):
            if not (len(acts[rsc]) == len(plotted_acts[rsc])):
                print(
                    f"Error: Resource {rsc} does not contain all activities!")
                sys.exit(-1)
            else:
                print(f"Resource {rsc} ok.")

        for rsc in range(num_rsc):
            for act in plotted_acts[rsc]:
                new_rect = Rectangle((act.start, act.y_pos), act.proc,
                                     act.demand, facecolor=colors[act._id], edgecolor='black')
                end_times.append(act.start + act.proc)
                ax[rsc].add_artist(new_rect)
                rx, ry = new_rect.get_xy()
                cx = rx + new_rect.get_width()/2.0
                cy = ry + new_rect.get_height()/2.0
                ax[rsc].annotate(act._id, (cx, cy), color='black',
                                 fontsize=8, ha='center', va='center')

        y_range = range(0, max(caps), 1)
        x_range = range(0, max(end_times), 1)
        for i, a in enumerate(ax):
            a.title.set_text(f'Resource {i}')
            a.set(xlim=(-0.1, max(end_times) + 1), ylim=(-0.1,
                                                         max(caps) + 1), yticks=y_range, xticks=x_range)
            a.axline((-0.1, caps[i]), (max(end_times),
                                       caps[i]), ls='--', lw=1.5, color='black')


if __name__ == "__main__":
    if len(sys.argv) > 1:
        parse_files(sys.argv[1])
        plt.show()
        sys.exit(-1)
    else:
        print_usage()
        sys.exit(-1)
