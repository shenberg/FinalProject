
#include <SPHash.h>
#include <SPList.h>

#define NUM_ENTRIES 100
#define PRIME1 571
#define PRIME2 31

int hashKey(const char *key) {
	if (key == null) {
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
	SPList *table;
	size_t size;

};


SPHash hashCreate() {
	hash_t *hash = (hash_t *)malloc(sizeof(hash_t));

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
			/* alloc failed, clean up */
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

/**
 * hashDestroy: Deallocates an existing hashtable. Clears all elements by using
 * the stored free function.
 *
 * @param hash Target SPHash to be deallocated. If hash is NULL nothing will be
 * done
 */
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

HashResult hashGetValue(SPHash hash, const char *key, double *out) {
	if ((NULL == hash) || (NULL == key) || (NULL == out)) {
		return SP_HASH_NULL_ARGUMENT;
	}

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found! */
			*out = getElementValue(e);
			return SP_HASH_OK;
		}
	}
	/* TODO: change design - not found simply does nothing */
	return SP_HASH_OK;
}

HashResult hashInsert(SPHash hash, const char *key, double value) {
	if ((NULL == hash) || (NULL == key)) {
		return SP_HASH_NULL_ARGUMENT;
	}

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found - replace value */
			setELementValue(e, value);
			return SP_HASH_OK;
		}
	}
	/* match not found, insert new item */
	SPListElement newElement = createElement(key, value);
	if (NULL == newElement) {
		return SP_HASH_OUT_OF_MEMORY;
	}

	HashResult result = SP_HASH_OK;
	/* items != NULL, newElement != NULL => possible error is out-of-memory */
	if (SP_LIST_SUCCESS == listInsertFirst(items, newElement)) {
		/* we added a new item, didn't replace an existing one */
		hash->size++;
	} else {
		result = SP_HASH_OUT_OF_MEMORY;
	}
	destroyElement(newElement);
	return result;
}

HashResult hashDelete(SPHash hash, const char *key) {
	if ((NULL == hash) || (NULL == key)) {
		return SP_HASH_NULL_ARGUMENT;
	}

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found - remove */
			ListResult result = listRemoveCurrent(items);
			assert(result == SP_LIST_SUCCESS);
			/* removed an item so update item count */
			hash->size--;
			assert(hash->size >= 0);
			return SP_HASH_OK;
		}
	}
	/* TODO: design work */
}

HashResult hashContains(SPHash hash, const char *key, bool *found) {
	if ((NULL == hash) || (NULL == key) || (NULL == found)) {
		return SP_HASH_NULL_ARGUMENT;
	}

	SPList items = hash->table[hashKey(key)];
	LIST_FOREACH(SPListElement, e, items) {
		if (isElementStrEquals(e, key)) {
			/* match found! */
			*found = true;
			return SP_HASH_OK;
		}
	}
	*found = false;
	return SP_HASH_OK;
}

HashResult hashIsEmpty(SPHash hash, bool *isEmpty) {
	if ((NULL == hash) || (NULL == isEmpty)) {
		return SP_HASH_NULL_ARGUMENT;
	}
	*isEmpty = (hash->size > 0);
	return SP_HASH_OK;
}

HashResult hashGetSize(SPHash hash, size_t *size) {
	if ((NULL == hash) || (NULL == size)) {
		return SP_HASH_NULL_ARGUMENT;
	}
	*size = hash->size;
	return SP_HASH_OK;
}
