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
ENVIRONMENT=release make cdtvlandtools
```
The build will be available in `build/cdtvlandtools`.


## How to use CDTVLandTools

To show the usage information, from an AmigaDOS prompt, type:

```
CDTVLandTools ?
```

Some examples:

```
    CDTVLandTools HDDBOOTDELAY           reads the current bootdelay value
    CDTVLandTools HDDBOOTDELAY 0         turn bootdelay feature off
    CDTVLandTools HDDBOOTDELAY 10        sets bootdelay to 10 seconds
    CDTVLandTools HDDBOOTDELAY 10 NTSC   sets bootdelay to 10 sec and forces 60Hz
    CDTVLandTools RESETBMSTORE           resets bookmark memory
```


## Resetting bookmark memory
On an A570 the CDTV bookmark memory lives in on-board EEPROM, so corruption survives power-off. Because this area is unprotected and memory-mapped, a rogue program can overwrite it, leading to symptoms such as:

- CDTV titles hanging during startup
- Flickering CDTV title screen
- A500 refusing to boot (hangs on black screen or Guru crashes)

Reset bookmark memory with:

```
CDTVLandTools RESETBMSTORE
```

`RESETBMSTORE` zeroes the first word at $DC8000, which is where bookmark memory is mapped to. After it completes, reboot the Amiga, and `bookmark.device` will detect the blank area and automatically re-initialise bookmark memory for you.

_Note: If your A500 won't even start with the A570 connected, you're not able to run any programs. In that case follow the procedure outlined below._

### A500 won't start up with A570 connected
If your A500 won't even start up with the A570 connected and you want to rule out corrupted bookmark memory, use the following procedure:

- Make sure both A570 and A500 are off
- Turn the headphones volume dial on your A570 all the way to the left until it clicks. (This will disable the CDTV OS ROM inside the A570)
- Turn the A570 power supply on
- Turn the A500 power supply on
- Boot your A500 into Workbench or from the _CDTVLandTools_ ADF image.
- From the CLI/Shell run  `CDTVLandTools RESETBMSTORE`
- Turn your A500 power supply off
- Turn your A570 power supply off
- Turn the headphones volume dial on your A570 to the right until it clicks. (This will re-enable the CDTV OS ROM inside the A570)
- Turn the A570 power supply on
- Turn the A500 power supply on

If your A500 displays the CDTV title screen (the rotating CDTV logos) then corrupted bookmark memory was the cause of the problem and it has now been solved. If your A500 still does not want to start up with the A570 enabled, the problem unfortunately lies elsewhere.

### What about the CD1000?

Running `RESETBMSTORE` on a CD1000 is perfectly safe, but usually unnecessary. Simply unplug the player for approximately 60 seconds (standby is not enough, you need to unplug the machine). Its SRAM bookmark memory clears when totally unpowered.


## 50Hz or 60Hz timing for HDD bootdelay
CDTV OS 2.35’s HDD Bootdelay feature counts vertical blanks (v-blanks), not seconds. Because PAL generates 50 v-blanks and NTSC 60 v-blanks, the same numerical value produces different real-time delays. _CDTVLandTools_ auto-detects the video standard of your system, converts the delay you enter in seconds to the correct v-blank count, and writes it for you to bookmark memory. Need to override that detection for some specific reason? Add `PAL` or `NTSC` to the command line:

```
CDTVLandTools HDDBOOTDELAY 5 PAL    # 5-second delay, force PAL timing
CDTVLandTools HDDBOOTDELAY 3 NTSC   # 3-second delay, force NTSC timing
```
