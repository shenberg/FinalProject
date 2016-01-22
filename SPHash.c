
#include "SPHash.h"
#include "SPList.h"
#include <stdlib.h>
#include <assert.h>


/* code from docs to hash a string */
#define NUM_ENTRIES 100
#define PRIME1 571
#define PRIME2 31

int hashKey(const char *key) {
	if (key == NULL) {
		return -1;
	} else {
		int result = PRIME1;
		for(const char *p = key; p; p++) {
			result = result*PRIME2 + (int)*p;
			result %= NUM_ENTRIES;
		}
		return result;
	}
}



struct hash_t {
	/* table of NUM_ENTRIES size, each entry being a list
	 * of keys sharing the same hash */
	SPList *table;
	/* total number of keys in the table at the moment */
	int size;
};

/* set error ptr to given error err, if error is not NULL */
#define SET_ERROR(err) if (NULL != error) { *error = err; }


SPHash hashCreate() {
	SPHash hash = (struct hash_t *)malloc(sizeof(struct hash_t));

	if (NULL == hash) {
		return NULL;
	}

	hash->table = (SPList *)malloc(sizeof(SPList)*NUM_ENTRIES);
	if (NULL == hash->table) {
		free(hash);
		return NULL;
	}

	/* initialize the table */
	for(int i = 0; i < NUM_ENTRIES; i++) {
		SPList newList = listCreate();
		if (NULL != newList) {
			hash->table[i] = newList;
		} else { /* (NULL == newList) */
			/* alloc failed, clean up all already-allocated lists */
			for(int j = i-1; j >= 0; j++) {
				listDestroy(hash->table[j]);
				free(hash->table);
				free(hash);
			}
			return NULL;
		}
	}

	hash->size = 0;
	return hash;
}

void hashDestroy(SPHash hash) {
	if (NULL == hash) {
		return;
	}

	for(int i = 0; i < NUM_ENTRIES; i++) {
		listDestroy(hash->table[i]);
	}
	free(hash->table);
	free(hash);
}

double *hashGetValue(SPHash hash, char *key, HashResult *error) {
	if ((NULL == hash) || (NULL == key)) {
		SET_ERROR(SP_HASH_NULL_ARGUMENT);
		return NULL;
	}

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found! */
			SET_ERROR(SP_HASH_OK);
			return getElementValue(e);
		}
	}
	SET_ERROR(SP_HASH_OK);
	return NULL;
}

bool hashInsert(SPHash hash, char *key, double value, HashResult *error) {
	if ((NULL == hash) || (NULL == key)) {
		SET_ERROR(SP_HASH_NULL_ARGUMENT);
		return false;
	}

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found - replace value */
			setELementValue(e, value);
			SET_ERROR(SP_HASH_OK);
			return false;
		}
	}
	/* match not found, insert new item */
	SPListElement newElement = createElement(key, value);
	if (NULL == newElement) {
		SET_ERROR(SP_HASH_OUT_OF_MEMORY);
		return false;
	}

	/* items != NULL, newElement != NULL => possible error is out-of-memory */
	if (SP_LIST_SUCCESS == listInsertFirst(items, newElement)) {
		/* we added a new item, didn't replace an existing one */
		hash->size++;
		destroyElement(newElement);
		SET_ERROR(SP_HASH_OK);
		return true;
	} else {
		destroyElement(newElement);
		SET_ERROR(SP_HASH_OUT_OF_MEMORY);
		return false;
	}
}

bool hashDelete(SPHash hash, char *key, HashResult *error) {
	if ((NULL == hash) || (NULL == key)) {
		SET_ERROR(SP_HASH_NULL_ARGUMENT);
		return false;
	}

	SET_ERROR(SP_HASH_OK);

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found - remove */
			ListResult result = listRemoveCurrent(items);
			/* we know list iterator is valid */
			assert(result == SP_LIST_SUCCESS);
			/* removed an item so update item count */
			hash->size--;
			assert(hash->size >= 0);
			return true;
		}
	}

	return false;
}

bool hashContains(SPHash hash, char *key, HashResult *error) {
	if ((NULL == hash) || (NULL == key)) {
		SET_ERROR(SP_HASH_NULL_ARGUMENT);
		return false;
	}

	SET_ERROR(SP_HASH_OK);
	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found! */
			return true;
		}
	}
	/* if not found */
	return false;
}

bool hashIsEmpty(SPHash hash, HashResult *error) {
	if (NULL == hash) {
		SET_ERROR(SP_HASH_NULL_ARGUMENT);
	}
	SET_ERROR(SP_HASH_OK);
	return (hash->size > 0);
}

int hashGetSize(SPHash hash, HashResult *error) {
	if (NULL == hash) {
		SET_ERROR(SP_HASH_NULL_ARGUMENT);
		return -1;
	}
	SET_ERROR(SP_HASH_OK);
	return hash->size;
}
