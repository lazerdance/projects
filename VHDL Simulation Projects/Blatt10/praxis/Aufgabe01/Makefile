#RUN := -all
#LIB := work
#VSIM := vsim -novopt
#VCOM := vcom
VMAP := RorgPrSimLib:../../../tools/ROrgPrSimLib
REMOTE_DIR := ~/.RorgPrRemote/

export PATH := ../../../tools/:$(PATH)

COMPILE  := bin2Char.vhd leftShifter.vhd signExtend.vhd addrDecoder.vhd reg.vhd regFile.vhd aluCtrl.vhd adder_1bit.vhd alu_1bit.vhd csAlu.vhd mipsCtrlFsm.vhd mipsCpu_mc.vhd mipsCpu_mc_tb.vhd
SIMULATE := mipsCpu_mc_tb

all: vsim/mipsCpu_mc_mem_tb vsim/mipsCpu_mc_alu_tb

vsim/mipsCpu_mc_mem_tb: | $(foreach FILE,$(COMPILE),vcom/$(FILE)) $(LIB) vsim
	@echo bash -c "$(VSIM) $(SIMULATE) -ginit_file_name="memcpy.mif" -c <<< \"log -r /$(SIMULATE)/*; run $(RUN); dataset save sim $@.wlf\" "
	@bash -c "$(VSIM) $(SIMULATE) -ginit_file_name="memcpy.mif" -c <<< \"log -r /$(SIMULATE)/*; run $(RUN); dataset save sim $@.wlf\" " | $(PRETTY_SIM_COLOR) 
	mv transcript $@
	wlf2log -lower $@.wlf | log2vcd | awk '/^#[0-9]+$$/{tmp=$$0} END{print "#"substr(tmp,2)+$(OVERTIME)} 1' | gzip > $@.vcd.gz
	ln -fs $@.vcd.gz waveform.vcd.gz
	ln -fs $@        transcript

vsim/mipsCpu_mc_alu_tb: | $(foreach FILE,$(COMPILE),vcom/$(FILE)) $(LIB) vsim
	@echo bash -c "$(VSIM) $(SIMULATE) -ginit_file_name="clip.mif" -c <<< \"log -r /$(SIMULATE)/*; run $(RUN); dataset save sim $@.wlf\" "
	@bash -c "$(VSIM) $(SIMULATE) -ginit_file_name="clip.mif" -c <<< \"log -r /$(SIMULATE)/*; run $(RUN); dataset save sim $@.wlf\" " | $(PRETTY_SIM_COLOR) 
	mv transcript $@
	wlf2log -lower $@.wlf | log2vcd | awk '/^#[0-9]+$$/{tmp=$$0} END{print "#"substr(tmp,2)+$(OVERTIME)} 1' | gzip > $@.vcd.gz
	ln -fs $@.vcd.gz waveform.vcd.gz
	ln -fs $@        transcript

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
