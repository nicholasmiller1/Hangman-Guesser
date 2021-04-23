#include <iostream>
#include <fstream>
#include <string>
#include <forward_list>
#include <unordered_map>
#include <regex>
using namespace std;

// Function Forward Declarations
bool readInDictionary(string filename, int wordLength);
char generateGuess(string word);

// Global Variables
forward_list<string>* dictionary;

int main() {
	int numLetters;
	cout << "Enter the number of letters: ";
	cin >> numLetters;

	bool dictionaryResult = readInDictionary("./dictionary.txt", numLetters);
	if (!dictionaryResult) {
		cout << "Failed to read dictionary file" << endl;
		return 1;
	}

	string word;
	for (int i = 0; i < numLetters; i++) {
		word += '.';
	}
	cout << word << endl;

	string input = "q";
	while (word.find('.') != string::npos) {
		cout << "Suggested Guess: " << generateGuess(word) << endl;
		cout << "Enter updated word (use . for unknown) - 'q' to exit: " << endl;
		cin >> input;

		if (input == "q") {
			return 0;
		}

		while (input.length() != numLetters) {
			cout << "Incorrect number of letters, please try again" << endl;
			cin >> input;
		}

		word = input;
		cout << "----------------------------" << endl;
	}

	cout << "Congrats! You guessed the whole word!" << endl;

	delete dictionary;
	return 0;
}

bool readInDictionary(string filename, int wordLength) {
	ifstream file(filename);
	if (!file.is_open()) {
		return false;
	}

	dictionary = new forward_list<string>;
	string word;
	while (file.good()) {
		file >> word;
		if (word.length() == wordLength) {
			dictionary->push_front(word);
		}
	}

	file.close();

	return true;
}

char generateGuess(string word) {
	// Removes all words from the dictionary that don't match the current word
	// TODO: Remove words with too many of guessed letters
	regex pattern = regex(word, regex_constants::optimize);
	dictionary->remove_if([&pattern](string word) -> bool { return !regex_match(word, pattern); });

	// Count character occurences in words
	// TODO: prevent suggesting the same character twice
	unordered_map<char, int> characterCount;
	for (auto itr = dictionary->begin(); itr != dictionary->end(); itr++) {
		for (char c : *itr) {
			if (characterCount.count(c) == 0) {
				characterCount.emplace(c, 1);
			}
			else {
				characterCount.at(c)++;
			}
		}
	}

	// Choose the letter with the most occurences
	auto suggestion = characterCount.begin();
	for (auto itr = characterCount.begin(); itr != characterCount.end(); itr++) {
		if (itr->second > suggestion->second) {
			suggestion = itr;
		}
	}

	return suggestion->first;
}