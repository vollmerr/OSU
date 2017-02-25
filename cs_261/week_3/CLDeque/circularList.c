#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "circularList.h"

// Double link
struct Link
{
	TYPE value;
	struct Link * next;
	struct Link * prev;
};

struct CircularList
{
	int size;
	struct Link* sentinel;
};

/**
 * Allocates the list's sentinel and sets the size to 0.
 * The sentinel's next and prev should point to the sentinel itself.
 */
static void init(struct CircularList* list)
{
	list->sentinel = (struct Link *)malloc(sizeof(struct Link));
	assert(list->sentinel != 0);
	list->sentinel->next = list->sentinel;
	list->sentinel->prev = list->sentinel;
	list->size = 0;
}

/**
 * Creates a link with the given value and NULL next and prev pointers.
 */
static struct Link* createLink(TYPE value)
{
	struct Link* link = (struct Link *)malloc(sizeof(struct Link));
	assert(link != 0);
	link->value = value;
	link->next = NULL;
	link->prev = NULL;
	return link;
}

/**
 * Adds a new link with the given value after the given link and
 * increments the list's size.
 */
static void addLinkAfter(struct CircularList* list, struct Link* link, TYPE value)
{
	assert(list != 0);
	assert(link != 0);
	struct Link* newLink = createLink(value);
	newLink->next = link->next;
	newLink->prev = link;
	link->next->prev = newLink;
	link->next = newLink;
	list->size++;
}

/**
 * Removes the given link from the list and
 * decrements the list's size.
 */
static void removeLink(struct CircularList* list, struct Link* link)
{
	assert(list != 0);
	assert(link != 0);
	if (!circularListIsEmpty(list)) {
		link->next->prev = link->prev;
		link->prev->next = link->next;
		list->size--;
		free(link);
	}
}

/**
 * Allocates and initializes a list.
 */
struct CircularList* circularListCreate()
{
	struct CircularList* list = malloc(sizeof(struct CircularList));
	init(list);
	return list;
}

/**
 * Deallocates every link in the list and frees the list pointer.
 */
void circularListDestroy(struct CircularList* list)
{
	assert(list != 0);
	struct Link* link = list->sentinel->next;
	struct Link* tmp;
	while (link != list->sentinel) {
		tmp = link->next;
		free(link);
		link = tmp;
	}
	free(list->sentinel);
	free(list);
}

/**
 * Adds a new link with the given value to the front of the deque.
 */
void circularListAddFront(struct CircularList* list, TYPE value)
{
	addLinkAfter(list, list->sentinel, value);
}

/**
 * Adds a new link with the given value to the back of the deque.
 */
void circularListAddBack(struct CircularList* list, TYPE value)
{
	assert(list != 0);
	struct Link* newLink = createLink(value);
	newLink->next = list->sentinel;
	newLink->prev = list->sentinel->prev;
	list->sentinel->prev->next = newLink;
	list->sentinel->prev = newLink;
	list->size++;
}

/**
 * Returns the value of the link at the front of the deque.
 */
TYPE circularListFront(struct CircularList* list)
{
	assert(list != 0);
	return list->sentinel->next->value;
}

/**
 * Returns the value of the link at the back of the deque.
 */
TYPE circularListBack(struct CircularList* list)
{
	assert(list != 0);
	return list->sentinel->prev->value;
}

/**
 * Removes the link at the front of the deque.
 */
void circularListRemoveFront(struct CircularList* list)
{
	assert(list != 0);
	removeLink(list, list->sentinel->next);
}

/**
 * Removes the link at the back of the deque.
 */
void circularListRemoveBack(struct CircularList* list)
{
	assert(list != 0);
	removeLink(list, list->sentinel->prev);
}

/**
 * Returns 1 if the deque is empty and 0 otherwise.
 */
int circularListIsEmpty(struct CircularList* list)
{
	assert(list != 0);
	if (list->size == 0) {
		return 1;
	}
	return 0;
}

/**
 * Prints the values of the links in the deque from front to back.
 */
void circularListPrint(struct CircularList* list)
{
	assert(list != 0);
	struct Link* tmp = list->sentinel;
	while (tmp->next != list->sentinel) {
		printf("%g\n", tmp->next->value);
		tmp = tmp->next;
	}
}

/**
 * Reverses the deque.
 */
void circularListReverse(struct CircularList* list)
{
	assert(list != 0);
	struct Link* cur = list->sentinel;
	struct Link* tmp;
	while (cur->next != list->sentinel) {
		tmp = cur->next;
		cur->next = cur->prev;
		cur->prev = tmp;
		cur = tmp;
	}
	tmp = cur->next;
	cur->next = cur->prev;
	cur->prev = tmp;
}
