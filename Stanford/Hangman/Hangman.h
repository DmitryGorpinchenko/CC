#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

class Hangman {
public:
    Hangman(ifstream& in);
    void play();
    bool win() const;
    void displayGameState() const;
    
private:
    void processGuess();
    void setWordLength(const set<int>& lengths);
    void setGuessNum();
    void setCurrGuess();
    string family(const string& word) const;
    static bool compareByValue(const pair<string, vector<string> >& p1, const pair<string, vector<string> >& p2);

    vector<string> wordList;
    int guessNum;
    int len;
    string currWord;
    char currGuess;
    set<char> trials;
};
