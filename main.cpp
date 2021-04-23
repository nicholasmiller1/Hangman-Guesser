#include <iostream>
#include <fstream>
#include <string>
#include <forward_list>
using namespace std;

// Function Forward Declarations
bool readInDictionary(string filename, int wordLength);
char generateGuess();

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
		word += '_';
	}
	cout << word << endl;

	string input = "q";
	while (word.find('_') != string::npos) {
		cout << "Suggested Guess: " << generateGuess() << endl;
		cout << "Enter updated word (_ for unknown) - 'q' to exit: " << endl;
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

char generateGuess() {
	return 'a';
}