import heapq
import sys


def shoelace(v):
    """
    Shoelace formula:
    A = 1/2 * sum ( x_i * y_i+1 - x_i+1 * y_i ) )
    """
    area = 0
    for i in range(len(v)-1):
        x1, y1 = v[i]
        x2, y2 = v[i + 1]
        area += x1 * y2 - x2 * y1 
    return abs(area // 2)


def solve(lines, part2: bool = False):

    inp = [tuple(ln.strip("\n").split(" ")) for ln in lines]
    dirs = {"R": (1, 0), "D": (0, 1), "L": (-1, 0), "U": (0, -1)}
    dirs_i = {0: "R", 1: "D", 2: "L", 3: "U"}

    if part2:
        inp2 = []
        for (_, _, col) in inp:
            k2 = int(col[2:7], 16)
            b = int(col[7:8])
            c2 = dirs_i[b]
            inp2.append((c2, k2, 0))
        inp = inp2


    v = [(0, 0)]
    x, y = 0, 0
    perimeter = 0
    for (d, k, _) in inp:
        dx, dy = dirs[d]
        x += dx * int(k)
        y += dy * int(k)
        v.append((x, y))
        perimeter += int(k)

    area = shoelace(v)

    # Pick's Theorem
    points_inside = area - perimeter // 2 + 1
    result = points_inside + perimeter
    return result


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines))
    print(solve(lines, True))
