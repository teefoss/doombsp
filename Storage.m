// borrowed from https://github.com/fragglet/DoomEd
#include <stdlib.h>
#include <string.h>

#import "Storage.h"

@implementation Storage

- (void) addElement:(void *)anElement
{
	// Equivalent to insert at the end:
	[self insertElement: anElement at: elements];
}

- (unsigned int)count
{
	return elements;
}

- (const char *)description
{
	return description;
}

- (void *)elementAt:(unsigned int)index
{
	if (index >= elements)
	{
		return NULL;
	}

	return data + elementSize * index;
}

- (void) empty
{
	elements = 0;
}

- (void) dealloc
{
	free(data);
	data = NULL;
	[super dealloc];
}

- (Storage *) initCount:(unsigned int)count
                        elementSize: (unsigned int) sizeInBytes
                        description: (const char *) string
{
	description = string;
	elementSize = sizeInBytes;

	elements = count;
	data = calloc(count, sizeInBytes);

	return self;
}

- (void) insertElement:(void *)anElement at:(unsigned int)index
{
	// Sanity check insert range; a maximum value of 'elements' is
	// okay to insert at the end of the array.
	if (index > elements)
	{
		return;
	}

	// Increase array size and move the latter part of the array
	// down by one.
	data = realloc(data, elementSize * (elements + 1));
	memmove(data + elementSize * (index + 1),
	        data + elementSize * index,
	        elementSize * (elements - index));

	// Copy in the new element.
	memmove(data + elementSize * index, anElement, elementSize);
	++elements;
}

- (void) removeElementAt:(unsigned int)index
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

- (void) replaceElementAt:(unsigned int)index with:(void *)anElement
{
	if (index >= elements)
	{
		return;
	}

	memmove(data + elementSize * index, anElement, elementSize);
}

@end

