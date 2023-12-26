import sys
from heapq import heappop, heappush


def min_cut(V, E, a):
    """
    Stoer-Wagner Algorithm
    """
    best = -1
    merges = {}
    for s in V: merges[s] = [s]
    for i in range(len(V)-1):
        V, E, cut, merges, size = min_cut_phase(V, E, a, merges)
        s, t, wt = cut
        print(f"iter {i}/{len(E)}: s={s} t={t} cut={wt}")
        if best == -1 or wt < best:
            best = wt
        if wt == 3:
            return size
    return -1


def min_cut_phase(V, E, a, merges):
    A = [a]
    H = []

    # initialise heap
    for v in V:
        if v in A: continue
        w = E[v][a]
        if w > 0: heappush(H, (-w, v))

    while set(A) != set(V):
        # add the most "tightly connected" node to A
        w, u = heappop(H)
        A += [u]

        # update weights in heap to include edges going to u
        for v, wv in enumerate(E[u]):
            if wv == 0 or v in A: continue
            wh = 0
            for wt, k in H:
                if k == v:
                    wh = wt
                    break
            heappush(H, (wh-E[u][v], v))

    s, t = A[-2], A[-1]  # cut of the phase

    # weight of cut
    wt = sum([E[m][t] for m in V])
    cur_size = len(merges[t])

    # merge the edges s & t into s and add their weights for edges connected to both
    merges[s] = merges[s] + merges[t]
    for m in range(len(E)):
        if m != s:
            E[m][s] = E[s][m]+E[t][m]
            E[s][m] = E[s][m]+E[t][m]

    # remove t
    V.remove(t)
    del merges[t]
    for v in range(len(E)):
        E[v][t] = 0
        E[t][v] = 0

    return V, E, (s, t, wt), merges, cur_size


def solve(lines):

    cms = {}
    for ln in lines:
        nm, ls = ln.split(":")
        ls = ls[1:].strip("\n").split(" ")
        if nm in cms:
            cms[nm] += ls
        else:
            cms[nm] = ls
        for c in ls:
            if c in cms:
                cms[c] += [nm]
            else:
                cms[c] = [nm]

    Vl = sorted(list(cms.keys()))
    V = [i for i in range(len(Vl))]
    E = [[0 for _ in range(len(Vl))] for _ in range(len(Vl))]

    for i, k in enumerate(Vl):
        for j, kc in enumerate(Vl):
            if kc in cms[k]:
                E[i][j] = 1

    size = min_cut(V, E, 1)
    res = size * (len(E) - size)

    return res


if __name__ == "__main__":

    lines = []
    file = sys.argv[1]
    with open(file, "r") as f:
        lines = f.readlines()

    print(solve(lines))