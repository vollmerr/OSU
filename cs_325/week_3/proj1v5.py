#!/usr/bin/env python
# -*- coding: utf-8 -*-
import csv
from timeit import default_timer as timer
from random import randint
from math import floor


# Enumeration
def enum(Arr):
    first = last = None       # will store subarray bounds of maxSum
    maxSum = currSum = 0      # stores the overall Maximun Sum and the Current Sum

    # test i through j for all values i and j where i<=j
    for i in range(0, len(Arr)):
        for j in range(i, len(Arr)):

            # sum a range
            currSum = 0
            for k in range(i, j + 1):
                currSum += Arr[k]

            # check if larger than the max
            if currSum > maxSum:
                maxSum = currSum
                first = i
                last = j

    # return max sub array, and the max sum
    return Arr[first:last + 1], maxSum


# Better Enumeration
def betterEnum(Arr):
    first = last = None       # will store subarray bounds of maxSum
    maxSum = currSum = 0      # stores the overall Maximun Sum and the Current Sum

    # test i through j for all values i and j where i<=j
    for i in range(0, len(Arr)):
        # currSum tracks previous sums
        currSum = 0
        for j in range(i, len(Arr)):

            # index by index sum a range
            currSum += Arr[j]

            # for each index check if sum is larger than max
            if currSum > maxSum:
                maxSum = currSum
                first = i
                last = j

    # return max sub array, and the max sum
    return Arr[first:last + 1], maxSum


# Divide and Conquer
# Based off of Introduction to Algorithms (CLRS) pages 70 - 74

# helper for divAndConq
def crossingSub(low, mid, high, Arr):
    # check left crossing half max sub array
    lSum = -float("inf")
    currSum = 0
    for i in range(mid, low - 1, -1):
        currSum += Arr[i]
        if currSum > lSum:
            lSum = currSum
            maxL = i

    # check right crossing half max sub array
    rSum = -float("inf")
    currSum = 0
    for j in range(mid + 1, high + 1):
        currSum += Arr[j]
        if currSum > rSum:
            rSum = currSum
            maxR = j

    # print lSum, rSum
    return maxL, maxR, lSum + rSum


#recursive helper function for divide and conquer
def _divAndConq(low, high, Arr):
    # base case for single element arrays
    if high == low:
        return low, high, Arr[low]

        # recursive calls for left half, right half, and crossing sub arrays
    else:
        # find the mid point
        mid = int(floor((low + high) / 2))

        lLow, lHigh, lSum = _divAndConq(low, mid, Arr)
        rLow, rHigh, rSum = _divAndConq(mid + 1, high, Arr)
        cLow, cHigh, cSum = crossingSub(low, mid, high, Arr)

        # check and return which recuresive call returns the higher sum
        if lSum >= rSum and lSum >= cSum:
            return lLow, lHigh, lSum
        elif rSum >= lSum and rSum >= cSum:
            return rLow, rHigh, rSum
        else:
            return cLow, cHigh, cSum


# entry point for divde and conquer
def divAndConq(Arr):
    if not len(Arr):
        return 0, []
    l, h, s = _divAndConq(0, len(Arr) - 1, Arr)
    return Arr[l:h + 1], s

# Linear Time
# Based off of Kadane's Algorithm: https://www.youtube.com/watch?v=86CQq3pKSUw
def linear(Arr):
    # set current and global maximums to the first element of the array
    currMax = globMax = Arr[0]
    first = currFirst = last = currLast = 0

    # traverse the array and at each index
    for i in range(1, len(Arr)):

        # take the larger of either the current index, or the current index and
        # the current max
        if Arr[i] > Arr[i] + currMax:
            currMax = Arr[i]
            currFirst = currLast = i
        else:
            currMax = Arr[i] + currMax
            currLast = i

        # check for a new global max
        if currMax > globMax:
            globMax = currMax
            first = currFirst
            last = currLast

    return Arr[first:last + 1], globMax


def print_results():
    # open file to read input
    # seperate file into lines and store in x
    inFile = open('MSS_Problems.txt', 'r')
    x = inFile.readlines()

    # create file to write to
    outFile = open('MSS_Results.txt', 'w')

    print "_________________________________________________________"
    # for each line in x
    for i in x:
        # convert the string into ints and store in inArr
        inArr = [int(x) for x in i.split()]
        print "\n..........Next input........"

        print "Enumeration:"
        print inArr
        outFile.write(str(inArr) + '\n')
        testArr, s = enum(inArr)
        print testArr
        outFile.write(str(testArr) + '\n')
        print s
        outFile.write(str(s) + '\n')
        outFile.write('\n')

        print "\nBetter Enumeration:"
        print inArr
        outFile.write(str(inArr) + '\n')
        testArr, s = betterEnum(inArr)
        print testArr
        outFile.write(str(testArr) + '\n')
        print s
        outFile.write(str(s) + '\n')
        outFile.write('\n')

        print "\nDivide and Conquer: "
        print inArr
        outFile.write(str(inArr) + '\n')
        n = int(len(inArr) - 1)
        testArr, s = divAndConq(inArr)
        print testArr
        outFile.write(str(testArr) + '\n')
        print s
        outFile.write(str(s) + '\n')
        outFile.write('\n')

        print "\nLinear-time: "
        print inArr
        outFile.write(str(inArr) + '\n')
        testArr, s = linear(inArr)
        print testArr
        outFile.write(str(testArr) + '\n')
        print s
        outFile.write(str(s) + '\n')
        outFile.write('\n')

    inFile.close()
    outFile.close()


def print_a_csv(func, fname):
    n = 100
    tries_total = 10
    tries_avg = 3
    # write headers to csv file
    with open(fname, 'wb') as f:
        writer = csv.writer(f)
        writer.writerow(['n', 'time'])
        writer.writerow([0, 0])
    print("-") * 50
    print("Starting testing for %s\n" % func.__name__)
    for e in range(tries_total):
        print("\tstarting test %d" % e)
        num = n * (e + 1)
        avg = 0
        # run for average
        for t in range(tries_avg):
            print("\t\ttry %d/%d" % (t, tries_avg - 1))
            start = timer()
            func([randint(-200, 200) for _ in range(num)])
            end = timer()
            avg += (end - start)
        print("\t\t\twriting results: %d, %s\n" % (num, avg / tries_avg))
        # write results to csv file
        with open(fname, 'ab') as f:
            writer = csv.writer(f)
            writer.writerow([num, avg / tries_avg])


def print_all_csv():
    print_a_csv(enum, 'enum.csv')
    print_a_csv(betterEnum, 'betterEnum.csv')
    print_a_csv(divAndConq, 'divAndConq.csv')
    print_a_csv(linear, 'linear.csv')


def main():
    # print_results()
    print_all_csv()


if __name__ == '__main__':
    main()
