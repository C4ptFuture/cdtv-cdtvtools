PROJECT=cdtvlandtools
VBCC=/opt/vbcc
VC=$(VBCC)/bin/vc
NDK=$(VBCC)/targets/m68k-amigaos/ndk_3.9
NDK_INC=$(NDK)/include/include_h
NDK_LIB=$(NDK)/linker_libs
SRC=src/$(PROJECT)
DATESTRING := $(shell date +'%-d.%-m.%y')
VERSION = $(shell grep VERSION VERSION|cut -d= -f2)
REVISION = $(shell grep REVISION VERSION|cut -d= -f2)

cdtvlandtools: createrev
	$(VC) +kick13 -I$(NDK_INC) -L$(NDK_LIB) -o build/$(PROJECT)/cdtvlandtools src/$(PROJECT)/cdtvlandtools.c -lamiga

# creates the version string include according to Commodore standard
createrev:
	echo "VSTRING	MACRO" > $(SRC)/rev.i
	echo "  dc.b   $(PROJECT) $(VERSION).$(REVISION) ($(DATESTRING))',13,10,0" >> $(SRC)/rev.i
	echo "  CNOP   0,2" >> $(SRC)/rev.i
	echo "  ENDM" >> $(SRC)/rev.i
	echo "#define VSTRING \"$(PROJECT) $(VERSION).$(REVISION) ($(DATESTRING))\"" > $(SRC)/rev.h
	