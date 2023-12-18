import heapq
import sys


def solve(lines, part2: bool = False):

    inp = [tuple(ln.strip("\n").split(" ")) for ln in lines]

    #inp2 = []
    #for (d, k, col) in inp:
    #    k2 = int(col[2:7], 16)
    #    b = int(col[7:8])
    #    c2 = ""
    #    if b == 0:
    #        c2 = "R"
    #    if b == 1:
    #        c2 = "D"
    #    if b == 2:
    #        c2 = "L"
    #    if b == 3:
    #        c2 = "U"
    #    inp2.append((c2, k2, 0))

    dirs = ["R", "D", "L", "U"]
    xs = [1, 0, -1, 0]
    ys = [0, 1, 0, -1]
    dirc = [0, 0, 0, 0]

    for (d, k, col) in inp:
        for i in range(4):
            if dirs[i] == d: dirc[i] += int(k)

    G = [["." for _ in range(dirc[0]+2)] for _ in range(dirc[1]+2)]

    x, y = 1, 1
    for (d, k, col) in inp:
        i = 0
        while i < int(k):
            G[y][x] = "#"
            for j in range(4):
                if dirs[j] == d:
                    x += xs[j]
                    y += ys[j]
            i += 1

    q = [(0, 0)]

    while q:
        x, y = q.pop()
        if G[y][x] == "x": continue
        if G[y][x] != "#": G[y][x] = "x"

        for i in range(4):
            xn = x + xs[i]
            yn = y + ys[i]
            if xn >= 0 and xn < len(G[0]) and yn >= 0 and yn < len(G):
                if G[yn][xn] != "#":
                    q.append((xn, yn))

    res = 0
    for ln in G:
        for c in ln:
            if c == "x":
                res += 1

    with open("path.txt", "w") as f:
        for ln in G:
            row = ""
            for c in ln:
                row += c
            f.write(row + "\n")

    return len(G) * len(G[0]) - res


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines))
    print(solve(lines, True))
