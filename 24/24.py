import sys
import sympy
from copy import copy


def intersects(ap, av, bp, bv, lo, hi):
    x1, y1, _ = ap
    x3, y3, _ = bp

    axv, ayv, _ = av
    bxv, byv, _ = bv

    t = lo+hi
    x2 = x1 + axv * t
    y2 = y1 + ayv * t
    x4 = x3 + bxv * t
    y4 = y3 + byv * t

    den = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1)
    if den == 0:
        return None

    ua = ((x4-x3) * (y1-y3) - (y4-y3) * (x1-x3)) / den
    if ua < 0 or ua > 1:
        return None

    ub = ((x2-x1) * (y1-y3) - (y2-y1) * (x1-x3)) / den
    if ub < 0 or ub > 1:
        return None

    x = x1 + ua * (x2-x1)
    y = y1 + ua * (y2-y1)

    return min(x, y) >= lo and max(x,y) <= hi


def part_one(lines):

    hs = []
    for ln in lines:
        sp = ln.find("@")
        pos = tuple(map(int, "".join(ln[:sp].split()).split(",")))
        vel = tuple(map(int, "".join(ln[sp+1:].split()).split(",")))
        hs += [(pos, vel)]

    res = 0

    for i in range(len(hs)):
        for j in range(i+1, len(hs)):
            if intersects(*hs[i], *hs[j], 200000000000000, 400000000000000):
                res += 1

    return res


def part_two(lines):

    hs = []
    for ln in lines:
        sp = ln.find("@")
        pos = tuple(map(int, "".join(ln[:sp].split()).split(",")))
        vel = tuple(map(int, "".join(ln[sp+1:].split()).split(",")))
        hs += [(pos, vel)]

    ap, av = hs[0]
    bp, bv = hs[1]
    cp, cv = hs[3]

    ax, ay, az = ap
    bx, by, bz = bp
    cx, cy, cz = cp

    axv, ayv, azv = av
    bxv, byv, bzv = bv
    cxv, cyv, czv = cv

    equations = []

    x = sympy.var("x")
    y = sympy.var("y")
    z = sympy.var("z")
    xv = sympy.var("xv")
    yv = sympy.var("yv")
    zv = sympy.var("zv")
    t1 = sympy.var("t1")
    t2 = sympy.var("t2")
    t3 = sympy.var("t3")

    equations.append(sympy.Eq(x + xv * t1, ax + axv * t1))
    equations.append(sympy.Eq(y + yv * t1, ay + ayv * t1))
    equations.append(sympy.Eq(z + zv * t1, az + azv * t1))
    equations.append(sympy.Eq(x + xv * t2, bx + bxv * t2))
    equations.append(sympy.Eq(y + yv * t2, by + byv * t2))
    equations.append(sympy.Eq(z + zv * t2, bz + bzv * t2))
    equations.append(sympy.Eq(x + xv * t3, cx + cxv * t3))
    equations.append(sympy.Eq(y + yv * t3, cy + cyv * t3))
    equations.append(sympy.Eq(z + zv * t3, cz + czv * t3))

    ans = sympy.solve(equations)[0]
    res = ans[x] + ans[y] + ans[z]

    return res


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(part_one(lines))
    print(part_two(lines))
