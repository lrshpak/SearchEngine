#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct hashmap* hm_create(int num_buckets)
{
	//allocates a hashmap and sets every bucket to NULL
	struct hashmap* hm =(struct hashmap*) malloc(sizeof(struct hashmap));
	hm->map = malloc(sizeof(struct llnode)*num_buckets);
	
	int i; 
	
	for (i = 0; i < num_buckets; i++)
	{
		hm->map[i] = NULL;
		
	}
	// sets number of elements to zero because nothing has been added, and num buckets to the number passed in
	hm->num_elements = 0;
	hm->num_buckets = num_buckets;
	return hm;
}


struct llnode* hm_get(struct hashmap* hm, char* word)
{
	// gets the hashed word back to find what bucket to search in
	int checkBucket = hash(hm, word);
	
	// sets iterator to the beginning of the bucket
	struct llnode* iterator = hm->map[checkBucket];
	
		// goes through the entire list until it finds the node
		while (iterator != NULL)
		{
			//printf("in while in get\n");
			if (!strcmp(iterator->word, word) )
			{
				
				// returns the whole node, so all its parameters can be used in the search engine
				return iterator;
			}
			iterator = iterator->next;
		}
	// if the node cant be found, nothing is returned	
	return NULL;

}

void hm_put(struct hashmap* hm, char* word, int d1_occurences, int d2_occurences, int d3_occurences, int docFreq)
{
	
	struct llnode *theNode = (struct llnode*) malloc(sizeof(struct llnode));
	// copies the word because strtok passes in a pointer to a word
	char* wordCopy = (char*)malloc(sizeof(char)*strlen(word));
	strcpy(wordCopy, word);
	// hashes the bucket to put the word
	int whatBucket = hash(hm, wordCopy);
	
	// if that bucket is null then 
	if (hm->map[whatBucket] == NULL)
	{
		// sets the nodes with proper parameters
		theNode->word = wordCopy;
		
		theNode->d1_occurences=d1_occurences;
		theNode->d2_occurences= d2_occurences;
		theNode->d3_occurences = d3_occurences;
		theNode->docFreq = docFreq;

		theNode->next = NULL;
		hm->map[whatBucket] = theNode;
		
	}
	else
	{
	// it then goes through and if the node is already there 
	struct llnode* iterator = hm->map[whatBucket];
	int add = 1;
		while (iterator != NULL)
		{
			if (!(strcmp(iterator->word, word)))
			{
				
				
				add = 0;
			}
			if (iterator->next == NULL)
			{
				break;
			}
			iterator = iterator->next;

			
		}
		// if the node is not there then it creates a new node with the proper parameters
		if (add)
		{
			struct llnode* theNode = (struct llnode*) malloc(sizeof(struct llnode));

			
			theNode->word = wordCopy;

			theNode->d1_occurences = d1_occurences;
			theNode->d2_occurences = d2_occurences;
			theNode->d3_occurences = d3_occurences;
			theNode->next = NULL;
			iterator->next = theNode;
			
			
		}
	}
}

void hm_remove(struct hashmap* hm, char* word)
{
	
	
	int whatBucket = hash(hm, word);
	// the bucket that got hashed has nothing in it 
	if (hm->map[whatBucket] == NULL)
	{
		printf("There are no nodes to remove");

	}
	else
	{
		struct llnode* iterator = hm->map[whatBucket];
		int index = 0;
		int count = 0;
		// goes until it finds the word that is to be removed, saves its index
		while (strcmp(iterator->word, word))
		{
			iterator = iterator->next;
			index = index + 1;

			if (!(strcmp(iterator->word, word)) && iterator->next == NULL) // if the the next node is null and it hasnt found the node, its not there 
			{
				printf("No Node");
			}


		}
		// if the iterator is the head node then it deletes and sets a new head node
		if (iterator == hm->map[whatBucket])
		{
			struct llnode* temp = (struct llnode*) malloc(sizeof(struct llnode));
			temp = hm->map[whatBucket];
			hm->map[whatBucket] = iterator->next;
			temp = NULL;
			free(temp);
		}
		else
		{
			// if it is not the head node
			 struct llnode* listPtr = (struct llnode*) malloc(sizeof(struct llnode));
			listPtr = hm->map[whatBucket];
			// runs until it is at the index before the node to remove
			while (count < index - 1)
			{
				listPtr = listPtr->next;
				count = count + 1;

			}
			// removes the node
			struct llnode* tempList = (struct llnode*) malloc(sizeof(struct llnode));
			tempList = listPtr->next;
			listPtr->next = listPtr->next->next;
			tempList = NULL;
			free(tempList);
		}
	}
	
	
}
void hm_destroy(struct hashmap* hm)
{
	int i = 0;
	{
		// goes through all the nodes and buckets
		while (i < hm->num_buckets)
		{
			struct llnode* temp = hm->map[i];
			while (temp != NULL)
			{
				// sets current node to a tem and frees the temp
				struct llnode* itTemp = temp;
				free(itTemp);
				temp = temp->next;
			}
			// frees the bucket
			struct llnode* mapTemp = hm->map[i];
			free(mapTemp);
			i++;
		}
	}
}


int hash(struct hashmap* hm, char* word)
{
	
	int i;
	// adds ASCII values of each letter together and mods it by num buckets to get the hash value
	int sum =0;
	for(i=0; i<(int)strlen(word); i++)
	{
		sum = sum + (int)word[i];
	}
	
	sum= sum % hm->num_buckets;
	return sum;
	

	
}
