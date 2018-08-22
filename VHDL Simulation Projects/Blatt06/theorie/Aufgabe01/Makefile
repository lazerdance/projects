#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../../tools/ROrgPrSimLib
REMOTE_DIR := ~/.RorgPrRemote/

export PATH := ../../../tools/:$(PATH)

COMPILE  := fulladd.vhd adder.vhd

all: | $(foreach FILE,$(COMPILE),vcom/$(FILE))
	@echo
	@echo ALLES ERFOLGREICH KOMPILIERT
	@echo

vcom/%.vhd: $(wildcard %.vhd)

.PHONY:
#view:
#wave:
#view_wave:
#	gtkwave -mtranscript waveform.vcd.gz

#.PHONY:
clean:
#	rm -f waveform.vcd.gz
#	rm -f transcript

-include ../../../tools/modelsim.mk
-include ../../../tools/remote.mk
