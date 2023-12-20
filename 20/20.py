import sys
from functools import cache


def send_pulse(nm, frm, pls, modules):
    typ, dsts, state, mem = modules[nm]
    pq = []

    if typ == "%":
        if pls == 0:
            if state == 0:
                state = 1
                for dst in dsts:
                    pq += [(dst, nm, 1)]
                modules[nm] = (typ, dsts, state, mem)
            else:
                state = 0
                for dst in dsts:
                    pq += [(dst, nm, 0)]
                modules[nm] = (typ, dsts, state, mem)

        return (pq)

    elif typ == "&":
        mem[frm] = pls
        pls = 0
        for val in mem.values():
            if val == 0:
                pls = 1
        for dst in dsts:
            if pls == 1:
                pq += [(dst, nm, pls)]
            else:
                pq += [(dst, nm, pls)]
    elif dsts:
        for dst in dsts:
            if pls == 1:
                pq += [(dst, nm, pls)]
            else:
                pq += [(dst, nm, pls)]

    return pq


def solve(lines):

    modules = {}

    for ln in lines:
        ax = ln.find("->")
        dsts = ln[ax+2:-1].strip(' ').split(', ')
        if ln[0] == "b":
            modules["bcst"] = ("bcst", dsts, 0, {})
            continue
        typ = ln[0]
        nm = ln[1:ax-1]
        # (type, destinations, state, memory)
        modules[nm] = (typ, dsts, 0, {})  # 0 = off or low, 1 = on or high

    not_in = []
    # set low state for all conjuctions
    for mdl in modules.keys():
        typ, dsts, state, mem = modules[mdl]
        for dst in dsts:
            if dst in modules and modules[dst][0] == "&":
                modules[dst][3][mdl] = 0
            elif dst not in modules:
                not_in += [dst]

    for dst in not_in:
        modules[dst] = ("end", [], 0, {})

    # (to, from, pls, lo, hi)
    pq = [("bcst", "start", 0)]
    ct = [0, 0] # lo, hi

    for _ in range(1000):
        pq = [("bcst", "start", 0)]
        while pq:
            cur = pq.pop(0)
            if cur[0] == "rx" and cur[2] == 0:
                return i
            ct[cur[2]] += 1
            plss = send_pulse(cur[0], cur[1], cur[2], modules)
            pq += plss

    return ct[0] * ct[1]


if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()
    print(solve(lines))
