#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
		int d1_occurences;
		int d2_occurences;
		int d3_occurences;
		int docFreq;
        struct llnode* next;
};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};
struct LList
{
	struct llnode* head;
};


struct hashmap* hm_create(int num_buckets);
struct llnode* hm_get(struct hashmap* hm, char* word);
void hm_put(struct hashmap* hm, char* word, int d1_occurences, int d2_occurences, int d3_occurences, int docFreq);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);

#endif
