import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import seaborn as sns
import sys

colors = [
    'aliceblue',
    'antiquewhite',
    'aqua',
    'aquamarine',
    'azure',
    'beige',
    'bisque',
    'black',
    'blanchedalmond',
    'blue',
    'blueviolet',
    'brown',
    'burlywood',
    'cadetblue',
    'chartreuse',
    'chocolate',
    'coral',
    'cornflowerblue',
    'cornsilk',
    'crimson',
    'cyan',
    'darkblue',
    'darkcyan',
    'darkgoldenrod',
    'darkgray',
    'darkgreen',
    'darkkhaki',
    'darkmagenta',
    'darkolivegreen',
    'darkorange',
    'darkorchid',
    'darkred',
    'darksalmon',
    'darkseagreen',
    'darkslateblue',
    'darkslategray',
    'darkturquoise',
    'darkviolet',
    'deeppink',
    'deepskyblue',
    'dimgray',
    'dodgerblue',
    'firebrick',
    'floralwhite',
    'forestgreen',
    'fuchsia',
    'gainsboro',
    'ghostwhite',
    'gold',
    'goldenrod',
    'gray',
    'green',
    'greenyellow',
    'honeydew',
    'hotpink',
    'indianred',
    'indigo',
    'ivory',
    'khaki',
    'lavender',
    'lavenderblush',
    'lawngreen',
    'lemonchiffon',
    'lightblue',
    'lightcoral',
    'lightcyan',
    'lightgoldenrodyellow',
    'lightgreen',
    'lightgray',
    'lightpink',
    'lightsalmon',
    'lightseagreen',
    'lightskyblue',
    'lightslategray',
    'lightsteelblue',
    'lightyellow',
    'lime',
    'limegreen',
    'linen',
    'magenta',
    'maroon',
    'mediumaquamarine',
    'mediumblue',
    'mediumorchid',
    'mediumpurple',
    'mediumseagreen',
    'mediumslateblue',
    'mediumspringgreen',
    'mediumturquoise',
    'mediumvioletred',
    'midnightblue',
    'mintcream',
    'mistyrose',
    'moccasin',
    'navajowhite',
    'navy',
    'oldlace',
    'olive',
    'olivedrab',
    'orange',
    'orangered',
    'orchid',
    'palegoldenrod',
    'palegreen',
    'paleturquoise',
    'palevioletred',
    'papayawhip',
    'peachpuff',
    'peru',
    'pink',
    'plum',
    'powderblue',
    'purple',
    'red',
    'rosybrown',
    'royalblue',
    'saddlebrown',
    'salmon',
    'sandybrown',
    'seagreen',
    'seashell',
    'sienna',
    'silver',
    'skyblue',
    'slateblue',
    'slategray',
    'snow',
    'springgreen',
    'steelblue',
    'tan',
    'teal',
    'thistle',
    'tomato',
    'turquoise',
    'violet',
    'wheat',
    'white',
    'whitesmoke',
    'yellow',
    'yellowgreen'
]


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

    return not (a_x_top_right < b_x_bottom_left or a_x_bottom_left > b_x_top_right or a_y_top_right < b_y_bottom_left or a_y_bottom_left > b_y_top_right)


def print_usage():
    print('usage: $ python3 plotter.py <PLOTFILE_PATH> ')


def parse_files(sol_path: str):
    sns.set_style("dark")
    sns.set_context("paper")
    caps = []
    num_act = -1
    num_rsc = -1
    rects = []

    with open(sol_path) as s_file:
        lines = s_file.readlines()
        first = lines[0]
        first = first.split()
        num_act = int(first[0])
        num_rsc = int(first[1])

        second = lines[1]
        second = second.split()
        for act in range(num_rsc):
            caps.append(int(second[act]))

        print(f'Activities: {num_act}')
        print(f'Resources: {num_rsc}')
        fig, ax = plt.subplots(num_rsc)

        for i, a in enumerate(ax):
            a.grid()
            a.set(xlim=(-0.1, 90), ylim=(-0.1, caps[i] + 0.1))

        for line in lines[2:]:
            line = line.split()
            _id = int(line[0])
            start = int(line[1])
            proc = int(line[2])

            for i, demand in enumerate(line[3:]):
                if int(demand) != 0:
                    rect = Rectangle((start, 0), proc, int(
                        demand), facecolor=colors[_id + 30], edgecolor='black')
                    rect_dict = {str(_id): rect}
                    rects.append(rect)

                    for r in rects:
                        if check_overlap(r, rect):
                            rect.set_y(r.get_height() + rect.get_height())
                        break

                    ax[i].add_artist(rect_dict[str(_id)])
                    rx, ry = rect_dict[str(_id)].get_xy()
                    cx = rx + rect_dict[str(_id)].get_width()/2.0
                    cy = ry + rect_dict[str(_id)].get_height()/2.0

                    ax[i].annotate(str(_id), (cx, cy), color='grey', weight='bold',
                                   fontsize=8, ha='center', va='center')

                    ax[i].title.set_text(f'Resource {i}')


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print_usage()
        sys.exit()
    else:
        parse_files(sys.argv[1])

    plt.show()
