#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../../tools/ROrgPrSimLib
REMOTE_DIR := ~/.RorgPrRemote/

export PATH := ../../../tools/:$(PATH)

COMPILE  := leftShifter.vhd leftShifter_tb.vhd
SIMULATE := leftShifter_tb

all: vsim/$(SIMULATE)
	ln -fs vsim/$(SIMULATE).vcd.gz waveform.vcd.gz
	ln -fs vsim/$(SIMULATE)        transcript

vsim/$(SIMULATE): | $(foreach FILE,$(COMPILE),vcom/$(FILE))
vcom/%.vhd: $(wildcard %.vhd)

.PHONY:
view:
wave:
view_wave:
	gtkwave -mtranscript waveform.vcd.gz

.PHONY:
clean:
	rm -f waveform.vcd.gz
	rm -f transcript

-include ../../../tools/modelsim.mk
-include ../../../tools/remote.mk
