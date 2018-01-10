from timeit import default_timer as timer
from random import randint
import csv


def mergeSort(alist):
    '''
    based off
    http://interactivepython.org/runestone/static/pythonds/SortSearch/TheMergeSort.html
    @param {array} alist        - list of integers to sort
    '''
    if len(alist) > 1:
        mid = len(alist) // 2
        lefthalf = alist[:mid]
        righthalf = alist[mid:]
        mergeSort(lefthalf)
        mergeSort(righthalf)
        i = 0
        j = 0
        k = 0
        while i < len(lefthalf) and j < len(righthalf):
            if lefthalf[i] < righthalf[j]:
                alist[k] = lefthalf[i]
                i = i + 1
            else:
                alist[k] = righthalf[j]
                j = j + 1
            k = k + 1

        while i < len(lefthalf):
            alist[k] = lefthalf[i]
            i = i + 1
            k = k + 1

        while j < len(righthalf):
            alist[k] = righthalf[j]
            j = j + 1
            k = k + 1


def insertionSort(alist):
    '''
    based off
    http://interactivepython.org/runestone/static/pythonds/SortSearch/TheInsertionSort.html
    @param {array} alist        - list of integers to sort
    '''
    for index in range(1, len(alist)):
        currentvalue = alist[index]
        position = index
        while position > 0 and alist[position - 1] > currentvalue:
            alist[position] = alist[position - 1]
            position = position - 1
        alist[position] = currentvalue


def test(func, fname):
    '''
    runs tests on given sorting alrogithm
    writes results to file
    @param {func} func      - sorting function to runs
    @param {string} fname   - filename to print results to
    '''
    n = 5000
    tries_total = 10
    tries_avg = 3
    # write headers to csv file
    with open(fname, 'wb') as f:
        writer = csv.writer(f)
        writer.writerow(['n', 'time'])
        writer.writerow([0, 0])
    for e in range(tries_total):
        num = n * (e + 1)
        avg = 0
        # run for average
        for t in range(tries_avg):
            start = timer()
            func([randint(0, 100) for _ in range(num)])
            end = timer()
            avg += (end - start)
        print(num, avg / tries_avg)
        # write results to csv file
        with open(fname, 'ab') as f:
            writer = csv.writer(f)
            writer.writerow([num, avg / tries_avg])


def test_worst_best(func, fname, worse):
    '''
    runs tests on given sorting alrogithm for worst or best case
    writes results to file
    @param {func} func      - sorting function to runs
    @param {string} fname   - filename to print results to
    @param {bool} worse     - determines if worse or best case
    '''
    n = 5000
    tries_total = 10
    tries_avg = 3
    # write headers to csv file
    with open(fname, 'wb') as f:
        writer = csv.writer(f)
        writer.writerow(['n', 'time'])
        writer.writerow([0, 0])
    for e in range(tries_total):
        num = n * (e + 1)
        avg = 0
        # run for average
        for t in range(tries_avg):
            nums = sorted([randint(0, 100) for _ in range(num)], reverse=worse)
            start = timer()
            func(nums)
            end = timer()
            avg += (end - start)
        print(num, avg / tries_avg)
        # write results to csv file
        with open(fname, 'ab') as f:
            writer = csv.writer(f)
            writer.writerow([num, avg / tries_avg])


def main():
    # test avg
    test(mergeSort, 'merge_results.csv')
    test(insertionSort, 'insert_results.csv')
    # test best case
    test_worst_best(mergeSort, 'merge_results_best.csv', 0)
    test_worst_best(insertionSort, 'insert_results_best.csv', 0)
    # test worse case
    test_worst_best(mergeSort, 'merge_results_worst.csv', 1)
    test_worst_best(insertionSort, 'insert_results_worst.csv', 1)


if __name__ == "__main__":
    main()
