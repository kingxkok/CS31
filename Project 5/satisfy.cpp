// Example program
#include <iostream>
#include <cstring>
#include <cctype>
#include <assert.h>
using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_LENGTH = 200;

int normalizeRules(char word1[][MAX_WORD_LENGTH+1],
                   char word2[][MAX_WORD_LENGTH+1],
                   int distance[],
                   int nRules);

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1],
                          const char word2[][MAX_WORD_LENGTH+1],
                          const int distance[],
                          int nRules,
                          const char document[]);



bool isAllAlpha(char word[]){
	for(int i = 0; word[i]!='\0'; i++){
		if(!isalpha(word[i])) return false;
	}
	return true;
}

//checks if the pair exists and returns distance if they do, else returns -1
int distanceFunc(char word1[], char word2[], 
	char word1List[][MAX_WORD_LENGTH+1], 
	char word2List[][MAX_WORD_LENGTH+1], int distance[],
	int nRules, int& index){
	for(int i = 0; i<nRules; i++){
		if(strcmp(word1, word1List[i]) == 0 )
			if(strcmp(word2, word2List[i]) == 0 ){
				index = i;
				return distance[i];
			}
	}
	return -1;
}


int normalizeRules(char word1[][MAX_WORD_LENGTH+1],
                   char word2[][MAX_WORD_LENGTH+1],
                   int distance[],
                   int nRules){
	//toLower
	for(int i = 0; i<nRules; i++){
		//word1[]
		for(int w1Index = 0; word1[i][w1Index] != '\0'; w1Index++ ){
			word1[i][w1Index] = tolower(word1[i][w1Index]);
		}
		//word2[]
		for(int w2Index = 0; word2[i][w2Index] != '\0'; w2Index++ ){
			word2[i][w2Index] = tolower(word2[i][w2Index]);
		}
	}
	//to make sure word1 <= word2 to remove duplicates more easily
	for(int i = 0; i<nRules; i++){
		if(strcmp(word1[i] , word2[i]) > 0){
			//swap
			char temp[MAX_WORD_LENGTH+1];
			for(int w1Index = 0; ; w1Index++ ){  //probably could've/should've used strcpy here
				temp[w1Index] = word1[i][w1Index];
				if(word1[i][w1Index] == '\0')
					break;
			}
			for(int w2Index = 0; ; w2Index++ ){
				word1[i][w2Index] = word2[i][w2Index];
				if(word2[i][w2Index] == '\0') break;
			}
			for(int w1Index = 0; ; w1Index++){
				word2[i][w1Index] = temp[w1Index];
				if(temp[w1Index]=='\0') break;
			}
			//end of swap
		}
	}

	//remove duplicates by adding to new array
	char word1copy[nRules][MAX_WORD_LENGTH+1];
	char word2copy[nRules][MAX_WORD_LENGTH+1];

	int copyIndex = 0;
	int replaceIndex;

	for(int i = 0; i<nRules; i++){
		if(isAllAlpha(word1[i]) && isAllAlpha(word2[i]) && distance[i]>0){
			if(  distanceFunc( word1[i], word2[i], word1copy, word2copy, distance, nRules, replaceIndex) == -1 ){
				//^checks if the word pair exists in word1copy&word2copy 
				//and returns distance if they do, else returns -1
				//-1 means they don't exist in the copies yet so add to copies
				strcpy(word1copy[copyIndex], word1[i]);
				strcpy(word2copy[copyIndex], word2[i]);
				distance[copyIndex] = distance[i];
				copyIndex++;
			}
			else if( distanceFunc( word1[i], word2[i], word1copy, word2copy, distance, nRules, replaceIndex) < distance[i] ){
				distance[replaceIndex] = distance[i];
			}
		}
	}

	//copyIndex is now the length of new arrays
	for(int i = 0; i < copyIndex; i++){
		//copy back to original
		strcpy(word1[i], word1copy[i]);
		strcpy(word2[i], word2copy[i]);
	}

	return copyIndex;




}

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1],
                          const char word2[][MAX_WORD_LENGTH+1],
                          const int distance[],
                          int nRules,
                          const char document[]){
	//copy document into array of words (char[]s)
	//for each word in document, check if word is in list1, and note its index
	//if exist then check if document is, at anywhere from index-distance to index+distance (skipping index),
	//the corresponding list2 word
	//if word2 is found within range, mark that rule as satisfied

	//copy document into array of words (char[]s)
	char documentCopy[MAX_DOC_LENGTH+1][MAX_DOC_LENGTH /*max doc's word length*/ +1];
	int wordIndex = 0;
	int letterIndex = 0;
	for(int i = 0; ; i++){
		if( isalpha(document[i]) ){
			documentCopy[wordIndex][letterIndex] = tolower(document[i]);
			letterIndex++;
		}
		else if( document[i] == ' ' ){
			documentCopy[wordIndex][letterIndex] = '\0';
			wordIndex++;
			letterIndex = 0;
		}
		else if( document[i] == '\0' ){
			documentCopy[wordIndex][letterIndex] = '\0';
			wordIndex++; //now represents number of words
			break;
		}
		//else do nothing
	}
	//remove blank arrays
	int blanksRemoved = 0;
	for(int i = 0; i < wordIndex; i++){
		if(strcmp(documentCopy[i], "") != 0
		&&strcmp(documentCopy[i], " ")!=0 
		&& strcmp(documentCopy[i], "\t")!=0 
		) {
			strcpy(documentCopy[i-blanksRemoved], documentCopy[i]);
		}
		else blanksRemoved++;
	}

	//record how many words
	const int numWords = wordIndex - blanksRemoved;

	//track which rules are satisfied
	bool satisfied[nRules];
	for(int i = 0; i < nRules; i++){
		satisfied[i] = false;
	}

	//turn satisfied to true
	for(int i = 0; i<numWords; i++){ //for every word in document documentCopy[i] is first word
		for(int wordListIndex = 0; wordListIndex<nRules; wordListIndex++){ //check list1 
			if(strcmp(documentCopy[i],word1[wordListIndex])==0 ){ //check if exists in list1
				//found, so check list2 within the distance and bounds of array
				for( int documentIndex = max(0,i - distance[wordListIndex]);  //documentCopy[documentIndex] is 2nd word
					documentIndex <=  min(i+distance[wordListIndex], numWords-1); 
					documentIndex++){
					if(documentIndex == i) continue;
					if( strcmp(documentCopy[documentIndex], word2[wordListIndex]) ==0 ){
						satisfied[wordListIndex] = true; //record that this rule has been satisfied
					}
				}

			}
		}
		
	}

	//calculate score
	int score = 0;
	for(int i = 0; i<nRules; i++){ 
		if(satisfied[i]) score++;
	}
	return score;


}

int main()
{
	   const int TEST1_NRULES = 7;
    char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "mad",       "derAnGed", "nefar-ious", "h ave",	 "mad",	"henises", "harAMbe"
    };
    char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "scientist",  "robot",    "plot",      "mad",	 "mad",	"hArambe", "henises"
    };
    int test1dist[TEST1_NRULES] = {
        2,           4,          1,           13, 	  6,	14, 	  	12
    };
    
    cout << normalizeRules(test1w1, test1w2, test1dist, TEST1_NRULES) << endl;
  
  for(int i = 0; i < TEST1_NRULES; i++){
    	cout << test1w1[i] << "	" << test1w2[i] << "	" << test1dist[i] << endl;
    }
}

int main2()
{
	string separator = "!@#$^&*()";
    const int TEST1_NRULES = 5;
            char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "mad",       "deranged", "nefarious", "have", "robot"
            };
            char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
                "scientist", "robot",    "plot",      "mad", "robot"
            };
            int test1dist[TEST1_NRULES] = {
                2,           4,          1,           13, 3
            };
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
            cout << separator << endl;
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "The mad UCLA scientist unleashed    a deranged robot.") == 2);
            cout << separator << endl;
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "**** 2016 ****") == 0);
            cout << separator << endl;
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "  That plot: NEFARIOUS!") == 1);
            cout << separator << endl;
            cout << calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "deranged deranged robot deranged robot robot") << " MARKER " << endl;
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "deranged deranged robot deranged robot robot") == 2);
            cout << separator << endl;
            assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                "That scientist said two mad scientists suffer from deranged-robot fever.") == 0);

            cout << "All tests succeeded" << endl;

   
   return 0;
}



