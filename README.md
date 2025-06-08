# CDTVLandTools

This repo contains the source code to the _CDTVLandTools_ application, which allows you to:

- Configure the HDD Bootdelay feature of CDTV OS 2.35
- Reset your bookmark memory (especially useful for A570 drives)

Currently _CDTVLandTools_ is available as a CLI tool only. Future revisions may include a Workbench UI as will the feature set expand.

Note: _CDTVLandTools_ requires a keyboard to operate.


## How to build or install CDTVLandTools

_CDTVLandTools_ comes as a standalone binary that you can install and run anywhere. It is also available as a bootable ADF. It is compatible with Kickstart 1.3 and higher.

Download the latest release, including source code, from the [releases](https://github.com/C4ptFuture/cdtv-cdtvtools/releases) page.

You can also build _CDTVLandTools_ yourself:

```sh
make cdtvlandtools
```
The build will be available in `build/cdtvlandtools`.

## How to use CDTVLandTools

From an AmigaDOS prompt type:

```
CDTVLandTools ?
```

to show the usage information.

Some examples:

```
    CDTVLandTools HDDBOOTDELAY           reads the current bootdelay value
    CDTVLandTools HDDBOOTDELAY 0         turn bootdelay feature off
    CDTVLandTools HDDBOOTDELAY 10        sets bootdelay to 10 seconds
    CDTVLandTools HDDBOOTDELAY 10 NTSC   sets bootdelay to 10 sec and forces 60Hz
    CDTVLandTools RESETBMSTORE           resets bookmark memory
```

### Forcing 50Hz or 60Hz timing
The HDD Bootdelay feature of CDTV OS 2.35 uses vertical blank timing, which means a different boot delay value is required between "PAL" (50Hz) and "NTSC" (60Hz) systems to achieve the same boot delay. _CDTVLandTools_ will automatically detect the frequency of the system and translate the number of seconds to the required number of vertical blanks. However if, for whatever reason, you want to override this detection mechanism you can force _CDTVLandTools_ to use 50Hz or 60Hz timing by specifying the `PAL` or `NTSC` options on the command line as in the example above.

### Resetting bookmark memory
Bookmark memory on the A570 is stored in EEPROM, which means it survives even if power is disconnected from your A570. Because bookmark memory is mapped directly into the Amiga's address space and because there is no memory protection, buggy programs can unintentionally overwrite bookmark memory and corrupt it. Depending on how bookmark memory is corrupted, various types of undesired behaviors may occur like CDTV titles not working or even the A500 not starting up.

On the CD1000 (CDTV player) bookmark corruption is not really a problem, because disconnecting the CD1000 from power will automatically clear and reset bookmark memory. On the A570 however, because it stores bookmarks in EEPROM memory, this can be a problem, because potential bookmark corruption survives power cycles, and so there is no simple end user action to resolve it. In that case you can use _CDTVLandTools_ to clear and re-initialize bookmark memory.

You can easily reset bookmark memory by using the `RESETBMSTORE` option of _CDTVLandTools_. This will clear the first word of bookmark memory ($DC8000). By subsequently resetting your Amiga, bookmark memory will be cleared and re-initialized by bookmark.device. See below how to reset bookmark memory if your A500 won't even boot when the A570 is connected.

#### CD1000 bookmark memory
Although the `RESETBMSTORE` option is primarily intended for use with the A570, there is no harm in running it on a CD1000 machine, although it's much easier to reset bookmark memory on the CDTV player by disconnecting the power cord, waiting for approximately one minute and then reconnecting it. (Note: just putting the CD1000 in standby mode is not enough to reset bookmark memory. The CD1000 continues to power bookmark memory SRAM even when in standby mode).

#### A500 won't start up with A570 connected
If your A500 won't even start up with the A570 connected and you want to rule out corrupted bookmark memory, use the following procedure:

- Make sure both A570 and A500 are off
- Turn the headphones volume dial on your A570 all the way to the left until it clicks. (This will disable the CDTV OS ROM inside the A570)
- Turn the A570 power supply on
- Turn the A500 power supply on
- Boot your A500 into Workbench or from the CDTVLandTools ADF image.
- From the CLI/Shell run  `CDTVLandTools RESETBMSTORE`
- Turn your A500 power supply off
- Turn your A570 power supply off
- Turn the headphones volume dial on your A570 to the right until it clicks. (This will re-enable the CDTV OS ROM inside the A570)
- Turn the A570 power supply on
- Turn the A500 power supply on

If your A500 displays the CDTV Title screen then corrupted bookmark memory was the cause of the problem. If your A500 still does not want to start up with the A570 enabled, the problem unfortunately lies elsewhere.

