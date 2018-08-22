#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../../tools/ROrgPrSimLib

export PATH := ../../../tools/:$(PATH)

all: logic_tb

logic_tb: vsim/logic_tb
	ln -fs vsim/logic_tb.vcd.gz waveform.vcd.gz
	ln -fs vsim/logic_tb        transcript

vcom/logic_tb.vhd:      $(wildcard logic_tb.vhd)
vcom/and2.vhd:          $(wildcard and2.vhd)
vcom/xnor2.vhd:         $(wildcard xnor2.vhd)
vcom/xor2.vhd:          $(wildcard xor2.vhd)

vsim/logic_tb: vcom/logic_tb.vhd

vcom/logic_tb.vhd: vcom/and2.vhd vcom/xnor2.vhd vcom/xor2.vhd

.PHONY:
view_wave:
	gtkwave -mtranscript waveform.vcd.gz

.PHONY:
clean:
	rm -f waveform.vcd.gz
	rm -f transcript

-include ../../../tools/modelsim.mk
