#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../../tools/ROrgPrSimLib

export PATH := ../../../tools/:$(PATH)

all: mux_tb

mux_tb: vsim/mux_tb
	ln -fs vsim/mux_tb.vcd.gz waveform.vcd.gz
	ln -fs vsim/mux_tb        transcript

vcom/mux_tb.vhd:      $(wildcard mux_tb.vhd)
vcom/mux2.vhd:        $(wildcard mux2.vhd)

vsim/mux_tb: vcom/mux_tb.vhd

vcom/mux_tb.vhd: vcom/mux2.vhd

.PHONY:
view_wave:
	gtkwave -mtranscript waveform.vcd.gz

.PHONY:
clean:
	rm -f waveform.vcd.gz
	rm -f transcript

-include ../../../tools/modelsim.mk
