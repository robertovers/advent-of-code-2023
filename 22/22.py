import sys
from functools import cache


def overlaps(b1, b2):
    a1, c1 = b1
    a2, c2 = b2
    x1, y1, z1 = a1
    x2, y2, z2 = c1
    x3, y3, z3 = a2
    x4, y4, z4 = c2

    if max(x1, x2) < min(x3, x4) or min(x1, x2) > max(x3, x4):
        return False

    if max(y1, y2) < min(y3, y4) or min(y1, y2) > max(y3, y4):
        return False

    if max(z1, z2) < min(z3, z4) or min(z1, z2) > max(z3, z4):
        return False

    return True


def overlaps_xy(b1, b2):
    a1, c1 = b1
    a2, c2 = b2
    x1, y1, _ = a1
    x2, y2, _ = c1
    x3, y3, _ = a2
    x4, y4, _ = c2

    if max(x1, x2) < min(x3, x4) or min(x1, x2) > max(x3, x4):
        return False

    if max(y1, y2) < min(y3, y4) or min(y1, y2) > max(y3, y4):
        return False

    return True


def solve(lines):
    bricks = [tuple([tuple(map(int, b.split(",")))
                     for b in (l.strip("\n").split("~"))]) for l in lines]

    bricks = sorted(bricks, key=lambda b: b[0][2])
    settled = []

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
                    if overlaps(c, next) or min(az, bz) == 1:
                        can_fall = False
                if can_fall:
                    az -= 1
                    bz -= 1
        settled += [((ax, ay, az), (bx, by, bz))]

    print(len(settled))

    supports = {} # key supports values
    supported_by = {} # key supported by values
    for b in settled:
        sups = []
        for c in settled:
            if b != c and overlaps_xy(b, c) and max(b[0][2], b[1][2]) == min(c[0][2], c[1][2])-1:
                sups += [c]
                if c not in supported_by:
                    supported_by[c] = [b]
                else:
                    supported_by[c] += [b]
        supports[b] = sups

    def depends_on(a, b, supported_by, memo):
        # will a fall if b is pulled 
        if (a, b) in memo:
            return memo[(a, b)]
        if a not in supported_by.keys():
            memo[(a, b)] = False
            return False
        elif b in supported_by[a] and len(supported_by[a]) == 1:
            memo[(a, b)] = True
            return True
        elif b in supported_by[a]:
            memo[(a, b)] = False
            return False
        else:
            res = True
            for c in supported_by[a]:
                res &= depends_on(c, b, supported_by, memo)
            memo[(a, b)] = res
            return res

    cant_dis = []
    for b in settled:
        suppb = []
        for c in settled:
            if b != c and overlaps_xy(b, c) and min(b[0][2], b[1][2]) == max(c[0][2], c[1][2])+1:
                suppb += [c]
        if len(suppb) < 2:
            cant_dis += suppb

    cant_dis = list(set(cant_dis))

    ct = 0
    i = 0
    memo = {}
    for c in cant_dis:
        j = 0
        for b in settled:
            if (b, c) in memo:
                x = memo[(b,c)]
                if x:
                    ct += 1
            elif c != b and min(b[0][2], b[1][2]) > max(c[0][2], c[1][2]) and depends_on(b, c, supported_by, memo):
                memo[(b, c)] = True
                ct += 1
            else:
                memo[(b, c)] = False
            j += 1
        i += 1

    return ct


if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines))
