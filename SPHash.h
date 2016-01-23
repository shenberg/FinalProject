/*
 * Author: Roee Shenberg
 */

#ifndef SPHASH_H__
#define SPHASH_H__

#include <stdbool.h>

typedef struct hash_t* SPHash;

/**
 * possible error causes
 */
typedef enum HashResult_t {
	SP_HASH_OK = 0,
	SP_HASH_OUT_OF_MEMORY = 1,
	SP_HASH_NULL_ARGUMENT = 2
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

/**
 * hashGetValue: Get a value got a key from the hash-table
 *
 * Errors: SP_HASH_NULL_ARGUMENT if hash==NULL,
 *         SP_HASH_OUT_OF_MEMORY on copy allocation failure
 *
 * @param hash hashtable to read from
 * @param key the key to look up
 * @param error out param, operation status. Can be NULL if don't care
 *
 * @return pointer to copy of value if key is in hash table (user must free())
 *         NULL if key not in hash table or on error
 */
double *hashGetValue(SPHash hash, char *key, HashResult *error);

/**
 * hashInsert: insert a key->value mapping to the hash-table. If key already
 * maps to old_value, make it map to value instead.
 *
 * Errors: SP_HASH_NULL_ARGUMENT if hash==NULL,
 *         SP_HASH_OUT_OF_MEMORY on internal allocation failure
 *
 * @param hash hashtable to read from
 * @param key the key
 * @param value the value
 * @param error out param, operation status. Can be NULL if don't care
 *
 * @return true if key set successfully, false otherwise (look at error
 *         for cause)
 * 
 *
 */
bool hashInsert(SPHash hash, char *key, double value, HashResult *error);

/**
 * hashDelete: Remove mapping for given key
 *
 * Errors: SP_HASH_NULL_ARGUMENT if hash==NULL
 *
 * @param hash hashtable to remove the key from
 * @param key the key to remove
 * @param error out param, operation status. Can be NULL if don't care
 *
 * @return true if key was removed successfully, false otherwise
 *         (error or key not in hash-table)
 */
bool hashDelete(SPHash hash, char *key, HashResult *error);

/**
 * hashContains: does the hash table contain a given key?
 *
 * Errors: SP_HASH_NULL_ARGUMENT if hash==NULL
 *
 * @param hash hashtable to read from
 * @param key the key to test for
 * @param error out param, operation status. Can be NULL if don't care
 *
 * @return true if key in hash-table, false otherwise
 */
bool hashContains(SPHash hash, char *key, HashResult *error);

/**
 * hashIsEmpty: does the hash-table have any key->value mappings?
 *
 * Errors: SP_HASH_NULL_ARGUMENT if hash==NULL
 *
 * @param hash hashtable to test
 * @param error out param, operation status. Can be NULL if don't care
 *
 * @return true if hash table is valid and has no mappings, false otherwise
 */
bool hashIsEmpty(SPHash hash, HashResult *error);

/**
 * hashGetSize: get number of key->value mappings in the hash table
 *
 * Errors: SP_HASH_NULL_ARGUMENT if hash==NULL
 *
 * @param hash hashtable to read from
 * @param error out param, operation status. Can be NULL if don't care
 *
 * @return number of key->value mappings in the hash-table. -1 on error.
 */

int hashGetSize(SPHash hash, HashResult *error);

#endif
