import sys


def part_one(lines):
    G = lines
    start = (-1, -1)
    for y in range(len(G)):
        for x in range(len(G[0])):
            if G[y][x] == "S":
                start = (x, y, 0)
                break

    ds = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    Q = [start]  # (x,y, step)
    seen = {}

    while Q:
        ux, uy, s = Q.pop(0)
        if s == 64:
            return 1+len(Q)

        for dx, dy in ds:
            xn = ux+dx
            yn = uy+dy
            valid = xn in range(len(G[0])) and yn in range(len(G)) and G[yn][xn] != "#"
            if valid and (xn, yn, s+1) not in seen:
                seen[(xn, yn, s+1)] = 1
                Q += [(xn, yn, s+1)]

    return 0


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(part_one(lines))
