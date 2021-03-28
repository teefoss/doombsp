// cmdlib.h

#ifndef __CMDLIB__
#define __CMDLIB__

#ifndef __BYTEBOOL__
#define __BYTEBOOL__
typedef unsigned char byte;
#endif

#ifdef __NeXT__

#define O_BINARY        0
#define PATHSEPERATOR   '/'

#endif

void	Error (char *error, ...);

int 	SafeOpenWrite (char *filename);
void 	SafeWrite (int handle, void *buffer, long count);

#endif
