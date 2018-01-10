#!/usr/bin/env python
# coding=utf-8
###################################################################
#  Group Project 3 - CS 325
#  Ryan Vollmer, Clarence Pine, Parker Howell
#  Various solutions to TSP.
###################################################################

import sys
import math
import random
import christofides from './christofides'

###################################################################
#  City - creates a city object
#  cityID - the ID number of the city
#  x - the x coordinate value of the city
#  y - the y coordinate value of the city
###################################################################
class City(object):
	def __init__(self, cityID, x, y):
		self.cityID = cityID
		self.x = x
		self.y = y




###################################################################
#  dist
#  a - city object with x and y attributes
#  b - city object with x and y attributes
#  Basic Pythagorean Theorem, takes two city objects and returns
#  their hypotenuse distance rounded to nearest integer.
###################################################################
def dist(a, b):
	return round(math.hypot(b.x-a.x, b.y-a.y))





###################################################################
#  singleSpace
#  string - the string to edit
#  loops through a entered string and reduces any multiple white
#  spaces with just single spaces. Returns the single spaced string
#  e.g.  string = "the    fox    jumps."  =>   "the fox jumps."
###################################################################
def singleSpace(string):
	while '  ' in string:
		string = string.replace("  ", " ")
	return string





###################################################################
#  get_input
#  inFileName - name of input file
#  opens the file with the filename provided and processes it to
#  create and return a list of city objects to be evaluated in
#  solving our TSP problems.
###################################################################
def get_input(inFileName):
	with open(inFileName, "r") as in_f:
		inputData = [line.strip() for line in in_f]
		cities = []
		for city in inputData:
			city = singleSpace(city)
			cityID, x, y = city.split(' ')
			cities.append(City(int(cityID), int(x), int(y)))
	return cities





###################################################################
#  make_tsp
#  inputCities - list of city objects
#  inFileName - name of input file
#  evaluates inFileName and calls the appropriate algoritm on the
#  inputCities list.
###################################################################
def make_tour(inputCities, inFileName):
	#if (inFileName == "tsp_example_1.txt"):
	length, tour = nearNeighbor(inputCities)

	# if we need to run different algos on different problems...
	elif(inFileName == "tsp_example_2.txt" || "competition_5.txt"):
		length, tour = christofides(inputCities)

	#elif(inFileName == "tsp_example_3.txt" || "competition_2.txt"):
	#	length, tour = genetic(inputCities)

	# etc...

	#else print "Error: The input file name DOES NOT match expected file names."

	return length, tour





###################################################################
#  make_output
#  length - the length of the tour
#  tour - list of visited city objects in the order visited
#  inFileName - name of input file
#  Creates an output file based on the name of the input file and
#  then writes the length of the tour followed by each city of the
#  tour to that output file.
###################################################################
def make_output(length, tour, inFileName):
	outFileName = inFileName + ".tour"
	with open(outFileName, "w") as out_f:
		out_f.write(str(length) + "\n")
		for city in tour:
			out_f.write(str(city.cityID) + "\n")





###################################################################
#  findNext
#  origin - city to compare other city distances to
#  remainingCities - list of cities to compare to origin
#  Compares each city in remainingCities to the origin, saving and
#  returning the city closest to the origin, and the distance
#  between them.
###################################################################
def findNext(origin, remainingCities):
	currShort = float("inf")
	for city in remainingCities:
		currDist = dist(origin, city)
		if currDist < currShort:
			currCity = city
			currShort = currDist
	return currCity, currShort





###################################################################
#  nearNeighbor
#  inputCities - list of cities to evaluate
#  This function takes a list of cities and attempts to effeciently
#  solve the TSP for the list. At each step of the algorithm we
#  evaluate, and go to the nearest neighbor of the current city.
#  Different starting locatins may result in differnt length tours.
#  function returns the length of the tour, and a in-order list of
#  visited ities.
###################################################################
def nearNeighbor(inputCities):
	length = 0
	tour = []
	curr = inputCities[random.randrange(0, len(inputCities))]
	tour.append(curr)
	inputCities.remove(curr)

	while inputCities:
		next, trav = findNext(curr, inputCities)
		tour.append(next)
		inputCities.remove(next)
		length += trav
		curr = next

	length += dist(tour[0], tour[len(tour) - 1])

	return length, tour





###################################################################
#  main
#  inFileName - name of the input file
#  i do things...
###################################################################
def main(inFileName):
	#print inFileName
	inputCities = get_input(inFileName)
	length, tour = make_tour(inputCities, inFileName)
	make_output(length, tour, inFileName)




###################################################################
#  calls main with the name of the file to run TSP on
###################################################################
if __name__ == '__main__':
   main(sys.argv[1])
