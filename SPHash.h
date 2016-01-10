#ifndef SPHASH_H__
#define SPHASH_H__

#include <stdbool.h>

typedef struct hash_t* SPHash;

typedef enum HashResult_t {
	SP_HASH_OK,
	SP_HASH_OUT_OF_MEMORY,
	SP_HASH_NULL_ARGUMENT
} HashResult;


/**
 * Allocates a new Hashtable.
 *
 * This function creates a new empty hashtable.
 * @return
 * 	NULL - If allocations failed.
 * 	A new SPHash in case of success.
 */
SPHash hashCreate();

/**
 * hashDestroy: Deallocates an existing hashtable. Clears all elements by using
 * the stored free function.
 *
 * @param hash Target SPHash to be deallocated. If hash is NULL nothing will be
 * @param result result of the operation, if NULL then not written to.
 * done
 */
void hashDestroy(SPHash hash);

/**
 * 
 */
double *hashGetValue(SPHash hash, char *key, HashResult *error);

bool hashInsert(SPHash hash, char *key, double value, HashResult *error);

bool hashDelete(SPHash hash, char *key, HashResult *error);

bool hashContains(SPHash hash, char *key, HashResult *error);

bool hashIsEmpty(SPHash hash, HashResult *error);

int hashGetSize(SPHash hash, HashResult *error);

#endif
