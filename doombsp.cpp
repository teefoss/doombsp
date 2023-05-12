// doombsp.c
#include "doombsp.h"

Wadfile *   wad_i;
bool		draw;

/*
==================
=
= main
=
==================
*/

int main (int argc, char **argv)
{
	char		*inmapname, *scan, *scan2;
	char		outmapname[1024];
	char		basename[80];
	
	memset (outmapname,0,sizeof(outmapname));
	memset (basename,0,sizeof(basename));
	inmapname = NULL;
	
	if (argc == 4)
	{
		if (strcmp(argv[1], "-draw"))
			Error ("doombsp [-draw] inmap outwadpath");
		inmapname = argv[2];
		strcpy (outmapname,argv[3]);
		draw = true;
		//NSApp = [NSApplication new];
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            puts("SDL_Init() failed");
            return 1;
        }
	}
	else if (argc == 3)
	{
		inmapname = argv[1];
		strcpy (outmapname,argv[2]);
		draw = false;
	}
	else
		Error ("doombsp [-draw] inmap outwadpath");
		
	strcat (outmapname,"/");
	scan = inmapname+strlen(inmapname)-1;
	while (*scan != '.' && scan !=inmapname)
		scan--;
	if (scan == inmapname)
		strcpy (basename, inmapname);	// no extension
	else
	{
		scan2 = scan;
		while (*scan != '/' && scan !=inmapname)
			scan--;
		if (scan != inmapname)
			scan++;
		strncpy (basename, scan, scan2-scan);
	}
	
	strcat (outmapname, basename);
	strcat (outmapname,".wad");
	
printf ("output wadfile: %s\n", outmapname);

//
// write a label for the map name at the start of the wadfile
//
	wad_i = Wadfile::initNew(outmapname);
	wad_i->addName(basename, basename, 0);
	
	LoadDoomMap (inmapname);
	DrawMap ();
	BuildBSP ();
	
printf ("segment cuts: %i\n",cuts);

	SaveDoomMap ();
	SaveBlocks ();
	
	wad_i->writeDirectory();
	wad_i->closeHandle();
	wad_i->dealloc();

    if (draw)
    {
        bool wait = true;
        while ( wait ) {
            SDL_Event event;
            while ( SDL_PollEvent(&event) ) {
                if ( event.type == SDL_KEYDOWN || event.type == SDL_QUIT ) {
                    wait = false;
                }
            }

            SDL_Delay(50);
        }
    }
    
    SDL_DestroyRenderer (renderer_i);
    SDL_DestroyWindow (window_i);
    SDL_Quit();

	return 0;
}
