# CDTVTools

This repo contains the source code to the CDTVTools application, which currently just does one thing which is to override the default HDD Boot Delay parameter for CDTV OS 2.35. Currently available as a CLI tool only, future revisions will include a Workbench UI as will the feature set expand.

## Instructions

From an AmigaDOS prompt type:

```
CDTVTools HDDBOOTDELAY <seconds>
```

where `<seconds>` is the number of seconds you want the HDD Boot Delay to be. Your setting will be stored in your CDTV player's bookmark memory, which remains active even if you turn off the CDTV player, as long as you don't disconnect it from mains power.