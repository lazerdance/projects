#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:/afs/tu-berlin.de/units/Fak_IV/aes/tools/rorg-pr/ROrgPrSimLib

export PATH := /afs/tu-berlin.de/units/Fak_IV/aes/tools/gtkwave/binaries/ubuntu$(shell grep RELEASE /etc/lsb-release | tr -c -d '[:digit:]' ):$(PATH)

-include /afs/tu-berlin.de/units/Fak_IV/aes/tools/gtkwave/modelsim.mk

COMPILE  := bin2Char.vhd bin2Char_tb.vhd
SIMULATE := bin2Char_tb

all: vsim/$(SIMULATE)
	ln -fs vsim/$(SIMULATE).vcd.gz waveform.vcd.gz
	ln -fs vsim/$(SIMULATE)        transcript

vsim/$(SIMULATE): | $(foreach FILE,$(COMPILE),vcom/$(FILE))

define RULE
vcom/$(1): $(wildcard $(1))
endef

$(foreach FILE,$(COMPILE),$(eval $(call RULE,$(FILE))))

.PHONY:
view_wave:
	gtkwave -mtranscript waveform.vcd.gz

.PHONY:
clean:
	rm -f waveform.vcd.gz
	rm -f transcript
