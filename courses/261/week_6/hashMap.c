/*
 * CS 261 Data Structures
 * Assignment 6
 * Name: 
 * Date: 
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
	assert(map);
	HashLink* link, *next;
	for (int i = 0; i < map->capacity; i++) {
		link = map->table[i];
		while (link) {
			next = link->next;
			hashLinkDelete(link);
			link = next;
		}
	}
	map->size = 0;
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
	assert(map);
	int i = HASH_FUNCTION(key) % map->capacity;
	if (i < 0) { i += map->capacity; }
	HashLink* link = map->table[i];
	while (link) {
		if (!strcmp(link->key, key)) {
			return &link->value;
		}
		link = link->next;
	}
    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity. After allocating the new table, all of the links need to be
 * rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
	assert(map);
	int idx;
	HashLink** oldTable = map->table;
	HashLink* link, *next;
	map->table = malloc(sizeof(HashLink*) * capacity);
	assert(map->table != 0);
	for (int i = 0; i < capacity; i++) {
		map->table[i] = NULL;
	}
	for (int i = 0; i < map->capacity; i++) {
		link = oldTable[i];
		while (link) {
			next = link->next;
			idx = HASH_FUNCTION(link->key) % capacity;
			if (idx < 0) { idx += capacity; }
			link->next = map->table[idx];
			map->table[idx] = link;
			link = next;
		}
	}
	map->capacity = capacity;
	free(oldTable);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
	assert(map);		
	int i = HASH_FUNCTION(key) % map->capacity;
	if (i < 0) { i += map->capacity; }
	HashLink* link = map->table[i];
	while (link) {
		if (!strcmp(link->key, key)) {
			link->value = value;
			return;
		}
		if (!link->next) {
			break;
		}
		link = link->next;
	}
	HashLink* newLink = hashLinkNew(key, value, NULL);
	if (link) { link->next = newLink; }
	else { map->table[i] = newLink; }
	map->size++;
	if (hashMapTableLoad(map) > MAX_TABLE_LOAD) { resizeTable(map, map->capacity * 2); }
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
	assert(map);
	int i = HASH_FUNCTION(key) % map->capacity;
	HashLink *link, *prev;
	if (i < 0) { i += map->capacity; }
	link = map->table[i];
	while (link) {
		if (!strcmp(link->key, key)) {
			break;
		}
		link = link->next;
	}
	if (!link) { return; }
	if (link == map->table[i]) { 
		map->table[i] = link->next;
	}
	else {
		prev = map->table[i];
		while (prev && prev->next) {
			if (prev->next == link) {
				prev->next = link->next;
				break;
			}
			prev = prev->next;
		}
	}
	free(link);
	map->size--;
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
	assert(map);
	int i = HASH_FUNCTION(key) % map->capacity;
	if (i < 0) { i += map->capacity; }
	HashLink* link = map->table[i];
	while (link) {
		if (!strcmp(link->key, key)) {
			return 1;
		}
		link = link->next;
	}
	return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
	assert(map);
	return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
	assert(map);
	return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
	assert(map);
	int i, count = 0;
	for (i = 0; i < map->capacity; i++) {
		if (!map->table[i]) {
			count++;
		}
	}
	return count;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
	assert(map);
    return (float)hashMapSize(map) / (float)hashMapCapacity(map);
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}