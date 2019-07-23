#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <map>
using namespace std;

bool punctuationArr[127]; // LUT (maps punctuation -> bool)
vector<string> chapterList; // List of chapters
int totalNumberOfWords; // Total number of words in book 
map<string, int> wordFrequencyMap; // Maps word -> frequency
multimap<int, string, greater<int>> reverseWordFrequencyMapLarge; // Maps frequency -> word (from greatest->least)
multimap<int, string> reverseWordFrequencyMapSmall; // Maps frequency -> word (from least->greatest)
vector<string> mostCommonWordsList; // List of words in 1000Words.txt 

// Custom functions
int min(int x, int y) { return (x < y) ? x : y; }
int max(int x, int y) { return (x > y) ? x : y; }
bool isLowercase(string word)
{
	for (int i = 0; i < word.size(); i++) if (isupper(word[i]) && i != 0) return false;
	return true;
}
string setLowercase(string word)
{
	string s = "";
	for (int i = 0; i < word.size(); i++)
	{
		if (isupper(word[i])) s += tolower(word[i]);
		else s += word[i]; 
	}
	return s;
}
bool isAlphaWord(string word)
{
	for (int i = 0; i < word.size(); i++) if (!isalpha(word[i])) return false;
	return true;
}


// Helper function: A 127-size LUT mapping punctuation -> boolean 
void populatePunctuationArr()
{
	memset(punctuationArr, 0, 127 * sizeof(bool)); // Initialize punctuationArr to 0
	
	string punctuationChars = " \n\r\t!\"'#$%^&*(){}[]<>+-=_/,.:;~`?|"; // Array of punctuation characters to omit
	for (int i = 0; i < punctuationChars.size(); i++)
	{
		int asciiIdx = (int) punctuationChars.at(i);
		punctuationArr[asciiIdx] = 1;
	}
	punctuationArr[0] = true; // Mark NULL character 
}

// Helper function: Strips leading punctuation from word
string stripLeadingPunctuation(string word)
{
	if (word == "") return word;
	int idx = 0;
	while (idx < word.size() && punctuationArr[(int)word[idx]] == 1) idx++;
	return word.substr(idx, word.size() - idx + 1); 
}

// Helper function: Strips trailing punctuation from word
string stripTrailingPunctuation(string word)
{
	if (word == "") return word;
	int idx = word.size() - 1;
	while (idx < word.size() && punctuationArr[(int)word[idx]] == 1) idx--;
	return word.substr(0, idx+1);
}

// Helper function: Parses book text file into chapters
void parseBookIntoChapters(string bookFileName)
{
	ifstream myFile (bookFileName);
	string line = "";

	populatePunctuationArr();

	string chapterText = "";
	if (myFile.is_open())
	{
		while (getline(myFile, line)) // Parse text by lines
		{
			stringstream ss(line); // Parse line into words 
			while (ss)
			{
				string word = "";
				ss >> word;
				totalNumberOfWords++; 

				if (word == "CHAPTER") // Begun new chapter
				{
					if(chapterText != "") chapterList.push_back(chapterText);
					chapterText = ""; // Clear chapter text 
				}
				else
				{
					chapterText += (word + " "); // Add word to chapter
					word = stripLeadingPunctuation(word); // Clean word
					word = stripTrailingPunctuation(word); // Clean word
					if(word != "") wordFrequencyMap[word]++; // Update map 
				}
			}
			chapterText.pop_back(); // Remove trailing whitespace
		}
		chapterList.push_back(chapterText); // Add chapter to list 
	}
	else // Could not open text file
	{
		cout << "Error opening file!" << endl;
	}
}

// Required Function#1
int getTotalNumerOfWords()
{
	return totalNumberOfWords;
}

// Required Function#2
int getTotalUniqueWords()
{
	return wordFrequencyMap.size(); 
}

// Helper function: populates maps of (frequency -> word)
void populateReverseWordFrequencyMaps()
{
	for (auto it : wordFrequencyMap)
	{
		string word = it.first;
		int count = it.second;
		reverseWordFrequencyMapLarge.insert({ count, word }); 
		reverseWordFrequencyMapSmall.insert({ count, word }); 
	}
}

// Required Function#3
vector<string> get20MostFrequentWords()
{
	vector<string> wordList;
	if (reverseWordFrequencyMapLarge.size() == 0) populateReverseWordFrequencyMaps();
	int ctr = 0;
	for (auto it : reverseWordFrequencyMapLarge)
	{
		if (ctr == 20) break;
		wordList.push_back(it.second);
		ctr++;
	}
	return wordList; 
}

// Helper function: Parses file containing 1000 most common English words 
vector<string> parseMostCommonWords(string fileName)
{
	ifstream myFile(fileName);
	string line = "";

	if (myFile.is_open())
	{
		while (getline(myFile, line)) // Parse file by lines
		{
			stringstream ss(line); // Parse line into words 
			while (ss)
			{
				string word = "";
				ss >> word;
				if(word != "") mostCommonWordsList.push_back(word);
			}
		}
	}
	else // Could not open file 
	{
		cout << "Error opening file!" << endl;
	}
	return mostCommonWordsList; // Return list of words in file
}

// Required Function#4
// Gets top-20 words (results vary on filter size)
vector<string> get20MostInterestingFrequentWords(string fileName, int filterSize)
{
	if(mostCommonWordsList.size() == 0) parseMostCommonWords(fileName);
	vector<string> filteredWordList(mostCommonWordsList.begin(), mostCommonWordsList.begin() + filterSize);

	vector <string> interestingWordList;
	
	if (reverseWordFrequencyMapLarge.size() == 0) populateReverseWordFrequencyMaps();
	int ctr = 0; 
	for (auto it : reverseWordFrequencyMapLarge)
	{
		if (ctr == 20) break;
		string word = it.second;
		int count = it.first;
		auto iter = find(filteredWordList.begin(), filteredWordList.end(), word);
		if (iter == filteredWordList.end())
		{
			interestingWordList.push_back(word);
			ctr++; 
		}
	}
	return interestingWordList;
}

// Required Function#5
vector<string> get20LeastFrequentWords()
{
	vector<string> wordList;
	if (reverseWordFrequencyMapSmall.size() == 0) populateReverseWordFrequencyMaps();
	int ctr = 0;
	for (auto it : reverseWordFrequencyMapSmall)
	{
		if (ctr == 20) break;
		wordList.push_back(it.second);
		ctr++;
	}
	return wordList;
}

// Required Function#6
vector<int> getFrequencyOfWord(string word)
{
	vector<int> wordFrequencyList;
	for (string chapter : chapterList) // Iterate over all chapters
	{
		if (chapter != "")
		{
			int wordCtr = 0;
			int currPos = 0;
			while (currPos != std::string::npos) // Get all occurrences of word in chapter 
			{
				wordCtr++;
				currPos += word.size(); 
				currPos = chapter.find(word, currPos);
			}
			wordFrequencyList.push_back(wordCtr-1);
		}
	}
	return wordFrequencyList; 
}

// Required Function#7
int getChapterQuoteAppears(string quote)
{
	int chapterCtr = 1;
	for (string chapter : chapterList)
	{
		int currPos = chapter.find(quote);
		if (currPos != -1) return chapterCtr;
		chapterCtr++; 
	}
	return -1; 
}

// Get the next word after the current word
vector<string> getWordAfter(string currWord)
{
	vector<string> nextWordList;
	for (string chapter : chapterList) 
	{
		int currPos = chapter.find(currWord); // Search for position 
		while (currPos != string::npos) // Search whole chapter for word
		{
			// If word is bounded by punctuation only (not part of another word)
			if (currPos - 1 >= 0 && punctuationArr[(int)chapter[currPos - 1]] == 1 && currPos + currWord.size() < chapter.size() && punctuationArr[(int)chapter[currPos + currWord.size()]] == 1)
			{
				int nextWordStartPos = (chapter.find(" ", currPos + 1)); // Find start of next word
				int nextWordEndPos = min(chapter.find(" ", nextWordStartPos+1), chapter.find("--", nextWordStartPos + 1)); // Find end of next word
				string nextWord = chapter.substr(nextWordStartPos+1, nextWordEndPos - nextWordStartPos); // Get next word based on start & end
				nextWord = stripLeadingPunctuation(nextWord); // Clean next word
				nextWord = stripTrailingPunctuation(nextWord); // Clean next word
				if(nextWord != "") nextWordList.push_back(nextWord); // Add next word to list
			}
			currPos += currWord.size(); // Update currPos
			currPos = chapter.find(currWord, currPos);
		}
	}
	return nextWordList; 
}

// Required Function#8
string generateSentence()
{
	srand(time(NULL)); // Seed random time 
	int ctr = 0;
	string sentence = "";
	string currWord = "The"; 
	while (ctr < 19)
	{
		vector<string> wordList = getWordAfter(currWord); // Get next word after current word
		if (wordList.size() != 0) // If a next word exists 
		{
			sentence += (currWord + " "); // Add current word to sentence
			if (wordList.size() == 1) currWord = wordList[0]; // Edge case
			else
			{
				while (true) // Loop to get a random word
				{
					int randNum = rand() % (wordList.size());
					currWord = wordList[randNum]; 
					if (currWord.size() == 1 || isLowercase(currWord)) break;
				}
			}
			ctr++; 
		}
	}
	sentence.pop_back(); // Remove trailing whitespace
	sentence += "."; // Append a period to end the sentence
	return sentence;
}

struct Node // Represents leaf in Trie 
{
	Node() : children(26, NULL), isLeaf(false) {}; 
	vector<Node*> children;
	bool isLeaf; // Marks if end of word 
};

// Helper Function: Adds word to Trie
void addWordToTrie(Node* root, string word)
{
	Node* curr = root;
	for (int i = 0; i < word.size(); i++)
	{
		int pos = (int)word[i] - 'a';
		if(curr->children[pos] == NULL) curr->children[pos] = new Node;
		curr = curr->children[pos]; 
	}
	curr->isLeaf = true; 
}

// Helper Function: Creates a new Trie with words from map
Node* createTrie()
{
	Node* root = new Node;
	int ctr = 0;
	if (reverseWordFrequencyMapLarge.size() == 0) populateReverseWordFrequencyMaps();
	for (auto it : reverseWordFrequencyMapLarge)
	{
		string word = it.second;
		word = setLowercase(word);
		if(isAlphaWord(word)) addWordToTrie(root, word);
		ctr++;
	}
	return root;
}

// Helper Function: Gets all words in Trie from a Node and returns them as a vector
vector<string> getAllWordsInTrie(Node* root, string& startOfSentence, string str, int level, vector<string>& wordList)
{
	if (root->isLeaf && level != 0) wordList.push_back(startOfSentence+str); // Base Case (no return because words can still exist!)

	for (int i = 0; i < 26; i++) // Iterate over children 
	{
		if (root->children[i] != NULL)
		{
			str += (char)('a' + i); // Add letter to string
			getAllWordsInTrie(root->children[i], startOfSentence, str, level+1, wordList); // Recur
			str.pop_back(); // Remove letter from string (now try new combination)
		}
	}
	return wordList; 
}

// Returns Node where startOfSentence prefix ends  
Node* getNodeOfStrInTrie(Node* root, string startOfSentence)
{
	Node* curr = root;
	for (int i = 0; i < startOfSentence.size(); i++) 
	{
		int pos = (int)startOfSentence[i] - 'a';
		curr = curr->children[pos]; 
	}
	return curr; // Points to a Node at the end of the startOfSentence prefix
}

// Optional Function#9
vector<string> getAutocompleteSentence(string startOfSentence)
{
	Node* root = createTrie(); // Create Trie
	vector<string> wordList;
	Node* curr = getNodeOfStrInTrie(root, startOfSentence); // Skip startOfSentence prefix in Trie
	getAllWordsInTrie(curr, startOfSentence, "", 0, wordList); // Get all autocomplete words 
	return wordList; 
}

int main()
{
	// 0. Parse book into a list of chapters
	string bookName = "1260.txt";
	parseBookIntoChapters(bookName);
	cout << "Processed book sucessfully!" << endl;

	// 1. Get total number of words:
	cout << "Total number of words in book " << bookName << " is: " << getTotalNumerOfWords() << endl;

	// 2. Get total number of unique words:
	cout << "Total number of unique words in book " << bookName << " is: " << getTotalUniqueWords() << endl;

	// 3. Get 20 most frequent words:
	vector<string> mostFrequentWords = get20MostFrequentWords();
	cout << "20 most frequent words in book " << bookName << " are: " << endl;
	for (auto word : mostFrequentWords)
	{
		cout << word << endl;
	}
	cout << endl;

	// 4. Get 20 most interesting frequent words
	vector<string> interestingWords = get20MostInterestingFrequentWords("1000Words.txt", 500); // Same answer!
	cout << "20 most interesting words in book " << bookName << " are: " << endl;
	for (auto word : interestingWords)
	{
		cout << word << endl;
	}
	cout << endl;

	// 5. Get 20 least frequent words:
	vector<string> leastFrequentWords = get20LeastFrequentWords();
	cout << "20 least frequent words in book " << bookName << " are: " << endl;
	for (auto word : leastFrequentWords)
	{
		cout << word << endl;
	}
	cout << endl;

	// 6. Get frequency list of word
	string wordToSearch = "Rochester";
	cout << "The word frequency list for word '" << wordToSearch << "' is: " << endl;
	vector<int> wordFreqList = getFrequencyOfWord(wordToSearch);
	for (auto word : wordFreqList)
	{
		cout << word << " ";
	}
	cout << endl;

	// 7. Get chapter in which quote appears
	string quote = "Life appears to me too short to be spent in nursing animosity or registering wrongs.";
	int chapterNum = getChapterQuoteAppears(quote);
	cout << "The quote '" << quote << "' appears in Chapter#" << chapterNum << " of book " << bookName << endl << endl;

	// 8. Generate sentence 
	string sentence = generateSentence();
	cout << "Sentence generated in author style: " << endl << sentence << endl << endl;

	// 9. Get a list of all words beginning with a particular string (uses Trie data structure)
	string wordToAutoComplete = "Ja";
	cout << "Autocompleting word '" << wordToAutoComplete << "':" << endl;
	vector<string> wordList = getAutocompleteSentence(setLowercase(wordToAutoComplete));
	for (string word : wordList)
	{
		cout << word << endl;
	}
	cout << endl;

	return 0;
}
