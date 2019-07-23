#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <map>
//#include "BookAnalysis.h"
using namespace std;

bool punctuationArr[127];
vector<string> chapterList;
int totalNumberOfWords; 
map<string, int> wordFrequencyMap; 
multimap<int, string, greater<int>> reverseWordFrequencyMapLarge;
multimap<int, string> reverseWordFrequencyMapSmall; 
vector<string> mostCommonWordsList;

void populatePunctuationArr()
{
	memset(punctuationArr, 0, 127 * sizeof(bool)); // Initialize punctuationArr to 0
	
	string punctuationChars = " \n\r\t!\"'#$%^&*(){}[]<>+-=_/,.:;~`?|"; // Array of punctuation characters to omit
	for (int i = 0; i < punctuationChars.size(); i++)
	{
		int asciiIdx = (int) punctuationChars.at(i);
		punctuationArr[asciiIdx] = 1;
	}
	punctuationArr[0] = true; // Mark NULL 
}

string stripLeadingPunctuation(string word)
{
	if (word == "") return word;
	int idx = 0;

	while (idx < word.size() && punctuationArr[(int)word[idx]] == 1)
	{
		idx++;
	}
	return word.substr(idx, word.size() - idx + 1); 
}

string stripTrailingPunctuation(string word)
{
	if (word == "") return word;
	int idx = word.size() - 1;

	while (idx < word.size() && punctuationArr[(int)word[idx]] == 1)
	{
		idx--;
	}
	return word.substr(0, idx+1);
}

void parseBookIntoChapters(string bookName)
{
	ifstream myFile (bookName);
	string line = "";

	populatePunctuationArr();

	string chapterText = "";
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			stringstream ss(line);
			while (ss)
			{
				string word = "";
				ss >> word;
				totalNumberOfWords++; 

				if (word == "CHAPTER")
				{
					if(chapterText != "") chapterList.push_back(chapterText);
					chapterText = "";
				}
				else
				{
					chapterText += (word + " "); // FIXME: Put word w/or w/o punctuation in chapterList? 
					word = stripLeadingPunctuation(word);
					word = stripTrailingPunctuation(word);
					if(word != "") wordFrequencyMap[word]++; 
				}
			}
			chapterText.pop_back();
		}
		chapterList.push_back(chapterText);

		/*for (string chapter : chapterList)
		{
			cout << "New Chapter: " << endl;
			cout << chapter << endl;
		}*/
	}
	else
	{
		cout << "Error opening file!" << endl;
	}
}

int getTotalNumerOfWords()
{
	return totalNumberOfWords;
}

int getTotalUniqueWords()
{
	return wordFrequencyMap.size(); 
}

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

vector<string> parseMostCommonWords(string fileName)
{
	ifstream myFile(fileName);
	string line = "";

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			stringstream ss(line);
			while (ss)
			{
				string word = "";
				ss >> word;
				if(word != "") mostCommonWordsList.push_back(word);
			}
		}
	}
	else
	{
		cout << "Error opening file!" << endl;
	}
	return mostCommonWordsList;
}

vector<string> get20MostInterestingFrequentWords(string fileName, int filterSize)
{
	if(mostCommonWordsList.size() == 0) parseMostCommonWords(fileName);
	vector<string> filteredWordList(mostCommonWordsList.begin(), mostCommonWordsList.begin() + filterSize);

	vector <string> interestingWordList;
	//vector<int> tmp;

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
			//tmp.push_back(count);
			ctr++; 
		}
	}
	return interestingWordList;
}

vector<string> get20LeastFrequentWords()
{
	vector<string> wordList;
	//vector<int> tmp;
	if (reverseWordFrequencyMapSmall.size() == 0) populateReverseWordFrequencyMaps();
	int ctr = 0;
	for (auto it : reverseWordFrequencyMapSmall)
	{
		if (ctr == 20) break;
		wordList.push_back(it.second);
		//tmp.push_back(it.first);
		ctr++;
	}
	return wordList;
}

vector<int> getFrequencyOfWord(string word)
{
	vector<int> wordFrequencyList;
	for (string chapter : chapterList)
	{
		if (chapter != "")
		{
			int wordCtr = 0;
			int currPos = 0;
			while (currPos != std::string::npos)
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

int getChapterQuoteAppears(string quote)
{
	int chapterCtr = 1;
	for (string chapter : chapterList)
	{
		int currPos = chapter.find(quote);
		if (currPos != -1)
		{
			return chapterCtr;
		}
		chapterCtr++; 
	}
	return -1; 
}

int min(int x, int y) { return (x < y) ? x : y; }
int max(int x, int y) { return (x > y) ? x : y; }
bool isLowercase(string word)
{
	for (int i = 0; i < word.size(); i++) if (isupper(word[i]) && i != 0) return false;
	return true;
}

vector<string> getWordAfter(string currWord)
{
	vector<string> nextWordList;
	for (string chapter : chapterList)
	{
		int currPos = chapter.find(currWord);
		while (currPos != string::npos)
		{
			if (currPos - 1 >= 0 && punctuationArr[(int)chapter[currPos - 1]] == 1 && currPos + currWord.size() < chapter.size() && punctuationArr[(int)chapter[currPos + currWord.size()]] == 1)
			{
				int nextWordStartPos = (chapter.find(" ", currPos + 1));
				int nextWordEndPos = min(chapter.find(" ", nextWordStartPos+1), chapter.find("--", nextWordStartPos + 1));
				string nextWord = chapter.substr(nextWordStartPos+1, nextWordEndPos - nextWordStartPos); 
				nextWord = stripLeadingPunctuation(nextWord);
				nextWord = stripTrailingPunctuation(nextWord);
				if(nextWord != "") nextWordList.push_back(nextWord);
			}
			currPos += currWord.size();
			currPos = chapter.find(currWord, currPos);

		}
	}
	return nextWordList; 
}

string generateSentence()
{
	srand(time(NULL));
	int ctr = 0;
	string sentence = "";
	string currWord = "The"; 
	while (ctr < 19)
	{
		vector<string> wordList = getWordAfter(currWord); 
		if (wordList.size() != 0)
		{
			sentence += (currWord + " "); 
			if (wordList.size() == 1) currWord = wordList[0];
			else
			{
				while (true)
				{
					int randNum = rand() % (wordList.size());
					currWord = wordList[randNum]; 
					if (currWord.size() == 1 || isLowercase(currWord)) break;
				}
			}
			ctr++; 
		}
	}
	sentence.pop_back();
	sentence += ".";
	return sentence;
}

int main()
{
	// 0. Parse book into a list of chapters
	string bookName = "1260.txt";
	parseBookIntoChapters(bookName);
	//cout << "Processed book sucessfully!" << endl;

	// 1. Get total number of words:
	//cout << "Total number of words in book " << bookName << " is: " << getTotalNumerOfWords() << endl;

	// 2. Get total number of unique words:
	//cout << "Total number of unique words in book " << bookName << " is: " << getTotalUniqueWords() << endl;

	// 3. Get 20 most frequent words:
	//vector<string> mostFrequentWords = get20MostFrequentWords();
	//cout << "20 most frequent words in book " << bookName << " are: " << endl;
	//for (auto word : mostFrequentWords)
	//{
		//cout << word << endl;
	//}
	//cout << endl;

	// 5. Get 20 least frequent words:
	//vector<string> leastFrequentWords = get20LeastFrequentWords();
	//cout << "20 least frequent words in book " << bookName << " are: " << endl;
	//for (auto word : leastFrequentWords)
	//{
		//cout << word << endl;
	//}
	//cout << endl;

	// 4.
	//vector<string> interestingWords = get20MostInterestingFrequentWords("1000Words.txt", 500); // Same answer!
	//cout << "20 most interesting words in book " << bookName << " are: " << endl;
	//for (auto word : interestingWords)
	//{
	//	cout << word << endl;
	//}
	//cout << endl;

	//// 6. 
	/*string wordToSearch = "Rochester";
	cout << "The word frequency list for word '" << wordToSearch << "' is: " << endl;
	vector<int> wordFreqList = getFrequencyOfWord(wordToSearch);
	for (auto word : wordFreqList)
	{
		cout << word << " ";
	}
	cout << endl;
	cout << "Total Number of chapters: " << chapterList.size() << endl;*/

	//7.
	//string quote = "hello world how";
	//string quote = "Life appears to me too short to be spent in nursing animosity or registering wrongs.";
	//int chapterNum = getChapterQuoteAppears(quote);
	//cout << "The quote '" << quote << "' appears in Chapter#" << chapterNum << " of book " << bookName << endl;

	// 8.
	for (int i = 0; i < 100; i++)
	{
		string sentence = generateSentence();
		cout << sentence << endl;
		//cout << "Sentence generated in author style: " << endl << sentence << endl;

	}

	return 0;
}
