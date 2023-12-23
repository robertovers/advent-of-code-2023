import sys
from functools import cache


def overlaps(b1, b2, check_z):
    a1, c1 = b1
    a2, c2 = b2
    x1, y1, z1 = a1
    x2, y2, z2 = c1
    x3, y3, z3 = a2
    x4, y4, z4 = c2

    if max(x1, x2) < min(x3, x4) or min(x1, x2) > max(x3, x4):
        return False
    elif max(y1, y2) < min(y3, y4) or min(y1, y2) > max(y3, y4):
        return False
    elif check_z:
        if max(z1, z2) < min(z3, z4) or min(z1, z2) > max(z3, z4):
            return False
    return True


def solve(lines):
    bricks = [tuple([tuple(map(int, b.split(",")))
                     for b in (l.strip("\n").split("~"))]) for l in lines]

    bricks = sorted(bricks, key=lambda b: b[0][2])
    settled = []

    # drop the bricks
    for a, b in bricks:
        ax, ay, az = a
        bx, by, bz = b
        if not settled:
            while min(az, bz) > 1:
                az -= 1
                bz -= 1
        else:
            can_fall = True
            while can_fall:
                next = ((ax, ay, az-1), (bx, by, bz-1))
                for c in settled:
                    if overlaps(c, next, True) or min(az, bz) == 1:
                        can_fall = False
                if can_fall:
                    az -= 1
                    bz -= 1
        settled += [((ax, ay, az), (bx, by, bz))]

    above = {}
    below = {}
    for b in settled:
        b1, b2 = b
        sups = []
        for c in settled:
            c1, c2 = c
            if b != c and overlaps(b, c, False) and max(b1[2], b2[2]) == min(c1[2], c2[2])-1:
                sups += [c]
                if c not in below:
                    below[c] = [b]
                else:
                    below[c] += [b]
        above[b] = sups

    def supported_by(a, b, below, memo):
        # check if a is supported by b
        if (a, b) in memo:
            return memo[(a, b)]
        if a not in below.keys():  # has nothing supporting it
            memo[(a, b)] = False
            return False
        elif b in below[a] and len(below[a]) == 1:  # b is only brick supporting a
            memo[(a, b)] = True
            return True
        else:  # check if bricks supporting a are supported by b
            res = True
            for c in below[a]:
                res &= supported_by(c, b, below, memo)
            memo[(a, b)] = res
            return res

    unsafe = []
    for b in settled:
        b1, b2 = b
        sup_by = []
        for c in settled:
            c1, c2 = c
            if b != c and overlaps(b, c, False) and min(b1[2], b2[2]) == max(c1[2], c2[2])+1:
                sup_by += [c]
        if len(sup_by) < 2:
            unsafe += sup_by

    unsafe = list(set(unsafe))
    print(len(settled) - len(unsafe))

    ct = 0
    memo = {}
    for c in unsafe:
        for b in settled:
            if (b, c) in memo:
                sup = memo[(b,c)]
                if sup: ct += 1
            elif c != b and min(b[0][2], b[1][2]) > max(c[0][2], c[1][2]) and supported_by(b, c, below, memo):
                memo[(b, c)] = True
                ct += 1
            else:
                memo[(b, c)] = False

    return ct


if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines))