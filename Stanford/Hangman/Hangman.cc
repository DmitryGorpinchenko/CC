#include "Hangman.h"

#include <iomanip>
#include <map>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>

const string dict = "dictionary.txt";
//const string dict = "dict";

int main()
{	
	ifstream in(dict.c_str());
	Hangman game(in);	
		
	game.play();
	
	return 0;
}

Hangman::Hangman(ifstream& in)
{	
	copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(wordList));
	
	set<int> lengths;
	for (auto itr = wordList.begin(); itr != wordList.end(); ++itr) {
		lengths.insert(itr->length());
	}
	
	setWordLength(lengths);
	setGuessNum();

	currWord.resize(len, '-');
	
	wordList.erase(remove_if(wordList.begin(), wordList.end(), [&](string s)->bool { return s.length() != len; }), wordList.end());	
}

void Hangman::play()
{	
	cout << setw(30) << "Current word" << setw(20) << "Remain guesses" << setw(20) << "Remain words" << endl;
	
	while (guessNum > 0 && trials.size() < 26) {
		displayGameState();
		setCurrGuess();
		processGuess();
		if (win()) {
			cout << endl << "You win the game!" << endl;
			cout << "The word I thought of is indeed: [" << currWord << "]" << endl;
			return;
		}
	}
	
	cout << endl << "You loose the game!" << endl;
	cout << "The word I thought of was: [" << wordList[0] << "]" << endl;
}

bool Hangman::win() const
{	
	return currWord.find('-') == string::npos;
}

void Hangman::processGuess()
{	
	map<string, vector<string> > families;
	for (auto itr = wordList.begin(); itr != wordList.end(); ++itr) {
		families[family(*itr)].push_back(*itr);
	}
	
	auto itr = max_element(families.begin(), families.end(), compareByValue);
	
	if (currWord == itr->first) {
		guessNum--;
	}
	
	wordList = itr->second;
	currWord = itr->first;
}

string Hangman::family(const string& word) const
{	
	string family = currWord;
	
	for (int i = 0; i < len; i++) {
		if (word[i] == currGuess) {
			family[i] = currGuess;
		}
	}
	
	return family;
}

void Hangman::setCurrGuess()
{	
	cout << "Please, enter you next guess: ";
	
	while (true) {
		string line;
		getline(cin, line);
		if (line.length() == 1 && islower(line[0]) && !trials.count(line[0])) {
			currGuess = line[0];
			trials.insert(line[0]);
			break;
		}
		cout << "Retry: ";
	}
}

void Hangman::setWordLength(const set<int>& lengths)
{	
	cout << endl << "Please, enter word length: ";

	while (true) {
		string line;
		getline(cin, line);
		stringstream s;
		s << line;
		if (s >> len && lengths.count(len)) {
			break;
		}		
		cout << "Retry: ";
	}	
}

void Hangman::setGuessNum()
{	
	cout << endl << "Please, enter a number of guesses: ";

	while (true) {
		string line;
		getline(cin, line);
		stringstream s;
		s << line;
		if (s >> guessNum) {
			break;
		}		
		cout << "Retry: ";
	}	
}

void Hangman::displayGameState() const
{
	cout << setw(30) << currWord; 
	cout << setw(20) << guessNum;
	cout << setw(20) << wordList.size();
	cout << endl;
}

bool Hangman::compareByValue(const pair<string, vector<string> >& p1, const pair<string, vector<string> >& p2)
{
	return p1.second.size() < p2.second.size();
}
