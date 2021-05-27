import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import seaborn as sns
import sys
from operator import itemgetter
from matplotlib.pyplot import cycler
import numpy as np
import matplotlib
import random


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

    # if (a_x_bottom_right <= b_x_top_left) or (b_x_bottom_right <= a_x_top_left):
    #     return False

    # if (a_y_bottom_right <= b_y_top_left) or (b_y_bottom_right <= a_y_top_left):
    #     return False
    # return True

    return not (a_x_top_right <= b_x_bottom_left or a_x_bottom_left >= b_x_top_right or a_y_top_right <= b_y_bottom_left or a_y_bottom_left >= b_y_top_right)


def print_usage():
    print('usage: $ python3 plotter.py <PLOTFILE_PATH> ')


def parse_files(sol_path: str):
    # sns.set_style("dark")
    # sns.set_context("paper")
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

        print(f'Activities: {num_act}')
        print(f'Resources: {num_rsc}')
        fig, ax = plt.subplots(num_rsc)

        feaseable = True

        sorted_lines = sorted(
            lines[2:], key=lambda x: x.split()[2], reverse=True)

        while True:
            feaseable = True

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
                        for r in rects[i]:
                            if r != rect:
                                while check_overlap(r, rect):
                                    x, y = r.get_xy()
                                    _y0 += 1
                                    rect.set_y(_y0)

                        if (rect.get_y() + rect.get_height()) > caps[i]:
                            feaseable = False
                            break

                        rects[i].append(rect)

                        # print(len(rects[0]))

                        rect_list = {str(_id): rect}
                        ax[i].add_artist(rect_list[str(_id)])
                        rx, ry = rect_list[str(_id)].get_xy()
                        cx = rx + rect_list[str(_id)].get_width()/2.0
                        cy = ry + rect_list[str(_id)].get_height()/2.0

                        ax[i].annotate(str(_id), (cx, cy), color='black',
                                       fontsize=8, ha='center', va='center')

                        ax[i].title.set_text(f'Resource {i}')
                if not feaseable:
                    break
            seed = random.randint(1, 65536)
            if feaseable:
                print(f'Seed: {seed}')
                break
            
            random.Random(seed).shuffle(sorted_lines)
            sorted_lines = sorted(
                lines[2:], key=lambda x: x.split()[2], reverse=True)

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

    plt.show()
