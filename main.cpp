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
string guesses;

int main() {
	int numLetters;
	cout << "Enter the number of letters: ";
	cin >> numLetters;

	bool dictionaryResult = readInDictionary("./dictionary.txt", numLetters);
	if (!dictionaryResult) {
		cout << "Failed to read dictionary file" << endl;
		return 1;
	}
	guesses = "";

	string word;
	for (int i = 0; i < numLetters; i++) {
		word += '_';
	}
	cout << word << endl;

	string input = "q";
	while (word.find('_') != string::npos) {
		cout << "Suggested Guess: " << generateGuess(word) << endl;
		cout << "Did the guess succeed? (y/n) - 'q' to exit: " << endl;
		cin >> input;

		if (input == "q") {
			return 0;
		}
		else if (input == "y") {
			cout << "Enter updated word (use _ for unknown) - 'q' to exit: " << endl;
			cin >> input;

			while (input.length() != numLetters) {
				cout << "Incorrect number of letters, please try again" << endl;
				cin >> input;
			}

			word = input;
			for (unsigned int i = 0; i < word.length(); i++) {
				char letter = word.at(i);
				if (letter != '_' && guesses.find(letter) == string::npos) {
					guesses.append(string(1, letter));
				}
			}
		}
		else if (input == "n") {
			cout << "Enter the guessed letter: " << endl;
			cin >> input;

			guesses.append(input);
		}
		else {
			cout << "Not a valid response" << endl;
		}

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
	string regexPattern = word;
	string plugIn = guesses.length() == 0 ? "." : "[^" + guesses + "]";
	for (unsigned int i = 0; i < regexPattern.length(); i++) {
		if (regexPattern.at(i) == '_') {
			regexPattern.replace(i, 1, plugIn);
		}
	}
	regex pattern = regex(regexPattern, regex_constants::optimize);
	dictionary->remove_if([&pattern](string word) -> bool { return !regex_match(word, pattern); });

	// Count character occurences in words
	unordered_map<char, int> characterCount;
	characterCount.emplace('_', 0);
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
	while (guesses.find(suggestion->first) != string::npos) {
		suggestion++;
	}
	cout << suggestion->first << endl;
	for (auto itr = characterCount.begin(); itr != characterCount.end(); itr++) {
		// TODO: prevent suggesting the same character twice
		if (guesses.find(itr->first) == string::npos && itr->second > suggestion->second) {
			cout << guesses << " | " << itr->first << " | " << itr->second << " | " << suggestion->second << endl;
			cout << (guesses.find(itr->first) == string::npos) << endl;
			suggestion = itr;
		}
	}

	return suggestion->first;
}