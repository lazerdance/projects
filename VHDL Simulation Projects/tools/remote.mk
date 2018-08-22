REPO_PATH := $(shell git rev-parse --show-toplevel)
REPO_CWD  := $(subst $(REPO_PATH),,$(PWD))
REPO_NAME := $(shell basename "$(REPO_PATH)")


UPLOAD   := '*.mif' '*.mk' '*.vhd' '*.vhdl' 'tools/ROrgPrSimLib/***' 'log2vcd' 'Makefile'
DOWNLOAD := 'vsim/***' 'vcom/***' 'transcript' 'waveform.vcd.gz'

ifndef REMOTE_DIR
REMOTE_DIR := ~/.RorgPrRemote/
endif

ifndef HOST
HOST := ubu16.eecsit.tu-berlin.de
endif

SSH_CTRL := ~/.sshctrl

ifndef SSH_TIMEOUT
SSH_TIMEOUT := 60m
endif

SSH := ssh -o 'ControlPath "$(SSH_CTRL)"' '$(HOST)'

remote: upload remote_clean remote_all download

remote_all: $(SSH_CTRL)
	@$(SSH) mkdir -p '$(REMOTE_DIR)'
	@$(SSH) make -C '$(REMOTE_DIR)/$(REPO_NAME)/$(REPO_CWD)' all

upload: $(SSH_CTRL)
	@$(SSH) mkdir -p '$(REMOTE_DIR)'
	@rsync -azvh\
	      $(foreach INCLUDE,$(UPLOAD),--include=$(INCLUDE)) \
	      --include='*/'\
	      --exclude='*'\
	      -e "ssh -o 'ControlPath $(SSH_CTRL)'"\
	      --prune-empty-dirs\
	      '$(REPO_PATH)'\
	      '$(USER)@$(HOST):$(REMOTE_DIR)'

download: $(SSH_CTRL)
	@$(SSH) mkdir -p '$(REMOTE_DIR)'
	@rsync -azvh\
	      $(foreach INCLUDE,$(DOWNLOAD),--include=$(INCLUDE)) \
	      --include='*/'\
	      --exclude='*'\
	      -e "ssh -o 'ControlPath $(SSH_CTRL)'"\
	      --prune-empty-dirs\
	      '$(USER)@$(HOST):$(REMOTE_DIR)/$(REPO_NAME)/'\
	      '$(REPO_PATH)/'

login: $(SSH_CTRL)

.PHONY:
logout:
	-ssh -S $(SSH_CTRL) -O exit localhost
	-rm $(SSH_CTRL)

$(SSH_CTRL):
	@echo "Logging in on $(HOST)"
	@while [ -z $$SSHUSER ]; do echo -n "Username: "; read SSHUSER; done;\
	ssh -o 'ControlPath "$(SSH_CTRL)"' -o 'ControlPersist $(SSH_TIMEOUT)' -MNf "$$SSHUSER@$(HOST)"

.PHONY:
remote_clean:
	@$(SSH) mkdir -p '$(REMOTE_DIR)'
	@$(SSH) make -C '$(REMOTE_DIR)/$(REPO_NAME)/$(REPO_CWD)' clean

ssh: $(SSH_CTRL)
	$(SSH)
