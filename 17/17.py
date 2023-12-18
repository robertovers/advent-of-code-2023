import heapq
import sys


def solve(lines, part2: bool = False):

    # Dijkstra's algorithm

    G = [[int(c) for c in ln.strip("\n")] for ln in lines]

    Q = [(0, 0, 0, -1, 0)]  # (cost, x, y, dir, travelled in dir)
    fin = {}

    while Q:
        (uc, ux, uy, ud, ut) = heapq.heappop(Q)

        if (ux, uy, ud, ut) in fin:
            continue

        fin[(ux, uy, ud, ut)] = uc

        ds = [(1, 0), (0, 1), (-1, 0), (0, -1)]

        for d, (dx, dy) in enumerate(ds):
            xn = ux+dx
            yn = uy+dy
            dtr = (ut+1 if d == ud else 1)
            valid = (dtr < 11 and (d == ud or ut > 3 or ud == -1)) if part2 else (dtr < 4)

            if xn >= 0 and xn < len(G[0]) and yn >= 0 and yn < len(G) and valid:
                if not (d != ud and d % 2 == ud % 2):
                    du = uc + G[yn][xn]
                    heapq.heappush(Q, (du, xn, yn, d, dtr))

    min = -1
    for (x, y, d, t) in fin:
        if x == len(G[0])-1 and y == len(G)-1:
            if min < 0 or fin[(x, y, d, t)] < min:
                min = fin[(x, y, d, t)]

    return min


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines))
    print(solve(lines, True))
