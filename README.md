# CDTVTools

This repo contains the source code to the _CDTVTools_ application, which allows you to:

- Configure the HDD Bootdelay feature of CDTV OS 2.35
- Reset your bookmark memory (especially useful for A570 drives)

Currently _CDTVTools_ is available as a CLI tool only. Future revisions will include a Workbench UI as will the feature set expand.


## How to build or install CDTVTools

_CDTVTools_ comes as a standalone binary that you can install and run anywhere. It is compatible with Kickstart 1.3 and higher. Download the latest release, including source code, from the releases page.

You can also build _CDTVTools_ yourself:

```sh
make cdtvtools
```
The build will be available in `build/cdtvools`.

## How to use CDTVTools

From an AmigaDOS prompt type:

```
CDTVTools ?
```

to show the usage information.

Some examples:

```
    CDTVTools HDDBOOTDELAY           reads the current bootdelay value
    CDTVTools HDDBOOTDELAY 0         turn bootdelay feature off
    CDTVTools HDDBOOTDELAY 10        sets bootdelay to 10 seconds
    CDTVTools HDDBOOTDELAY 10 NTSC   sets bootdelay to 10 sec and forces 60Hz
    CDTVTools RESETBMSTORE           resets bookmark memory
```

### Forcing 50Hz or 60Hz timing
The HDD Bootdelay feature uses vertical blank timing, which means a different boot delay value is required between "PAL" (50Hz) and "NTSC" (60Hz) systems. _CDTVTools_ will automatically detect the frequency of the system and translate the number of seconds to the required number of vertical blanks. However if, for whatever reason, you want to override this detection mechanism you can force _CDTVTools_ to use 50Hz or 60Hz timing by specifying the `PAL` or `NTSC` options on the command line as in the example above.

### Resetting bookmark memory
Bookmark memory on the A570 is stored in EEPROM, which means it survives even if power is disconnected from your A570. Because bookmark memory is mapped directly into the Amiga's address space and because there is no memory protection, buggy programs can unintentionally overwrite bookmark memory and corrupt it. On the CD1000 (CDTV player) this is not really a problem, because disconnecting it from power will automatically clear bookmark memory. On the A570, because it has EEPROM memory, this can be a problem, because there is no simple end user action to resolve this. Depending on how bookmark memory is corrupted, various types of undesired behaviors may occur like CDTV titles not working or even the A500 not starting up.

You can easily reset bookmark memory by using the `RESETBMSTORE` option of _CDTVTools_. This will clear the first word of bookmark memory ($DC8000). By subsequently resetting your Amiga, bookmark memory will be cleared and re-initialized by bookmark.device.

#### CD1000 bookmark memory
Although the `RESETBMSTORE` option is primarily intended for use with the A570, there is no harm in running it on a CD1000 machine, although it's much easier to reset bookmark memory on the CDTV player by disconnecting the power cord, waiting for approximately one minute and then reconnecting it. (Note: just putting the CD1000 in standby mode is not enough to reset bookmark memory. The CD1000 continues to power bookmark memory SRAM even when in standby mode).

#### A500 won't start up with A570 connected
If your A500 won't even start up with the A570 connected and you want to rule out corrupted bookmark memory, use the following procedure:

- Make sure both A570 and A500 are off
- Turn the headphones volume dial on your A570 all the way to the left until it clicks. (This will disable the CDTV OS ROM inside the A570)
- Turn the A570 power supply on
- Turn the A500 power supply on
- Boot your A500 into Workbench
- From the CLI/Shell run  `CDTVTools RESETBMSTORE`
- Turn your A500 power supply off
- Turn your A570 power supply off
- Turn the headphones volume dial on your A570 to the right until it clicks. (This will re-enable the CDTV OS ROM inside the A570)
- Turn the A570 power supply on
- Turn the A500 power supply on

If your A500 displays the CDTV Title screen then corrupted bookmark memory was the cause of the problem. If your A500 still does not want to start up with the A570 enabled, the problem unfortunately lies elsewhere.

