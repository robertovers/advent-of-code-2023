import sys


def part_one(lines):
    workflows = {}
    i = 0
    while lines[i] != "\n":
        ln = lines[i][:len(lines[i])-2]
        br = ln.find("{")  # }
        name = ln[0:br]
        rules = ln[br+1:].strip("\n").split(",")
        rules = [r[:len(r)] for r in rules]
        workflows[name] = rules
        i += 1

    parts = []
    for ln in lines[i+1:]:
        part = {}
        for p in ln[1:len(ln)-2].split(","):
            part[p[0]] = int(p[2:])
        parts += [part]

    res = 0
    for p in parts:
        r = 0
        current = workflows["in"]
        while r < len(current):
            rule = current[r]
            if rule == "R":
                break
            elif rule == "A":
                res += sum(p.values()) 
                break

            if ":" in rule:
                var, op = rule[0], rule[1]
                val, end = rule[2:].split(":") 
            else:
                current = workflows[current[r]]
                r = 0
                continue

            passed = p[var] < int(val) if op == "<" else p[var] > int(val)
            if passed:
                r = 0
                if end == "A" or current[r] == "A":
                    res += sum(p.values())
                    break
                elif end != "R":
                    current = workflows[end]
            else:
                r += 1

    return res


def part_two(lines):
    workflows = {}
    i = 0
    while lines[i] != "\n":
        ln = lines[i][:len(lines[i])-2]
        br = ln.find("{")  # }
        name = ln[0:br]
        rules = ln[br+1:].strip("\n").split(",")
        rules = [r[:len(r)] for r in rules]
        workflows[name] = rules
        i += 1

    parts = []
    for ln in lines[i+1:]:
        part = {}
        for p in ln[1:len(ln)-2].split(","):
            part[p[0]] = int(p[2:])
        parts += [part]

    res = 0

    return res





if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()
    print(part_one(lines))
    print(part_two(lines))
