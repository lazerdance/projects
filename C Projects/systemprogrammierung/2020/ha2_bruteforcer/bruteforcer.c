#include "bruteforcer.h"

void add_child_to_worker_list(pid_t child){
	for (int i=0; i<max_workers; i++) {
		if (worker[i] != 0) continue;
		worker[i] = child;
		break;
	}
}

int main(int argc, char *argv[]) {
	// Argumente checken
	if (argc != 4) {
		fprintf(stderr, "usage: %s pwdmaxlen workers hashfile\n", argv[0]);
		exit(1);
	}
	
	char *filename = NULL;
	hashes *loaded_hashes = NULL;
	
	// Kommandozeilenargumente auslesen und globale und lokale Variablen füllen
	// TODO: Vervollständigen
	pwd_maxlen = atoi(argv[1]);
	max_workers = atoi(argv[2]);
	filename = argv[3];
	
	worker = malloc(max_workers*sizeof(pid_t));
	// worker array mit 0 initialisieren
	for(int i=0; i<max_workers; i++){
		worker[i] = 0;
	}
	
	INFO("\nBRUTEFORCER GESTARTET\n");
	INFO("---------------------\n");
	INFO("Maximale Passwortlänge: %d\n", pwd_maxlen);
	INFO("Anzahl der Worker: %d\n", max_workers);
	INFO("Hashes lesen aus Datei: %s\n", filename);
	INFO("---------------------\n\n");
	
	// Hashes in ein hashes struct laden
	// TODO
	loaded_hashes = load_hashes(filename);
	// pwd *pass;
	// Main loop -> Iteriert über alle Hashes
	// int children = 0;
	pid_t child;
	pwd * pass;
	// Main loop -> Iteriert über alle Hashes
	for (int i = 0; i < loaded_hashes->len; i++) {
		// Hash mit crack_hash versuchen zu knacken
		if( (child = fork()) == 0){
            pass = crack_hash(loaded_hashes->array[i]);
			// Erfolg? -> print password
            if (pass != NULL) {
                printf("%s\n", pass->buf);
                free_password(pass);
            }
            free_hashes(loaded_hashes);
            free(worker);
            return 0;
		} else {
			add_child_to_worker_list(child);
			if (update_worker() == max_workers)
                waitpid(child, NULL, 0);
        }
	}
	// Fehlgeschlagen? -> Einfach weiter in der Schleife
	while (update_worker() != 0)
		waitpid(-1, NULL, 0);
	// Aufräumen und beenden
	free_hashes(loaded_hashes);
	free(worker);
	
	return 0;
}

// Versucht den Hash zu cracken, indem systematisch Passwörter generiert werden
// und darauf getestet wird, ob deren Hash mit hash übereinstimmt
// Returns pwd or NULL if no password was found
pwd *crack_hash(char *hash) {
	// Mit new_password() ein leeres Passwort anlegen
	pwd *password = new_password(pwd_maxlen);

	// Mit test_string() überprüfen, ob das (zuerst leere) Passwort zum Hash passt
	// In einer Schleife next_password() aufrufen, und das nächste Passwort überprüfen
	// Schleifenabbruch, sobald next_password() 0 zurückgibt => es gibt kein weiteres Passwort,
	// d.h. alle Passwörter bis zur maximalen Länge wurden bereits generiert und überprüft
	// ODER
	// Schleifenabbruch, wenn das Passwort gefunden wurde
	// TODO
	int found_pass = 0;
	while(1){
		if(test_string(password->buf,hash) == 1){
			found_pass = 1;
			break;
		}
		if(next_password(password) == 0) break;
	}
	// Aufräumen
	// TODO
	
	// Passwort nicht gefunden -> NULL zurückgeben
	// Passwort gefunden -> das Password zurückgeben
	// TODO
	if(found_pass == 1){
		return password;
	}

	free_password(password);
	return NULL;
}

// Berechnet den Hash von string und gibt 1 zurück, wenn er mit hash übereinstimmt, sonst 0
int test_string(char *string, char *hash) {
	char * hashed_string = sha256(string);
	if(strcmp(hashed_string, hash) == 0){
		free(hashed_string);
		return 1;
	}
	free(hashed_string);
	return 0;
}

/**
 * Diese Funktion soll alle worker Kindprozesse die in dem Array "worker"
 * gespeichert sind darauf überprüfen, ob diese noch laufen oder bereits
 * beendet sind. Wenn ein Prozess beendet wurde soll der Eintrag in dem Array
 * gelöscht werden (auf 0 setzen), so dass dieser wieder zur Verfügung steht.
 *
 * return value:
 *   Diese Funktion soll die Anzahl der momentan im Hintergrund laufenden
 *   Prozesse zurückgeben. Prozesse die beendet wurden zählen nicht dazu.
 */
int update_worker() {
	int n = 0;
	for (int i = 0; i < max_workers; i++) {
		if(worker[i] != 0){
			if (waitpid(worker[i], NULL, WNOHANG) == 0) n++;
			else worker[i] = 0;
		}
	}
	
	return n;
}
