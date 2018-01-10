#!/usr/bin/python3
""" FILE:       mypython.py
    AUTHOR:     Ryan Vollmer
    PURPOSE:    Python exploration that performs the following:
                    - generates 3 files with randomly generated characters
                    - displays the files contents
                    - displays two random numbers and their product
"""
import string
import random

def gen_chars(chars=10):
    """ generates random lowercased string
    """
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(chars))

def gen_files(files=3):
    """ generates files with random charaters
    """
    chars = [gen_chars() for _ in range(files)];    # generate list of random chars
    for x in chars:                                 # loop through chars
        with open('file_{0}'.format(x), 'w+') as f: # create/open file
            f.write('{0}\n'.format(x))              # write chars to file
        print(x)                                    # print chars written

def print_nums(nums=2, min=1, max=42):
    """ prints random numbers and their sum
    """
    sum = 0                                 # init sum
    for _ in range(nums):                   # loop through nums times
        x = random.randint(min, max + 1)    # gen random int in (min, max]
        print(x)                            # print number
        sum += x                            # add to sum
    print(sum)                              # print sum

def main():
    gen_files()
    print_nums()

if __name__ == '__main__':
    main()
