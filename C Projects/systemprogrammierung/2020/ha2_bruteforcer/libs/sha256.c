#include "sha256.h"

/**
 * Berechnet den SHA256 Hash des angegebenen Strings
 * Gibt einen String zurück, für dynamisch Speicher reserviert wird.
 * Dieser muss später gefreet werden.
 */
char *sha256(char *string) {
	if (string == NULL) {
		string = "";
	}
	
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	
	sha256_init(&ctx);
	sha256_update(&ctx, (BYTE *) string, strlen(string));
	sha256_final(&ctx, buf);
	
	char* hash = malloc(65 * sizeof(char));
	for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
		sprintf(hash + (i * 2), "%02x", buf[i]);
	}
	hash[64] = '\0';
	
	return hash;
}
