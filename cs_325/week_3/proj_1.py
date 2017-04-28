#!/usr/bin/env python
# coding=utf-8
###########################################################
#  proj_1.py
#  Ryan Vollmer - CS 325
#  Various solutions to max subarray problem.
###########################################################
import math


def max_enum(arr):
    """ Loops through pairs of indicies
        keeping the best sum so far.
        @param  {array} arr     - array to find max sum of
        @return {int}           - sum of max sum
        @return {array}         - subarray of max sum
    """
    arr_len = len(arr)
    max_sum = 0
    max_arr = []
    for i in range(arr_len):
        for j in range(i, arr_len):
            cur_arr = []
            cur_sum = 0
            for k in range(i, j + 1):
                cur_sum = cur_sum + arr[k]
                cur_arr.append(arr[k])
            if cur_sum > max_sum:
                max_sum = cur_sum
                max_arr = cur_arr[:]
    return max_sum, max_arr


def max_memoize(arr):
    """ Loops through pairs of indicies
        keeping the best sum so far.
        Uses memoization to keep track of past values.
        @param  {array} arr     - array to find max sum of
        @return {int}           - sum of max sum
        @return {array}         - subarray of max sum
    """
    arr_len = len(arr)
    max_sum = 0
    max_arr = []
    for i in range(arr_len):
        cur_sum = 0
        cur_arr = []
        for j in range(i, arr_len):
            cur_sum = cur_sum + arr[j]
            cur_arr.append(arr[j])
            if cur_sum > max_sum:
                max_sum = cur_sum
                max_arr = cur_arr[:]
    return max_sum, max_arr


def max_div_conq(arr):
    """ Uses divide and conquer to find maximum subarray
        @param {array} arr      - array to find max sum of
        @return {int}           - sum of max sum
        @return {array}         - subarray of max sum
    """
    if not len(arr):
        return 0, []
    s, l, h = _max_div_conq(arr, 0, len(arr) - 1)
    return s, arr[l:h + 1]


def _max_div_conq(arr, left, right):
    """ Helper function for max_div_conq
        Recursively finds largest max subarray.
        @param {array} arr      - array to find max sum of
        @return {int}           - sum of max sum
        @return {int}           - left index of max array
        @return {int}           - right index of max array
    """
    # base case, single element
    if left == right:
        return arr[left], left, right
    mid = int(math.floor((left + right) / 2))
    # get max of all subarrays
    max_left, low_left, left_high = _max_div_conq(arr, left, mid)
    max_right, low_right, high_right = _max_div_conq(arr, mid + 1, right)
    # find cross section max
    mid_low = mid
    mid_high = mid + 1
    max_mid_left = arr[mid_low]
    max_mid_right = arr[mid_high]
    cur = 0
    for i in range(mid, left - 1, -1):
        cur = cur + arr[i]
        if cur > max_mid_left:
            max_mid_left = cur
            mid_low = i
    cur = 0
    for i in range(mid + 1, right + 1):
        cur = cur + arr[i]
        if cur > max_mid_right:
            max_mid_right = cur
            mid_high = i
    # max in cross section
    if max_mid_left + max_mid_right > max(max_left, max_right):
        return max_mid_left + max_mid_right, mid_low, mid_high
    # max in left section
    if max_left > max_right:
        return max_left, low_left, left_high
    return max_right, low_right, high_right


def max_linear(arr):
    """ Finds max subarray in linear time based off
        the fact a max subarray A[1..j] is eiher a
        max subarray of A[1..j] or a subarray of A[i..j+1]
        for some 1 <= i <= j.
        @param {array} arr      - array to find max sum of
        @return {int}           - sum of max sum
        @return {array}         - subarray of max sum
    """
    max_sum = max_cur = arr[0]
    low = high = tmp = 0
    for i in range(1, len(arr)):
        if arr[i] > max_cur + arr[i]:
            max_cur = arr[i]
            tmp = i
        else:
            max_cur = max_cur + arr[i]
        if max_cur > max_sum:
            max_sum = max_cur
            low = tmp
            high = i
    return max_sum, arr[low:high + 1]


def print_result(func, arr):
    """ Print test case for given function
        @param {func} func      - function to run test on
        @param {array} arr      - array to use in test
    """
    print(arr)
    s, a = func(arr)
    print(a)
    print(s)
    print('\n')


def main():
    # arr = [31, -41, 59, 26, -53, 58, -97, -93, -23, 84]
    with open('MSS_TestProblems.txt') as f:
        for line in f:
            arr = line.split(' ')
            if arr[-1] == '\n':
                del arr[-1]
            arr = [int(x) for x in arr]
            print_result(max_enum, arr)
            print_result(max_memoize, arr)
            print_result(max_div_conq, arr)
            print_result(max_linear, arr)


if __name__ == '__main__':
    main()
