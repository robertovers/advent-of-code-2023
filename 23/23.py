import sys
from copy import copy


def solve(lines, part2: bool = False):
    dm = {">": 0, "v": 1, "<": 2, "^": 3}
    ds = [(1, 0), (0, 1), (-1, 0), (0, -1)]

    G = [ln.strip("\n") for ln in lines]
    Q = [(0, 1, 0, {})]  # (cost, x, y, visited)

    res = 0
    i = 0
    while Q:
        if (i % 10000 == 0):
            print(i, res)
        i += 1
        uc, ux, uy, vis = Q.pop()
        vis[(ux, uy)] = 1

        if uy == len(G)-1 and ux == (len(G[0])-2):
            if uc > res:
                res = uc

        for d, (dx, dy) in enumerate(ds):
            if not part2 and G[uy][ux] in dm:
                dn = dm[G[uy][ux]]
                if dn != d:
                    continue
            xn = ux+dx
            yn = uy+dy
            if xn in range(len(G[0])) and yn in range(len(G)):
                if G[yn][xn] != "#" and (xn, yn) not in vis:
                    du = uc + 1
                    Q += [(du, xn, yn, copy(vis))]

    return res


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines, True))
