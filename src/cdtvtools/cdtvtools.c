#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "exec/types.h"
#include "exec/execbase.h"
#include "exec/memory.h"
#include "clib/alib_protos.h"
#include "devices/cdtv/bookmark.h"
#include "cdtvtools.h"
#include "rev.h"


#define CDTVLAND_MFID 0x434c
#define CDTVLAND_SYSPREFS_PRODID 0x0001

void usage (void) {
    printf ("Usage:\n\n CDTVTools [HDDBOOTDELAY <seconds>] [PAL|NTSC]\n\n");
}

int main (int argc, char **argv) 
{
    
    int i, j;
    BOOL bootdelay = FALSE;
    ULONG seconds, vblanks, bmsize;
    BYTE err;
    UBYTE *data, vblankfreq;

    struct ExecBase *SysBase;
    struct MsgPort *IOPort;
    struct IOStdReq *IOReq;
    struct CDTVLandSysPrefs *Prefs;
    struct RivieraExtBookmarkHeader *RivHeader;

    /* print banner */
    printf("\n===========================================================\n");
    printf ("/// CDTVTools ///\n");
    printf("===========================================================\n");
    printf (VSTRING);
    printf (" - Captain Future / CDTV Land\n");
    printf("-----------------------------------------------------------\n\n");

    /* detect vblank frequency */
    SysBase = (*((struct ExecBase **) 4));
    vblankfreq = SysBase->VBlankFrequency;

    /* convert args to upper case */
    for (i=1; i<argc; i++) {
        int j=0;
        while (argv[i][j]) {
            argv[i][j] = toupper(argv[i][j]);
            j++;
        }
    }

    /* parse args */
    for (i=1; i<argc; i++)
    {
        if (!strcmp(argv[i], "HDDBOOTDELAY")) {
            bootdelay = TRUE;
            i++;
            if (i >= argc) 
            {
                usage();
                printf ("ERROR: HDDBOOTDELAY requires a parameter\n");
                return (1);
            } 
            else 
            {
                /* is it a number ? */
                for (j=0; j<strlen(argv[i]); j++) {
                    if (!isdigit(argv[i][j])) {
                        usage();
                        printf ("ERROR: HDDBOOTDELAY should be a numeric value.\n");
                        return (1);
                    }
                }

                /* convert to int */
                seconds = atoi(argv[i]);
                printf ("\t. %lu seconds HDD Boot Delay requested.\n", seconds);

            }
        } else if (!strcmp(argv[i], "PAL")) {
            printf ("\t. 'PAL' system requested. Will use 50Hz calculation.\n");
            vblankfreq = 50;
        } else if (!strcmp(argv[i], "NTSC")) {
            printf ("\t. 'NTSC' system requested. Will use 60Hz calculation.\n");
            vblankfreq = 60;
        } else if (!strcmp(argv[i], "?")) {
            usage();
            return (0);
        } else if (!strcmp(argv[i], "HELP")) {
            usage();
            return (0);
        } else {
            usage();
            printf ("ERROR: Unknown argument %s\n", argv[i]);
            return (1);
        }
    }


    if (bootdelay) {
        /* calculate vblanks */
        vblanks = seconds * vblankfreq;
        printf ("\t. %d seconds is %d vblanks for %dHz.\n", seconds, vblanks, vblankfreq);
    }


    /* create IOReq */
    IOPort = CreatePort(0,0);
    IOReq = CreateStdIO(IOPort);

    /* open bookmark.device */
    printf ("\t. Opening Bookmark ID %04x.\n", (CDTVLAND_MFID<<16) + CDTVLAND_SYSPREFS_PRODID);
    err = OpenDevice("bookmark.device", (CDTVLAND_MFID<<16) + CDTVLAND_SYSPREFS_PRODID, IOReq, 0);

    if (!err) {
        /* check bookmark size */
        IOReq->io_Command = BD_SIZEOF;
        IOReq->io_Data = 0;
        IOReq->io_Offset = 0;
        IOReq->io_Length = 0;
        err = DoIO(IOReq);

        if (!err) {
            /* alloc mem for bookmark */
            bmsize = IOReq->io_Actual;
            data = (UBYTE*)AllocMem(bmsize, MEMF_PUBLIC);

            /* read bookmark */
            IOReq->io_Command = CMD_READ;
            IOReq->io_Data = data;
            IOReq->io_Offset = 0;
            IOReq->io_Length = bmsize;
            err = DoIO(IOReq);

            if (!err) {
                printf ("\t. Read %lu bytes from bookmark.\n", IOReq->io_Actual);

                /* check Riviera header */
                RivHeader = data;
                if (RivHeader->magic == 0x52495631) {
                    Prefs = (struct CDTVLandSysPrefs*) (data+RivHeader->size);
                    printf ("\t. CDTVLand SysPrefs Bookmark version is %d.\n", Prefs->version);

                    /* Update of HDD bootdelay is requested */
                    if (bootdelay) {
                        Prefs->hdd_Bootdelay = vblanks;

                        /* write bookmark */
                        IOReq->io_Command = CMD_WRITE;
                        IOReq->io_Data = data;
                        IOReq->io_Offset = 0;
                        IOReq->io_Length = bmsize;
                        err = DoIO(IOReq);

                        if (err) {
                            printf ("ERROR: Could not write to bookmark. Bookmark was NOT updated.\n");
                        } else {
                            /* read updated bookmark back */
                            IOReq->io_Command = CMD_READ;
                            IOReq->io_Data = data;
                            IOReq->io_Offset = 0;
                            IOReq->io_Length = bmsize;
                            err = DoIO(IOReq);
                        }

                    }

                    /* dump current bookmark contents */
                    printf ("\n\t>> HDD Boot Delay is set to %lu vblanks.\n", Prefs->hdd_Bootdelay);
                } else {
                    printf ("ERROR: Unrecognized bookmark header %08x.\n", RivHeader->magic);
                }

            } else {
                printf ("ERROR: Could not read bookmark.\n");
            }
        } else {
            printf ("ERROR: Could not check bookmark size. Error: %d\n", IOReq->io_Error);
        }
        /* close bookmark.device */
        CloseDevice(IOReq);

    } else {
        printf ("ERROR: Could not open bookmark.\n");
    }


    /* Delete IOreq */
    DeleteStdIO(IOReq);
    DeletePort(IOPort);

    return (0);
}