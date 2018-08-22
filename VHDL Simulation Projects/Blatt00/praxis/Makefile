#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../tools/ROrgPrSimLib

export PATH := ../../tools/:$(PATH)

all: or2_tb

or2_tb: vsim/or2_tb
	ln -fs vsim/or2_tb.vcd.gz waveform.vcd.gz
	ln -fs vsim/or2_tb        transcript

vcom/or2_tb.vhd:      $(wildcard or2_tb.vhd)
vcom/or2.vhd:         $(wildcard or2.vhd)

vsim/or2_tb: vcom/or2_tb.vhd

vcom/or2_tb.vhd: vcom/or2.vhd

.PHONY:
view_wave:
	gtkwave -mtranscript waveform.vcd.gz

.PHONY:
clean:
	rm -f waveform.vcd.gz
	rm -f transcript

-include ../../tools/modelsim.mk

