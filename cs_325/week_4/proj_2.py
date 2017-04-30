#!/usr/bin/env python
# coding=utf-8
###########################################################
#  proj_2.py
#  Ryan Vollmer - CS 325
#  Various solutions to Coin Change problem.
###########################################################


def change_slow(V, A):
    """ Divide and conquer version of Coin Change problem.
        @param  {array} V   - Values to choose from
        @param  {int}   A   - Amount of change to make
        @return {array} C   - Number of coins needed for values of V
        @return {int}   m   - Minimmum number of coins needed
    """
    # base case, A is the min
    if A in V:
        C = [0] * len(V)
        C[V.index(A)] = 1
        return C, 1
    m = float("inf")
    # else for each i < A
    for i in xrange(1, A):
        # 1) find min coins to make i cents
        C_i, m_i = change_slow(V, i)
        # 2) find min coins to make A - i cents
        C_k, m_k = change_slow(V, A - i)
        # if smallest, save for later lookup
        if m_i + m_k < m:
            m = m_i + m_k
            C = (C_i, C_k)
    # choose i that minimizes sum of 1) and 2)
    a = [sum(x) for x in zip(*C)]
    return a, m


"""
    Pseudocode for change_slow

    CHANGE_SLOW(V=array, A=int)
        if A in V
            C = array of 0's, length of V.length
            C[index of A in V] = 1
            return C, 1
        m = infinity
        for i=1 to A
            C_i, m_i = CHANGESLOW(V, i)
            C_k, m_k = CHANGESLOW(V, A- i)
            if m_i + m_k < m
                m = m_i + m_k
                C_1 = C_i
                C_2 = C_k
        return sum(C_1, C_2), m
"""


def change_greedy(V, A):
    """ Greedy solution to finding min number of coins needed to make change.
        Always takes largest possbile coins.
        @param  {array} V   - Values to choose from
        @param  {int}   A   - Amount of change to make
        @return {array} C   - Number of coins needed for values of V
        @return {int}   m   - Minimmum number of coins needed
    """
    C = [0] * len(V)
    m = 0
    for i in xrange(len(V) - 1, -1, -1):
        while A >= V[i]:
            C[i] += 1
            m += 1
            A -= V[i]
    return C, m


"""
    Pseudocode for change_greedy

    CHANGE_GREEDY(V=array, A=int)
        C = array of 0's, length of V.length
        m = 0
        for i=V.length-1 to 0
            while A >= V[i]
                C[i] = C[i] + 1
                m = m + 1
                A = A - V[i]
        return C, m
"""


def change_dp(V, A):
    """ Dynamic Programing solution to finding min number of coins needed to make change.
        Builds a look up tables of smallest number of coins and their associated coins.
        @param  {array} V   - Values to choose from
        @param  {int}   A   - Amount of change to make
        @return {array} C   - Number of coins needed for values of V
        @return {int}   m   - Minimmum number of coins needed
    """
    C = [[0] * len(V)]
    T = [0]
    # build table
    for v in xrange(1, A + 1):
        T.append(0)
        C.append([0] * len(V))
        min_v = float("inf")
        for i in xrange(len(V)):
            # smaller than value, find min
            if V[i] <= v:
                # lookup number of coins needed
                cur_v = T[v - V[i]] + 1
                # smaller than smallest, store for later
                if cur_v < min_v:
                    # set number of coins in table
                    T[v] = cur_v
                    # set coin array to min one looked up
                    C[v] = C[v - V[i]][:]
                    # add one of current coin
                    C[v][i] += 1
                    # set as new minimum of this T[v]
                    min_v = cur_v
    return C[A], T[A]


"""
    Pseudocode for change_dp

    CHANGE_DP(V=array, A=int)
        C = array of 0's, length of V.length
        T = [0]
        for v=0 to A
            min_v = infinity
            for i=0 to V.length-1
                if V[i] <= v
                    cur_v = T[v - V[i]] + 1
                    if cur_v < min_v
                        T[v] = cur_v
                        C[v] = C[v - V[i]]
                        C[v][i] = C[v][i] + 1
                        min_v = cur_v
        return C[A], T[A]
"""


def from_file():
    """ Prompts for input file, runs any functions in 'funcs' with
        files input, then prints results to [input_file_name]change.txt
        Requires input file to be .txt with following format repeated:
            1 int int     # coins to use, variable number of ints in increasing order
            int           # amount to make change for, single number
    """
    funcs = [change_slow, change_greedy, change_dp]
    in_name = raw_input("File name:")
    if in_name[-4:] != ".txt":
        print("Bad file type, must be a .txt")
        return 1
    out_name = "%schange.txt" % in_name.replace(".txt", "")
    print("Writing results to: %s" % out_name)
    with open(in_name, "r") as in_f:
        with open(out_name, "w") as out_f:
            for func in funcs:
                in_f.seek(0, 0)
                out_f.write("Algorithm %s:\n" % func.__name__)
                print("\nRunning %s..." % func.__name__)
                while True:
                    V = in_f.readline()
                    A = in_f.readline()
                    if not A:
                        break
                    V = [int(x) for x in V.split()]
                    A = int(A.split()[0])
                    print("\tfinding min using %s for %d" % (V, A))
                    C, m = func(V, A)
                    out_f.write("%s\n" % V)
                    out_f.write("%s\n" % C)
                    out_f.write("%d\n" % m)
                    print("\t\tfound min using %s with %d coins" % (C, m))


def main():
    from_file()


if __name__ == '__main__':
    main()
