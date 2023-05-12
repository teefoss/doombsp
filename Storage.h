#ifndef STORAGE_H
#define STORAGE_H

struct Storage
{
	uint8_t *data;
	unsigned int elements;
	unsigned int elementSize;
	const char *_description;

    void addElement(void *anElement);
    unsigned int count();
    const char * description();
    void *elementAt(unsigned int index);
    void empty();
    void dealloc();
    static Storage * initCount(unsigned int count,
                               unsigned int elementSizeInBytes,
                               const char *description);
    void insertElement(void *anElement, unsigned int index);
    void removeElementAt(unsigned int index);
    void replaceElementAt(unsigned int index, void *anElement);
};

#endif /* STORAGE_H */
