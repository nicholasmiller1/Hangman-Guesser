#include <iostream>
#include <string>
using namespace std;

void printWord(string word, int letters);
char generateGuess();

int main() {
	int numLetters;
	cout << "Enter the number of letters: ";
	cin >> numLetters;

	string word;
	for (int i = 0; i < numLetters; i++) {
		word += '_';
	}
	printWord(word, numLetters);

	while (word.find('_') != string::npos) {
		cout << "Suggested Guess: " << generateGuess() << endl;
		cout << "Enter responses (letter position) - 'q' to exit: " << endl;

		char letter = 'q';
		int index = 0;
		while (true) {
			cin >> letter;
			if (letter == 'q') {
				break;
			}
			cin >> index;
			cout << letter << " " << index << endl;

			if (letter >= '_' && letter <= '~' && isdigit(index) && index >= 0 && index < numLetters) {
				cout << index << " " << (int)'q' << endl;
				word.at(index) = letter;
				printWord(word, numLetters);
			}
			else {
				cout << "Invalid input, please try again";
			}
		}

	}

	return 0;
}

void printWord(string word, int letters) {
	for (int i = 0; i < letters; i++) {
		cout << word[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < letters; i++) {
		cout << i << " ";
	}
}

char generateGuess() {
	return 'a';
}