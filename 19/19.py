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
        rules_p = []
        for rule in rules:
            if ":" in rule:
                var, op = rule[0], rule[1]
                val, end = rule[2:].split(":") 
                rules_p += [(var, op, int(val), end)]
            else:
                rules_p += [rule]
        workflows[name] = rules_p
        i += 1

    parts = []
    for ln in lines[i+1:]:
        part = {}
        for p in ln[1:len(ln)-2].split(","):
            part[p[0]] = int(p[2:])
        parts += [part]

    res = 0
    for p in parts:
        cur = "in"
        while cur not in "AR":
            rules = workflows[cur]
            end = rules[-1:][0]
            for var, op, val, next in rules[:-1]:
                passed = p[var] < val if op == "<" else p[var] > val
                if passed:
                    cur = next
                    break
            if not passed:
                cur = end
        res += sum(p.values()) if cur == "A" else 0

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

    rs = {"x": (0, 4000), "m": (0, 4000), "a": (0, 4000), "s": (0, 4000)}

    def recur(workflow, rs):
        return

    recur("in", rs)

    return res


if __name__ == "__main__":
    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()
    print(part_one(lines))
    print(part_two(lines))
