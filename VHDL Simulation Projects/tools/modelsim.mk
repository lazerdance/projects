export LM_LICENSE_FILE := 1717@licaes.msc.tu-berlin.de:$(LM_LICENSE_FILE)
export PATH := $(PATH):/afs/tu-berlin.de/units/Fak_IV/aes/tools/mentor/modeltech/linux_x86_64

PRETTY_SIM_COLOR := awk --field-separator=";" '/(Failure|Fatal|Error)/{print "\033[31m\# "$$2"+"$$3": "substr($$1, 2)": "$$5" | "$$4"\033[0m"; fflush(); next} /(Warning)/{print "\033[33m\# "$$2"+"$$3": "substr($$1, 2)": "$$5" | "$$4"\033[0m"; fflush(); next} /Note/{print "\# "$$2"+"$$3": "substr($$1, 2)": "$$5" | "$$4; fflush(); next} 1'
PRETTY_COMP_COLOR := awk '/Error/{print "\033[31m"$$0;next} /Warning/{print "\033[33m"$$0;next} {print $0} END{printf "\033[0m"}'

ifndef LIB
LIB := work
endif

ifndef RUN
RUN := -all
endif

ifndef VCOM
VCOM := vcom
endif

ifndef VSIM
VSIM := vsim -novopt
endif

ifndef VMAP
VMAP  := 
endif

ifndef OVERTIME
OVERTIME := 100000
endif

define MODELSIM_INI
[Library]
others = $$MODEL_TECH/../modelsim.ini
work = work
[vcom]
VHDL93 = 2002
Explicit = 1
Quiet = 1
[vlog]
LibrarySearchPath = mtiAvm mtiOvm mtiUPF
[sccom]
[vopt]
[vsim]
VoptFlow = 0
Resolution = ps
UserTimeUnit = default
RunLength = 100
IterationLimit = 5000
BreakOnAssertion = 3
MessageFormatBreakLine = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormatBreak     = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormat          = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormatNote      = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormatWarning   = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormatError     = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormatFail      = %S;%T;%D;%F:%L:%i;%R;\\n
MessageFormatFatal     = %S;%T;%D;%F:%L:%i;%R;\\n
ShowFunctions = 1
DefaultRadix = symbolic
TranscriptFile = transcript
PathSeparator = /
DatasetSeparator = :
UnbufferedOutput = 0
ConcurrentFileLimit = 40
ScTimeUnit = ns
ScMainStackSize = 10 Mb
ScMainFinishOnQuit = 1
ScvPhaseRelationName = mti_phase
OnFinish = ask
DumpportsCollapse = 1
[lmc]
libsm = $$MODEL_TECH/libsm.sl
libhm = $$MODEL_TECH/libhm.sl
[msg_system]
endef

help:
	@echo "################# usage ######################"
	@echo "make vcom/<file>   Compiles <file>."
	@echo "make vsim/<entity> Simulates <entity>."
	@echo "make clean         Removes everything generated."
	@echo "make clean_lib     Removes libs."
	@echo "make clean_comp    Removes compiler outputs."
	@echo "make clean_sim     Removes simulation outputs."
	@echo "make options       Prints all setable options."
	@echo "##############################################"

options:
	@echo VERBOSE: $(VERBOSE)
	@echo LIB: $(LIB)
	@echo RUN: $(RUN)
	@echo VCOM: $(VCOM)
	@echo VSIM: $(VSIM)

vsim:
	mkdir vsim

vsim/%: | $(LIB) vsim
	@echo bash -c "$(VSIM) $(notdir $@) -c <<< \"log -r /$(notdir $@)/*; run $(RUN); dataset save sim $@.wlf\" "
	@bash -c "$(VSIM) $(notdir $@) -c <<< \"log -r /$(notdir $@)/*; run $(RUN); dataset save sim $@.wlf\" " | $(PRETTY_SIM_COLOR) 
	-mv transcript $@
	wlf2log -lower $@.wlf | log2vcd | awk '/^#[0-9]+$$/{tmp=$$0} END{print "#"substr(tmp,2)+$(OVERTIME)} 1' | gzip > $@.vcd.gz

vcom:
	mkdir vcom

.DELETE_ON_ERROR: vcom/%
vcom/%: % | $(LIB) vcom
	 @echo "$(VCOM) $<";\
	 $(VCOM) $< 2>&1 > $@;\
	 error=$$?;\
	 $(PRETTY_COMP_COLOR) $@;\
	 exit $$error;

$(LIB): modelsim.ini
	vlib $(LIB)
	vmap work $(LIB)
	$(foreach map,$(VMAP),vmap $(subst :, ,$(map)))

export MODELSIM_INI
modelsim.ini:
	@echo "Creating modelsim.ini"
	@echo "$$MODELSIM_INI" > $@

.PHONY: clean
clean: clean_lib clean_vcom clean_vsim

.PHONY: clean_lib
clean_lib:
	rm -f modelsim.ini
	rm -rf $(LIB)

.PHONY: clean_vcom
clean_vcom:
	rm -rf vcom/

.PHONY: clean_vsim
clean_vsim:
	rm -rf vsim/

