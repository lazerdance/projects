
REPO=git@gitlab.tubit.tu-berlin.de:svc-rorg-pr/RorgPrWS17.git
SEMESTER=WS17

all:
	@echo Willkommen im RechnerOrganisation Praktikum $(SEMESTER)!
	@echo Dieses Script konfiguriert euer Git Repo
	@echo Folgende Optionen bestehen:
	@echo
	@make -s help

help:
	@echo "make setup    | Automatische Konfiguration des Repos (macht alles folgende von alleine)"
	@echo "make editor   | Setzt den Standard Editor"
	@echo "make user     | Setzt euren Git Nutzernamen"
	@echo "make email    | Setzt eure Git Emailadresse"
	@echo "make remote   | Referenzieren des Vorgabe Repos"
	@echo "make help     | Zeigt diese Hilfe an"

setup:
	@echo "Referenziere Vorgabe Repo..."
	@make -s remote
	@echo
	@echo "Setze sonstige Git Configs..."
	@make -s misc
	@echo
	@echo "Welcher Editor soll genommen werden? (Enter fuer default)"
	@make -s editor
	@echo
	@echo "Wie soll dein Git Nutzername lauten? (Enter fuer default)"
	@make -s user
	@echo
	@echo "Welche Email soll Git nehmen? (Enter fuer default)"
	@make -s email

editor:
	@echo -n "Editor (default: nano): "
	@read VAR; git config core.editor "$${VAR:-nano}"

email:
	@echo -n "Git Email: (default: $(USER)@mailbox.tu-berlin.de): "
	@read VAR; git config user.email "$${VAR:-$(USER)@mailbox.tu-berlin.de}"

user:
	@echo -n "Git User: (default: $${USERNAME:-$$USER}): "
	@read VAR; git config user.name "$${VAR:-$${USERNAME:-$$USER}}"

misc:
	git config push.default simple
	git config format.pretty 'format:%C(yellow)%h %Creset%ai %<(15) %C(green)%aN %Creset%s'
	git config color.ui true

remote:
ifeq ($(shell git remote | grep -c vorgabe ),0)
	git remote add vorgaben $(REPO)
endif

