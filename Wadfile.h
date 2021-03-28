#import <appkit/appkit.h>
#include "Storage.h"

@interface Wadfile : NSObject
{
	int		handle;
	char    *pathname;
	Storage *info;
	BOOL	dirty;
}

- initFromFile: (char const *)path;
- initNew: (char const *)path;
- close;
- (void)free;

- (int)numLumps;
- (int)lumpsize: (int)lump;
- (int)lumpstart: (int)lump;
- (char const *)lumpname: (int)lump;
- (int)lumpNamed: (char const *)name;
- (void *)loadLump: (int)lump;
- (void *)loadLumpNamed: (char const *)name;

- addName: (char const *)name data: (void *)data size: (int)size;
- writeDirectory; 

@end
