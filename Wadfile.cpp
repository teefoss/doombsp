#include "doombsp.h"
#include "Wadfile.h"
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
	char    identification[4];		// should be IWAD
	int		numlumps;
	int		infotableofs;
} wadinfo_t;


typedef struct
{
	int		filepos;
	int		size;
	char    name[8];
} lumpinfo_t;


//=============================================================================

/*
============
=
= initFromFile:
=
============
*/

Wadfile * Wadfile::initFromFile(const char *path)
{
    Wadfile *newWad = (Wadfile *)malloc(sizeof(*newWad));

	wadinfo_t	wad;
	lumpinfo_t	*lumps;
	int			i;
	
	newWad->pathname = (char *)malloc(strlen(path)+1);
	strcpy (newWad->pathname, path);
    newWad->dirty = false;
	newWad->handle = open (newWad->pathname, O_RDWR, 0666);
    if ( newWad->handle == -1 ) {
        newWad->dealloc();
        return NULL;
    }

//
// read in the header
//
	read (newWad->handle, &wad, sizeof(wad));
	if (strncmp(wad.identification,"IWAD",4))
	{
		close (newWad->handle);
        newWad->dealloc();
		return NULL;
	}
	wad.numlumps = LONG (wad.numlumps);
	wad.infotableofs = LONG (wad.infotableofs);
	
//
// read in the lumpinfo
//
	lseek (newWad->handle, wad.infotableofs, L_SET);
    newWad->info = Storage::initCount(wad.numlumps, sizeof(lumpinfo_t), "Wadfile::info");
	lumps = (lumpinfo_t *)newWad->info->elementAt(0);
	
	read (newWad->handle, lumps, wad.numlumps*sizeof(lumpinfo_t));
	for (i=0 ; i<wad.numlumps ; i++, lumps++)
	{
		lumps->filepos = LONG (lumps->filepos);
		lumps->size = LONG (lumps->size);
	}
	
	return newWad;
}


/*
============
=
= initNew:
=
============
*/

Wadfile *Wadfile::initNew(const char *path)
{
    Wadfile *newWad = (Wadfile *)malloc(sizeof(*newWad));

    if (newWad == NULL)
        Error("Could not allocate wad file");

    wadinfo_t	wad;

    newWad->pathname = (char *)malloc(strlen(path)+1);
    strcpy (newWad->pathname, path);
    newWad->info = Storage::initCount(0, sizeof(lumpinfo_t), "Wadfile::info");
    newWad->dirty = true;
    newWad->handle = open (newWad->pathname, O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (newWad->handle== -1) {
        printf("could not open wad handle\n");
        return NULL;
    }

    // leave space for wad header
    write (newWad->handle, &wad, sizeof(wad));

    return newWad;
}

void Wadfile::closeHandle()
{
	close (handle);
}

void Wadfile::dealloc()
{
	close (handle);
	info->dealloc();
	free (pathname);
}

//=============================================================================

int Wadfile::numLumps()
{
	return info->count();
}

int Wadfile::lumpsize(int lump)
{
	lumpinfo_t	*inf;
	inf = (lumpinfo_t *)info->elementAt(lump);
	return inf->size;
}

int Wadfile::lumpstart(int lump)
{
	lumpinfo_t	*inf;
	inf = (lumpinfo_t *)info->elementAt(lump);
	return inf->filepos;
}

const char *Wadfile::lumpname(int lump)
{
	lumpinfo_t	*inf;
	inf = (lumpinfo_t *)info->elementAt(lump);
	return inf->name;
}

/*
================
=
= lumpNamed:
=
================
*/

int Wadfile::lumpNamed(const char *name)
{
	lumpinfo_t	*inf;
	int			i, count;
	char			name8[9];
	int			v1,v2;

// make the name into two integers for easy compares

	memset(name8,0,9);
	if (strlen(name) < 9)
		strncpy (name8,name,9);
	for (i=0 ; i<9 ; i++)
		name8[i] = toupper(name8[i]);	// case insensitive

	v1 = *(int *)name8;
	v2 = *(int *)&name8[4];


// scan backwards so patch lump files take precedence

	count = info->count();
	for (i=count-1 ; i>=0 ; i--)
	{
		inf = (lumpinfo_t *)info->elementAt(i);
		if ( *(int *)inf->name == v1 && *(int *)&inf->name[4] == v2)
			return i;
	}
	return  -1;
}

/*
================
=
= loadLump:
=
================
*/

void *Wadfile::loadLump(int lump)
{
	lumpinfo_t	*inf;
	byte			*buf;
	
	inf = (lumpinfo_t *)info->elementAt(lump);
	buf = (byte *)malloc (inf->size);
	
	lseek (handle, inf->filepos, L_SET);
	read (handle, buf, inf->size);
	
	return buf;
}

void *Wadfile::loadLumpNamed(const char *name)
{
	return loadLump(lumpNamed(name));
}

//============================================================================

/*
================
=
= addName:data:size:
=
================
*/

void Wadfile::addName(const char *name, void *data, int size)
{
	int		i;
	lumpinfo_t	newLump;
	
	dirty = true;
	memset (newLump.name,0,sizeof(newLump.name));
	strncpy (newLump.name, name, 8);
	for (i=0 ; i<8 ; i++)
        newLump.name[i] = toupper(newLump.name[i]);
    newLump.filepos = lseek(handle,0, L_XTND);
    newLump.size = size;
	info->addElement(&newLump);
	
	write (handle, data, size);
}


/*
================
=
= writeDirectory:
=
	char		identification[4];		// should be IWAD
	int		numlumps;
	int		infotableofs;
================
*/

void Wadfile::writeDirectory()
{
	wadinfo_t	wad;
	int			i,count;
	lumpinfo_t	*inf;
	
//
// write the directory
//
	count = info->count();
	inf = (lumpinfo_t *)info->elementAt(0);
	for (i=0 ; i<count ; i++)
	{
		inf[i].filepos = LONG (inf[i].filepos);
		inf[i].size = LONG (inf[i].size);
	}
	wad.infotableofs = LONG (lseek(handle,0, L_XTND));
	write (handle, inf, count*sizeof(lumpinfo_t));
	for (i=0 ; i<count ; i++)
	{
		inf[i].filepos = LONG (inf[i].filepos);
		inf[i].size = LONG (inf[i].size);
	}
	
//
// write the header
//
	strncpy (wad.identification, "IWAD",4);
	wad.numlumps = LONG (info->count());
	lseek (handle, 0, L_SET);
	write (handle, &wad, sizeof(wad));
}

