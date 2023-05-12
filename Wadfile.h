#ifndef WADFILE_H
#define WADFILE_H

#include "Storage.h"

struct Wadfile
{
	int		handle; // TODO: use FILE * for better portability
	char    *pathname;
	Storage *info;
	bool	dirty;

    static Wadfile *initFromFile(const char *path);
    static Wadfile *initNew(const char *path);
    void closeHandle();
    void dealloc();

    int numLumps();
    int lumpsize(int lump);
    int lumpstart(int lump);
    const char *lumpname(int lump);
    int lumpNamed(const char *name);
    void *loadLump(int lump);
    void *loadLumpNamed(const char *name);

    void addName(const char *name, void *data, int size);
    void writeDirectory();
};

#endif /* WADFILE_H */
