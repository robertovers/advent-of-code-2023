import sys


def part_one(lines, steps=1, start=(-1,-1)):
    G = lines
    if start == (-1, -1):
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
        if s == steps:
            return 1+len(Q)

        for dx, dy in ds:
            xn = ux+dx
            yn = uy+dy
            valid = xn in range(len(G[0])) and yn in range(len(G)) and G[yn][xn] != "#"
            if valid and (xn, yn, s+1) not in seen:
                seen[(xn, yn, s+1)] = 1
                Q += [(xn, yn, s+1)]

    return 0


def part_two(lines):
    G = lines
    start = (-1, -1)
    for y in range(len(G)):
        for x in range(len(G[0])):
            if G[y][x] == "S":
                start = (x+131*2, y+131*2, 0)
                break

    # expand the map
    G = [l.strip("\n").replace("S",",")*5 for l in lines]*5

    # map segments are 131 * 131
    # 26501365 % 131 = 65
    g1 = part_one(G, 65, start)
    g2 = part_one(G, 196, start)
    g3 = part_one(G, 327, start)

    print(g1, g2, g3)

    # used WolframAlpha to get a quadratic
    f = lambda x: (14716)*x**2 + (14835)*x + 3734

    # 26501365 - 131 // 65 = 202300
    res = f(202300)

    return res


if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(part_one(lines, 64))
    print(part_two(lines))
