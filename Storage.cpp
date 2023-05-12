// borrowed from https://github.com/fragglet/DoomEd (Simon Howard)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Storage.h"

void Storage::addElement(void *anElement)
{
	// Equivalent to insert at the end:
	insertElement(anElement, elements);
}

unsigned int Storage::count()
{
	return elements;
}

const char *Storage::description()
{
	return _description;
}

void *Storage::elementAt(unsigned int index)
{
	if (index >= elements)
	{
		return NULL;
	}

	return data + elementSize * index;
}

void Storage::empty()
{
	elements = 0;
}

void Storage::dealloc()
{
	free(data);
	data = NULL;
}

Storage * Storage::initCount(unsigned int count,
                             unsigned int elementSizeInBytes,
                             const char *description)
{
    Storage * storage = (Storage *)malloc(sizeof(*storage));
    if ( storage == NULL )
        printf("failed to allocate Storage (%s)\n", description);

    storage->_description = description;
    storage->elementSize = elementSizeInBytes;

    storage->elements = count;
    if ( count != 0 ) {
        storage->data = (uint8_t *)calloc(count, elementSizeInBytes);
    } else {
        storage->data = NULL;
    }


	return storage;
}

void Storage::insertElement(void *anElement, unsigned int index)
{
	// Sanity check insert range; a maximum value of 'elements' is
	// okay to insert at the end of the array.
	if (index > elements)
	{
		return;
	}

	// Increase array size and move the latter part of the array
	// down by one.
    if ( elements == 0 ) {
        data = (uint8_t *)malloc(elementSize);
    } else {
        data = (uint8_t *)realloc(data, elementSize * (elements + 1));
    }

	memmove(data + elementSize * (index + 1),
	        data + elementSize * index,
	        elementSize * (elements - index));

	// Copy in the new element.
	memmove(data + elementSize * index, anElement, elementSize);
	++elements;
}

void Storage::removeElementAt(unsigned int index)
{
	if (index >= elements)
	{
		return;
	}

	// Move latter half of array down towards the start, and decrement
	// the array size.
	memmove(data + elementSize * index,
	        data + elementSize * (index + 1),
	        elementSize * (elements - index - 1));
	--elements;
}

void Storage::replaceElementAt(unsigned int index, void *anElement)
{
	if (index >= elements)
	{
		return;
	}

	memmove(data + elementSize * index, anElement, elementSize);
}
