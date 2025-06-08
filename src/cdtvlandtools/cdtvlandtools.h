#include "exec/types.h"


struct CDTVLandSysPrefs {
    BYTE  version;
    BYTE  reserved1;
    ULONG hdd_Bootdelay;
    WORD  reserved2;
};

struct RivieraExtBookmarkHeader {
    ULONG   magic;          /* "RIV1" */
    UWORD   size;           /* size of the extended bookmark metadata, excluding this header */
    UBYTE   first_Chunk;    /* the first metadata chunk */
};

