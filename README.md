# CDTVTools

This repo contains the source code to the CDTVTools application, which currently just does one thing which is to override the default HDD Boot Delay parameter for CDTV OS 2.35. Currently available as a CLI tool only, future revisions will include a Workbench UI as will the feature set expand.


## How to build or install CDTVTools

CDTVTools comes as a standalone binary that you can install and run anywhere. It is compatible with Kickstart 1.3 and higher. Download the latest release, including source code, from the releases page.

You can also build CDTVTools yourself:

```sh
make cdtvtools
```
The build will be available in `build/cdtvools`.

## How to use CDTVTools

From an AmigaDOS prompt type:

```
CDTVTools HDDBOOTDELAY <seconds>
```

where `<seconds>` is the number of seconds you want the HDD Boot Delay to be. Your setting will be stored in your CDTV player's bookmark memory, which remains active even if you turn off the CDTV player, as long as you don't disconnect it from mains power.