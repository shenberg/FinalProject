#ifndef SPHASH_H__
#define SPHASH_H__

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
 * done
 */
void hashDestroy(SPHash hash);

HashResult hashGetValue(SPHash hash, const char *key, double *out);

HashResult hashInsert(SPHash hash, const char *key, double value);

HashResult hashDelete(SPHash hash, const char *key);

HashResult hashContains(SPHash hash, const char *key, bool *found);

HashResult hashIsEmpty(SPHash hash, bool *isEmpty);

HashResult hashGetSize(SPHash hash, size_t *size);

#endif
