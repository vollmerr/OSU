#!/usr/bin/env python2
import math


def STOOGESORT(A, d):
    print(d)
    print(A)
    d = d + 1
    n = len(A)
    if n == 2 and A[0] > A[1]:
        A[0],A[1] = A[1],A[0]
    elif n > 2:
        k = int(math.floor((2.0*n)/3.0))
        A[:k] = STOOGESORT(A[:k], d)
        A[n-k:n] = STOOGESORT(A[n-k:n], d)
        A[:k] = STOOGESORT(A[:k], d)
    return A


def QuaternarySearch(A, value, low, high):
    if low <= high:
        q1 = int(low + ((high-low)/4))
        q2 = int(low + ((high-low)/2))
        q3 = int(low + (3*(high-low)/4))
        print(q1)
        print(q2)
        print(q3)
        if A[q1] == value or A[q2] == value or A[q3] == value:
            return True
        elif value < A[q1]:
            print("m1")
            return QuaternarySearch(A, value, low, q1-1)
        elif value < A[q2]:
            print("m2")
            return QuaternarySearch(A, value, q1+1, q2-1)
        elif value < A[q3]:
            print("m3")
            return QuaternarySearch(A, value, q2+1, q3-1)
        else:
            print("high")
            return QuaternarySearch(A, value, q3+1, high)
    return False


def _MIN_AND_MAX(A, low, high):
    if high - low + 1 == 2:
        print("if high-low+1:")
        print(high-low+1)
        if A[low] < A[high]:
            print("if A[low] < A[high]:")
            print(A[low], A[high])
            return A[low], A[high]  # min, max of pair
        else:
            print("NOT if A[low] < A[high]:")
            print(A[low], A[high])
            return A[high], A[low]
    mid = int(math.floor(low+high/2))
    print("mid is:")
    print(mid)
    min_l, max_l = MIN_AND_MAX(A, low, mid)
    min_r, max_r = MIN_AND_MAX(A, mid+1, high)
    if min_l < min_r:
        _min = min_l
        print("if min_l < min_r:")
        print(min_l, min_r)
    else:
        _min = min_r
        print("NOT if min_l < min_r:")
        print(min_l, min_r)
    if max_l > max_r:
        print("if max_l > max_r:")
        print(max_l, max_r)
        _max = max_l
    else:
        print("NOT if max_l > max_r:")
        print(max_l, max_r)
        _max = max_r
    print("returning with:")
    print(_min, _max)
    return (_min,_max)


def MIN_AND_MAX(A, low, high):
    if low == high: # single el
        _max = A[low]
        _min = A[high]
        return (_min, _max)
    elif high == low+1: # pair
        if A[low] > A[high]:
            _max = A[low]
            _min = A[high]
        else:
            _max = A[high]
            _min = A[low]
        return (_min, _max)
    # more than pair
    mid = int((low+high)/2)
    min_l, min_r = MIN_AND_MAX(A, low, mid)
    max_l, max_r = MIN_AND_MAX(A, mid+1, high)
    if min_l < min_r:
        _min = min_l
    else:
        _min = min_r
    if max_l > max_r:
        _max = max_l
    else:
        _max = max_r
    return (_min, _max)


def MAJORITY_ELEMENT(A, start, end):
    if start == end:
        return A[start]
    left = MAJORITY_ELEMENT(A[start:(start + end -1)/2], start, (start + end -1)/2)
    right =  MAJORITY_ELEMENT(A[(start + end -1)/2 + 1:end], (start + end -1)/2 + 1, end)
    if left == None and right == None:
        print("if left == None and right == None:")
        return None
    elif left == None and right != None:
        print("if left == None and right != None:")
        print(right)
        return right
    elif left != None and right != None:
        print("if left != None and right != None:")
        print(left)
        return left
    elif left != right:
        print("if left != right:")
        return None
    return left


def main():
    A = [1,-2,3,3,3,3,3]
    print(MAJORITY_ELEMENT(A, 0, len(A)-1))


if __name__ == "__main__":
    main()
