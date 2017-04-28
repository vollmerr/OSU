#!/usr/bin/env python
# coding=utf-8
###########################################################
#  proj_4.py
#  Ryan Vollmer - CS 325
#  Various solutions to Coin Change problem.
###########################################################


def change_slow(V=[], A=0):
    """ Divide and conquer version of Coin Change problem.
        @param {array}  V   - Values to choose from
        @param {int}    A   - Amount of change to make
        @return {array} C   - Number of coins needed for values of V
        @return {int}   m   - Minimmum number of coins needed
    """
    # base case, A is the min
    if A in V:
        C = [0] * len(V)
        C[V.index(A)] = 1
        return C, 1
    C = {}
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
    return [sum(x) for x in zip(*C)], m


def change_greedy(V=[], A=0):
    """ Greedy solution to finding min number of coins needed to make change.
        Always takes largest possbile coins.
        @param {array}  V   - Values to choose from
        @param {int}    A   - Amount of change to make
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


def change_dp(V=[], A=0):
    """ Dynamic Programing solution to finding min number of coins needed to make change.
        Builds a look up tables of smallest number of coins and their associated coins.
        @param {array}  V   - Values to choose from
        @param {int}    A   - Amount of change to make
        @return {array} C   - Number of coins needed for values of V
        @return {int}   m   - Minimmum number of coins needed
    """
    C = []
    T = []
    T.append(0)
    C.append([0] * len(V))
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
                    # exchange one of larger coin for one of smaller
                    C[v][i] += 1
                    # set as new minimum of this T[v]
                    min_v = cur_v
    return C[A], T[A]


def main():
    V = [1,5,10,12,25,50]
    A = 60
    print("change_slow:\t%s , %d" % change_slow(V, A))
    print("change_greedy:\t%s , %d" % change_greedy(V, A))
    print("change_dp:\t%s , %d" % change_dp(V, A))


if __name__ == '__main__':
    main()
