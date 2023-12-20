import sys
import math
from functools import cache


def send_pulse(nm, frm, pls, modules):
    typ, dsts, state, mem = modules[nm]
    pq = []

    if typ == "%":
        if pls == 0 and state == 0:
            state = 1
            for dst in dsts:
                pq += [(dst, nm, state)]
        elif pls == 0:
            state = 0
            for dst in dsts:
                pq += [(dst, nm, state)]
        modules[nm] = (typ, dsts, state, mem)

        return (pq)

    elif typ == "&":
        mem[frm] = pls
        pls = 0
        for val in mem.values():
            if val == 0:
                pls = 1

    for dst in dsts:
        pq += [(dst, nm, pls)]

    return pq


def parse_modules(lines):
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
        modules[nm] = (typ, dsts, 0, {})

    not_in = []
    # set low state for all conjuction inputs
    for mdl in modules.keys():
        typ, dsts, state, mem = modules[mdl]
        for dst in dsts:
            if dst in modules and modules[dst][0] == "&":
                modules[dst][3][mdl] = 0
            elif dst not in modules:
                not_in += [dst]

    for dst in not_in:
        modules[dst] = ("end", [], 0, {})
    
    return modules


def part_one(lines):

    modules = parse_modules(lines)

    pq = [("bcst", "start", 0)]
    ct = [0, 0] # lo, hi

    for _ in range(1000):
        pq = [("bcst", "start", 0)]
        while pq:
            cur = pq.pop(0)
            ct[cur[2]] += 1
            plss = send_pulse(*cur, modules)
            pq += plss

    return ct[0] * ct[1]


def inputs_of(nm, modules):
    inp = []
    for mdl in modules.keys():
        typ, dsts, state, mem = modules[mdl]
        if nm in dsts:
            inp += [mdl]
    return inp


def part_two(lines):

    modules = parse_modules(lines)    

    inputs = [inputs_of(inp, modules) for inp in inputs_of("rx", modules)][0]
    cycles = {}

    i = 0
    while not all([inp in cycles for inp in inputs]):
        pq = [("bcst", "start", 0)]
        while pq:
            for inp in inputs:
                typ, dsts, state, mem = modules[inp]
                sends_hi = True
                for m in mem.values():
                    sends_hi &= m==0
                if sends_hi and i > 0:
                    if inp not in cycles and i > 0:
                        cycles[inp] = i + 1

            cur = pq.pop(0)
            plss = send_pulse(*cur, modules)
            pq += plss
        
        i += 1

    return math.lcm(*cycles.values())


if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()
    print(part_one(lines))
    print(part_two(lines))
