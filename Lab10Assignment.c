#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    int isWord;
    struct Trie* next[26];
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *node = (struct Trie*)malloc(sizeof(struct Trie));
    if (node) {
        node->isWord = 0;
        for (int i = 0; i < 26; ++i)
            node->next[i] = NULL;
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *traverse = pTrie;
    while (*word) {
        int index = *word - 'a';
        if (!traverse->next[index]) {
            traverse->next[index] = createTrie();
        }
        traverse = traverse->next[index];
        word++;
    }
    traverse->isWord++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *traverse = pTrie;
    while (*word && traverse) {
        traverse = traverse->next[*word - 'a'];
        word++;
    }
    return (traverse && *word == '\0') ? traverse->isWord : 0;
}

// Deallocate the trie structure
struct Trie* deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL) return NULL;  // If the node is NULL, just return NULL

    // Recursively free all children
    for (int i = 0; i < 26; i++) {
        if (pTrie->next[i] != NULL) {
            pTrie->next[i] = deallocateTrie(pTrie->next[i]);  // Recursively deallocate each child and set to NULL
        }
    }

    // After all children have been deallocated, free the current node
    free(pTrie);
    return NULL;  // Return NULL to the caller
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file){
        return -1;
    }

    int numOfWords = 0;
    fscanf(file, "%i", &numOfWords);

    for (int i = 0; i < numOfWords; i++){
        pInWords[i] = (char*)malloc(sizeof(char) * 100);
        fscanf(file, "%s", pInWords[i]);
    }
    fclose(file);

    return numOfWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
    pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}