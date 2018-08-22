# Rechnerorganisation Praktikum WS17

## GIT
* GIT ist eine Revisonierungssoftware wie zb SVN
* GIT ist dezentral (es gibt keinen Server, der einem Dinge vorschreibt)
* ein GIT Repository ist eine Sammlung von **commits**
* ein **commit** ist eine Zusammenfassung von Aenderungen

### Workflow

* etwas 'hochladen'
 1. Datei **X** hat Aenderungen oder ist neu
 2. *git add **X**
   * fuegt die Datei zum naechsten **commit** hinzu
 3. *git status*
   * Ueberpruefen ob die auch wirklich nur die gewuenschten Dateien in den **commit** kommen
 4. *git diff --staged*
   * Optionale Ueberpruefung was fuer Aenderungen vorgenommen werden
 5. *git commit -m "**X wurde aktualisiert**"*
   * erzeugt den **commit** mit dem Kommentar "**X wurde aktualisiert**"
   * dieser **commit** ist jetzt im lokalen GIT hinzugefuegt
 6. *git push*
   * sendet die neuen **commits** zum Server


* lokales Repo aktualisieren
 1. *git status*
   * Ueberpruefen ob man selbst ein *sauberes* Repo hat (keine ungesicherten Aenderungen)
 2. *git pull*
   * Holt sich neue **commits** vom Server
 3. Es kann nun zu einem **merge conflict** kommen
   * Die neuen Aenderungen lassen sich nicht auf die lokalen Daten anwenden
   * Es muss nun ein *merge commit* gemacht werden
      1. GIT hat automatische alle unbetroffenen Dateien zu diesem commit hinzugefuegt
      2. Mit *git status* ueberpruefen, welche Dateien betroffen sind
      3. Die Dateien oeffnen und schauen welche Aenderungen nicht umsetztbar sind
      4. Sich fuer eine Variante entscheiden (im Notfall fragen)
      5. Mit *git add* die betroffenen Dateien zum *merge commit* hinzufuegen
      6. *git commit*

### wichtige GIT Befehle
| Befehl                         | Beschreibung
| :----------------------------- | :-------------
| git clone *repo-uri*           | Kopiert (klont) ein vorhandenes Repository auf den lokalen Rechner
| git status                     | Zeigt den Status aller Änderungen an und welche zum nächsten commit gehören
| git add *datei*                | Fügt Änderungen (und neue Dateien) zu dem nächsten commit hinzu
| git reset *datei*              | Entfernt Dateien vom naechsten commit, welche mit add hinzugefuegt worden sind
| git diff                       | Listet alle Aenderungen, welche in keinem commit sind
| git diff --staged              | Listet alle Aenderungen, welche mit *git add* hinzugefuegt worden sind
| git rm *datei*                 | Entfernt die Datei aus git und löscht sie auch lokal.
| git commit -m "*Beschreibung*“ | Fasst alle vorgemerkten Änderungen (git add) zu einem commit zusammen mit der angegebenen Beschreibung.
| git pull                       | Holt sich neue commits vom Mutter-Repository.
| git push                       | Lädt lokale commits zum Mutter-Repository hoch.
| git checkout *datei*           | Setzt alle Änderungen der Datei auf den Stand des letzten commits zurück.
| git pull vorgaben master       | Aktualisiert das repo mithilfe des vorgaben repos
