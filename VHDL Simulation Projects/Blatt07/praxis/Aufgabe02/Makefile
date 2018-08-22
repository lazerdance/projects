#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../../tools/ROrgPrSimLib
REMOTE_DIR := ~/.RorgPrRemote/

export PATH := ../../../tools/:$(PATH)

COMPILE  := aluCtrl.vhd

all: vcom/aluCtrl.vhd

define RULE
vcom/$(1): $(wildcard $(1))
endef

$(foreach FILE,$(COMPILE),$(eval $(call RULE,$(FILE))))

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
