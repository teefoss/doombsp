// cmdlib.c

#include "cmdlib.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define O_BINARY        0

/*
=============================================================================

						MISC FUNCTIONS

=============================================================================
*/

/*
=================
=
= Error
=
= For abnormal program terminations
=
=================
*/

void Error (const char *error, ...)
{
	va_list argptr;

	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");
	exit (1);
}





int SafeOpenWrite (char *filename)
{
	int     handle;

	handle = open(filename,O_RDWR | O_BINARY | O_CREAT | O_TRUNC
	, 0666);

	if (handle == -1)
		Error ("Error opening %s: %s",filename,strerror(errno));

	return handle;
}

int SafeOpenRead (char *filename)
{
	int     handle;

	handle = open(filename,O_RDONLY | O_BINARY);

	if (handle == -1)
		Error ("Error opening %s: %s",filename,strerror(errno));

	return handle;
}



void SafeWrite (int handle, void *buffer, long count)
{
	unsigned        iocount;

	while (count)
	{
		iocount = count > 0x8000 ? 0x8000 : count;
		if (write (handle,buffer,iocount) != iocount)
			Error ("File write failure");
		buffer = (void *)( (byte *)buffer + iocount );
		count -= iocount;
	}
}
