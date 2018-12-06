#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
struct hashmap* training(char** files, int numBuckets);
struct hashmap* documentFrequency(struct hashmap* theMap);
struct hashmap* stopWords(struct hashmap*);
void readQuery(struct hashmap* theMap, char* searchQuery);
void rank(struct hashmap* theMap, char** theWords,int numWords);
 struct hashmap* training(char** files, int numBuckets)
{
	struct hashmap* theMap = hm_create(numBuckets);
	

	FILE* dataFile;
	int i;
	for (i = 0; i < 3; i++)
	{
		// opens data file to read from
		dataFile = fopen(files[i], "r");
		char getWords[100];
		// while there are still words to read , read from file
		while ((fgets(getWords, 100, dataFile)) != NULL)
		{
			// uses strtok to get a word before each space
			char *theWord = strtok(getWords, " ");
			while (theWord != NULL)
			{
				// gets the word from the hash map
				struct llnode* occuring = hm_get(theMap, theWord);
				
				// if the word is not in the hashmap put it in with the corrent occurences depending on the file
				if (occuring == NULL)
				{
					if (!(strcmp(files[i], "D1.txt")))
					{
						hm_put(theMap, theWord, 1, 0, 0,0);
						
					}
					if(!(strcmp(files[i], "D2.txt")))
					{
						hm_put(theMap, theWord,0, 1, 0,0);
						
					}
					if (!(strcmp(files[i], "D3.txt")))
					{
						hm_put(theMap, theWord, 0, 0, 1,0);
						
					}
				}
				// if the word is in the hashmap all ready, update the occurences depending on the file
				else
				{
					if (!(strcmp(files[i], "D1.txt")))
					{

						int occur = occuring->d1_occurences;
						occur = occur + 1;
						occuring->d1_occurences=occur;
						
					}
					if (!(strcmp(files[i], "D2.txt")))
					{
						
						int occur = occuring->d2_occurences;
						occur = occur + 1;
						occuring->d2_occurences=occur;

					}
					if(!(strcmp(files[i],"D3.txt")))
					{
						int occur = occuring->d3_occurences;
						occur = occur + 1;
						occuring->d3_occurences=occur;

					}
					
				}
				
				theWord = strtok(NULL, " ");

			}
			
		}
		
	}
	// call documentFequency on the map with updated occurences
	struct hashmap* docFreqMap = documentFrequency(theMap);
	
	return docFreqMap;
}
struct hashmap* documentFrequency(struct hashmap* theMap)
{
	
	
	int i = 0;
	// iterates through the whole hashmap
	while (i < theMap->num_buckets)
	{
		struct llnode* temp = theMap->map[i];
		struct llnode* iterator = temp;
		// depending on the num occurences for each file is how docFre is updated
		while (iterator != NULL)
		{
			int docFre = 0;
			if (iterator->d1_occurences > 0)
			{
				docFre = docFre + 1;
			}
			if (iterator->d2_occurences)
			{
				docFre = docFre + 1;

			}
			if (iterator->d3_occurences)
			{
				docFre = docFre + 1;
			}
			// updates the nodes doc frequency
			iterator->docFreq = docFre;
			iterator = iterator->next;
		}
		i++;
	}

	// returns the updated hashmap
	return theMap;
}
struct hashmap* stopWords(struct hashmap* theMap)
{
	
	struct llnode* temp;
	
	int i=0;
	// iterates through entire hashmap
	while (i < theMap->num_buckets)
	{
		temp = theMap->map[i];
		struct llnode* iterator = temp;
		while (iterator != NULL)
		{
			// calculates the inverse docFrequency 
			double decimalDoc = (double)iterator->docFreq;
			double inverseDoc = log10(3.0 / decimalDoc);
			// if it is 0 then it removes the word
			if (inverseDoc == 0)
			{
				 
				hm_remove(theMap, iterator->word);
			}
			iterator = iterator->next;
		}
		i++;
	}
	
	
	return theMap;
}
void readQuery(struct hashmap* theMap, char* searchQuery)
{
	int numWords = 0;
	int j = 0;
	// counts how many words there are
	while (j<100)
	{
	// if there is a space it adds to the word count
		if (searchQuery[j] == ' ')
		{
			numWords = numWords + 1;
		}
		if (searchQuery[j] == '\000')// breaks after the last word
		{
			numWords = numWords + 1;
			break;
		}

		j++;
	}
	// creates an array for the words of the query to go in 
	char** theWords = malloc(sizeof(char*)*numWords);

	int i = 0;

	{
		char* individualWord;
		while (i < numWords)
		{
			
				 individualWord = strtok(searchQuery, " ");
			
			while (individualWord != NULL)
			{
				// copies word into the array 
				char* wordCopy = (char*)malloc(sizeof(char)*strlen(individualWord));
				strcpy(wordCopy, individualWord);
				theWords[i] = individualWord;
				i++;
				individualWord = strtok(NULL, " ");
			}
			i++;
			
		}
	}
	//calls ranks 
	rank(theMap, theWords, numWords);
	
}

void rank(struct hashmap* theMap,char** theWords, int numWords)
{
	
	int i = 0;
	double doc1Sum;
	double doc2Sum;
	double doc3Sum;
	
	while (i < numWords)
	{
		//tf-idf variables
		double doc1;
		double doc2;
		double doc3;
		// gets the node for the word, if the word isnt there then frequencys are 0
		struct llnode* temp = hm_get(theMap, theWords[i]);
		if (temp == NULL)
		{
			
			doc1 = 0;
			doc2 = 0;
			doc3 = 0;

		}
		else // if the node is there 
		{
			// gets the doc frequency for that node
			double freq = (double)temp->docFreq;
			// calculates the tf-idf for each word for each document
			doc1 = ((double)(temp->d1_occurences))* (log10(3.0 / freq));
			doc2 = ((double)(temp->d2_occurences))* (log10(3.0 / freq));
			doc3 = ((double)(temp->d3_occurences))* (log10(3.0 / freq));
			// keeps a running sum for each document
			doc1Sum = doc1Sum + doc1;
			doc2Sum = doc2Sum + doc2;
			doc3Sum = doc3Sum + doc3;

		}
		i++;

	}


	
	//create a char array of the files to sort them 
	char** fileArr = malloc(sizeof(char*) * 3);
	fileArr[0] = "D1.txt";
	fileArr[1] = "D2.txt";
	fileArr[2] = "D3.txt";
	// also create an array of the tf-idf values 
	double tfIdf [3];
	tfIdf[0] = doc1Sum;
	tfIdf[1] = doc2Sum;
	tfIdf[2] = doc3Sum;
	int g;
	int j;
	char* tempWord;
	// goes through and sorts the file array by the tfIdf
	
	for (g = 0; g < 3; g++)
	{
		
		for (j = g; j <3; j++)
		{
			// compares and if true it swaps them
			if (tfIdf[j] > tfIdf[g])
			{
				tempWord = fileArr[g];
				fileArr[g] = fileArr[j];
				fileArr[j] = tempWord;
			}
		}
	}
	
	int h;
	int b;
	printf("Document Order:\n");
	for (h = 0; h < 3; h++)
	{
		b = h + 1;
		printf("%d. %s\n",b, fileArr[h]);
	}
	
	
}
int main(void)
{
	// list of files to be read
	char** files = malloc(sizeof(char*) * 3);
	files[0] = "D1.txt";
	files[1] = "D2.txt";
	files[2] = "D3.txt";

	//asks user for number of buckets to create hashmap and sends it to training() and removes stop words
	printf("Enter the number of buckets for the hashmap(int)\n");
	int numBuckets;
	scanf("%d", &numBuckets);
	struct hashmap* theMap = training(files, numBuckets);
	struct hashmap* stopMap= stopWords(theMap);

	// start query process 
	printf("Enter X to exit or S to search\n");
	char searchExit;
	scanf(" %c", &searchExit);
	char searchQuery[100]; // decided to assume the query was not longer than 100 
	
	//depending on the search char 
	while (1)
	{
		if ((searchExit != 'S') && searchExit != 'X')
		{
			printf("Not a proper search/exit character");
			break;
		}
		if (searchExit == 'X')
		{
			printf("Exiting\n");
			break;


		}
		// if S is enter prompts user to enter a search
		if (searchExit == 'S')
		{
			printf("what do you want to search?\n");
			scanf(" %[^\n]s", searchQuery);
			// calls read query and then asks user if they want to search again or not
			readQuery(stopMap, searchQuery);
			printf("Enter X to exit or S to search\n");
			scanf(" %c", &searchExit);
			
		}

	}
	
	// destroy the map
	hm_destroy(stopMap);
	

	return 0;
}