Diese Datei erklaert die Nutzung des GUI-Modus der Simulation. Dieser
funktioniert nicht auf den Uniservern via SSH und ist komplett
optional!
---
1. Installation der Libraries
2. Anpassen der Vorgabe
    2.1. Anpassen des Makefiles
    2.2. Anpassen von Params.h
    2.3. Vollstaendig neu kompilieren
3. Weitere Hinweise
---
1. Installation der Libraries
Die benoetigten Libraries sind `SDL2` und `SDL2 image`. Es gibt mehrere
Moeglichkeiten zur Installation dieser Bibliotheken. Versuchen Sie zuerst
die Libraries mit ihrem Packetmanager zu installieren. Unter Ubuntu lautet der
Befehl `$ apt-get install libsdl2-image-dev`, unter Fedora `$ dnf install SDL2-devel`.
Falls Sie Probleme bei der Installation mittels Packetmanager haben, koennen Sie
die Bibliothek auch mit dem Befehl `make install` installieren. Dabei werden die
Bibliotheken in Ihr `~/SDL` installiert. Dies aendert auch ihre bashrc. Um die
die Aenderung der ~/bashrc wirksam zu machen, geben Sie folgenden Befehl ein:
`source ~/.bashrc`

2. Anpassen der Vorgabe
Um den GUI-Modus in der Anwendung zu aktivieren, muessen Sie drei Schritte
befolgen:

    2.1. Anpassen des Makefiles
Im Makefile muessen die Zeilen 13-15  von
```
#	${SRC_DIR}/GUI.c
# LFLAGS += `$(SDL_CONFIG_BIN) --libs` -lSDL2_image 
# CFLAGS += `$(SDL_CONFIG_BIN) --cflags` 
```
zu 
```
	${SRC_DIR}/GUI.c
LFLAGS += `$(SDL_CONFIG_BIN) --libs` -lSDL2_image 
CFLAGS += `$(SDL_CONFIG_BIN) --cflags` 
```
geaendert werden.

    2.2. Anpassen von Params.h
In der Datei include/Params.h muss Zeile 11 von
```//#define GUI_MODE```
zu
```#define GUI_MODE```
geandert werden.

    2.3 Vollstaendig neu kompilieren
Kompilieren Sie das Projekt vollstaendig neu mithilfe des  Befehl:
```make clean all```

3. Weiter Hinweise
Falls Sie Probleme mit der Installation von SDL haben, schauen Sie auf der
offiziellen SDL Seite nach: https://wiki.libsdl.org/Installation
Es lohnt sich auch, die Linux-spezifische FAQ Seite anzuschauen:
https://wiki.libsdl.org/FAQLinux
Um wieder in den CSV-Modus zu wechseln, muessen Sie nur den Schritt 2.2
rueckgaengi machen und danach Schritt 2.3 wieder ausfuehren.
