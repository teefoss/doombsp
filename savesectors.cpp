// savebsp.m

#include "doombsp.h"

Storage		*secdefstore_i;

#define		MAXVERTEX		8192
#define		MAXTOUCHSECS	16
#define		MAXSECTORS		2048
#define		MAXSUBSECTORS	2048

int			vertexsubcount[MAXVERTEX];
short		vertexsublist[MAXVERTEX][MAXTOUCHSECS];

int			subsectordef[MAXSUBSECTORS];
int			subsectornum[MAXSUBSECTORS];

int			buildsector;


/*
==========================
=
= RecursiveGroupSubsector
=
==========================
*/

void RecursiveGroupSubsector (int ssnum)
{
	int				i, l, count;
	int				vertex;
	int				checkss;
	short			*vertsub;
	int				vt;
	mapseg_t		*seg;
	mapsubsector_t	*ss;
	maplinedef_t	*ld;
	mapsidedef_t	*sd;
	
	ss = (mapsubsector_t *)subsecstore_i->elementAt(ssnum);
	subsectornum[ssnum] = buildsector;
	
	for (l=0 ; l<ss->numsegs ; l++)
	{
		seg = (mapseg_t *)maplinestore_i->elementAt(ss->firstseg+l);
		ld = (maplinedef_t *)ldefstore_i->elementAt(seg->linedef);
        DrawLineDef (ld);
		sd = (mapsidedef_t *)sdefstore_i->elementAt(ld->sidenum[seg->side]);
		sd->sector = buildsector;
		
		for (vt=0 ; vt<2 ; vt++)
		{
			if (vt)
				vertex = seg->v1;
			else
				vertex = seg->v2;
			
			vertsub = vertexsublist[vertex];
			count = vertexsubcount[vertex];
			for (i=0 ; i<count ; i++)
			{
				checkss = vertsub[i];
				if (subsectordef[checkss] == subsectordef[ssnum])
				{
					if (subsectornum[checkss] == -1)
					{
						RecursiveGroupSubsector (checkss);
						continue;
					}
					if ( subsectornum[checkss] != buildsector)
						Error ("RecusiveGroup: regrouped a sector");
				}
			}
		}
	}
}

/*
=================
=
= UniqueSector
=
= Returns the sector number, adding a new sector if needed 
=================
*/

int UniqueSector (sectordef_t *def)
{
	int		i, count;
	mapsector_t		ms, *msp;
	
	ms.floorheight = def->floorheight;
	ms.ceilingheight = def->ceilingheight;
	memcpy (ms.floorpic,def->floorflat,8);
	memcpy (ms.ceilingpic,def->ceilingflat,8);
	ms.lightlevel = def->lightlevel;
	ms.special = def->special;
	ms.tag = def->tag;
	
// see if an identical sector already exists
	count = secdefstore_i->count();
	msp = (mapsector_t *)secdefstore_i->elementAt(0);
	for (i=0 ; i<count ; i++, msp++)
		if (!bcmp(msp, &ms, sizeof(ms)))
			return i;

	secdefstore_i->addElement(&ms);
	
	return count;	
}




void AddSubsectorToVertex (int subnum, int vertex)
{
	int		j;
	
	for (j=0 ; j< vertexsubcount[vertex] ; j++)
		if (vertexsublist[vertex][j] == subnum)
			return;
	vertexsublist[vertex][j] = subnum;
	vertexsubcount[vertex]++;
}


/*
================
=
= BuildSectordefs
=
= Call before ProcessNodes
================
*/

void BuildSectordefs (void)
{
	int				i;
	worldline_t		*wl;
	int				count;
	mapseg_t		*seg;
	
//
// build sectordef list
//
    secdefstore_i = Storage::initCount(0, sizeof(mapsector_t), "secdefstore_i");

	count = linestore_i->count();
	wl= (worldline_t *)linestore_i->elementAt(0);
	for (i=0 ; i<count ; i++, wl++)
	{
//		seg = (mapseg_t *)maplinestore_i->elementAt(i); TF: unused in original
		wl->side[0].sector = UniqueSector(&wl->side[0].sectordef);
		if (wl->flags & ML_TWOSIDED)
		{
			wl->side[1].sector = UniqueSector(&wl->side[1].sectordef);
		}
	}
}


/*
================
=
= ProcessSectors
=
= Must be called after ProcessNodes, because it references the subsector list
================
*/

void ProcessSectors (void)
{
	int				i,l;
	int				numss;
	mapsubsector_t	*ss;	
	mapsector_t		sec;
	mapseg_t		*seg;
	maplinedef_t	*ml;
	mapsidedef_t	*ms;
	
//
// build a connection matrix that lists all the subsectors that touch
// each vertex
//
	memset (vertexsubcount, 0, sizeof(vertexsubcount));
	memset (vertexsublist, 0, sizeof(vertexsublist));
	numss = subsecstore_i->count();
	for (i=0 ; i<numss ; i++)
	{
		ss = (mapsubsector_t *)subsecstore_i->elementAt(i);
		for (l=0 ; l<ss->numsegs ; l++)
		{
			seg = (mapseg_t *)maplinestore_i->elementAt(ss->firstseg+l);
			AddSubsectorToVertex (i, seg->v1);
			AddSubsectorToVertex (i, seg->v2);
		}
		subsectornum[i] = -1;		// ungrouped
		ml = (maplinedef_t *)ldefstore_i->elementAt(seg->linedef);
		ms = (mapsidedef_t *)sdefstore_i->elementAt(ml->sidenum[seg->side]);
		subsectordef[i] = ms->sector;
	}
	
//
// recursively build final sectors
//
    secstore_i = Storage::initCount(0, sizeof(mapsector_t), "secstore_i");

	buildsector = 0;
	if (draw)
        SDL_SetRenderDrawColor (renderer_i, 0, 0, 0, 255);
		//PSsetgray (0);
	for (i=0 ; i<numss ; i++)
	{
		if (subsectornum[i] == -1)
		{
            EraseWindow ();
			RecursiveGroupSubsector (i);
			sec = *(mapsector_t *)secdefstore_i->elementAt(subsectordef[i]);
			secstore_i->addElement(&sec);
			buildsector++;
		}
	}

}


