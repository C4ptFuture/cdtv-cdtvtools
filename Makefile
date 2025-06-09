PROJECT=cdtvlandtools

# assembler, compiler and linker paths
VBCC=/opt/vbcc
VC=$(VBCC)/bin/vc
VASM=$(VBCC)/bin/vasmm68k_mot
VLINK=$(VBCC)/bin/vlink

# devkits paths
SDK=/opt/sdk
NDK=$(SDK)/NDK_3.9/Include
NDK_INC_C=$(NDK)/include_h
NDK_INC_ASM=$(NDK)/include_i
NDK_LIB=$(NDK)/linker_libs
CDTV_SDK=$(SDK)/cdtv-sdk
CDTV_INC_C=$(CDTV_SDK)/include_h
CDTV_INC_ASM=$(CDTV_SDK)/include_i

# project settings
SRC=src/$(PROJECT)
BUILD=build/$(PROJECT)
DATESTRING := $(shell date +'%-d.%-m.%y')
VERSION = $(shell grep VERSION VERSION|cut -d= -f2)
REVISION = $(shell grep REVISION VERSION|cut -d= -f2)
AFLAGS=-no-opt -m68000 -kick1hunks -Fhunk
DEFINES=-DVERSION=$(VERSION) -DREVISION=$(REVISION) -D$(ENVIRONMENT) 


.PHONY: check-env clean builddir

$(PROJECT): deps
	$(VC) +kick13 -lamiga -I$(NDK_INC) -I$(CDTV_SDK)/include_h -o build/$(PROJECT)/$(PROJECT) src/$(PROJECT)/$(PROJECT).c

deps: check-env createrev builddir


# make sure all required env vars are set
check-env:
ifndef VERSION
	$(error VERSION is undefined)
endif
ifndef REVISION
	$(error REVISION is undefined)
endif
ifndef ENVIRONMENT
	$(error ENVIRONMENT is undefined)
endif


# makes sure build dir exists
builddir:
	@mkdir -p build/$(PROJECT)


# creates the version string include according to Commodore standard
createrev: 
	@echo "VSTRING	MACRO" > $(SRC)/rev.i
	@echo "  dc.b   $(PROJECT) $(VERSION).$(REVISION) ($(DATESTRING))',13,10,0" >> $(SRC)/rev.i
	@echo "  CNOP   0,2" >> $(SRC)/rev.i
	@echo "  ENDM" >> $(SRC)/rev.i
	@echo "#define VSTRING \"$(PROJECT) $(VERSION).$(REVISION) ($(DATESTRING))\\0\"" > $(SRC)/rev.h

# delete all build artifacts
clean:
	find $(BUILD) -name "*.o" -exec rm {} \;
	find $(BUILD) -name "*.ld" -exec rm {} \;
	find $(BUILD) -name "$(PROJECT)" -type f -exec rm {} \;


