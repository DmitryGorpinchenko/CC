#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream> 

using namespace std;

int FindClumps(const string& dna, int k, int L, int t, vector<string>* clumps);
/*
 * avoids generating substrings by using base index
 */
int PatternToNum(const string& pattern, int base, int k);
int NucleotideToNum(char c); 
string NumToPattern(int num, int k);
void EColi();
void test();

int main() {
	ios::sync_with_stdio(false);
	EColi();
	//test();
	/*string dna;
	int k, L, t;
	cin >> dna >> k >> L >> t;
	vector<string> clumps;
	FindClumps(dna, k, L, t, &clumps);
	for(auto& pattern : clumps) {
		cout << pattern << " ";
	}
	cout << endl;*/
	return 0;
}

void EColi() {
	int k = 9, L = 500, t = 3;
	string dna;
	cin >> dna;
	vector<string> clumps;
	FindClumps(dna, k, L, t, &clumps);
	cout << clumps.size() << endl;
}

/*
 * seems that when 4^k is comparable with dna.size() or when most of
 * k-mer combinations exist in the DNA string (array would not be sparse), 
 * direct addressing scheme is more efficient than using hashtable
 */
int FindClumps(const string& dna, int k, int L, int t, vector<string>* clumps) {
	int len = 2 << (2*k);
	//parenths are needed for zero-initialization
	int *counts = new int[len](); 
	bool *is_clump = new bool[len]();
	//shift by 32 positions has undefined behavior, so, explixitly check for this
	int mask = k > 1 ? (~0u) >> (32 - (2*k - 2)) : 0;
	int i = 0, digit, tmp = PatternToNum(dna, i, k), tmp1;
	while(true) {
		//NOTE: pre-incrementation is essential here
		if(++counts[tmp] >= t) {
			is_clump[tmp] = true;
		}
		if(i >= L - k) {
			break;
		}
		tmp = ((tmp & mask) << 2) | NucleotideToNum(dna[i + k]);
		++i;
	}
	i = 1;
	tmp1 = PatternToNum(dna, i - 1, k);
	tmp = PatternToNum(dna, i + L - k, k);
	while(true) {
		--counts[tmp1];
		//NOTE: pre-incrementation is essential here
		if(++counts[tmp] >= t) {
			is_clump[tmp] = true;
		}
		if(i >= dna.size() - L) {
			break;
		}
		tmp1 = ((tmp1 & mask) << 2) | NucleotideToNum(dna[i - 1 + k]);
		tmp = ((tmp & mask) << 2) | NucleotideToNum(dna[i + L]);
		++i;
	}
	//push actual 'clumps' at the end to avoid duplicates
	for(int i = 0; i < len; ++i) {
		if(is_clump[i]) {
			clumps->push_back(NumToPattern(i, k));
		}
	}
	delete [] counts;
	delete [] is_clump;
}

int PatternToNum(const string& pattern, int base, int k) {
	int code = 0;
	for(int i = base; i < base + k; ++i) {
		code = 4*code + NucleotideToNum(pattern[i]);
	}
	return code;	
}

int NucleotideToNum(char c) {
	int digit;
	switch(c) {
		case 'A': digit = 0; break;
		case 'C': digit = 1; break;
		case 'T': digit = 2; break;
		case 'G': digit = 3; break;
	}
	return digit; 
}

string NumToPattern(int num, int k) {
	string pattern(k, '\0');
	char nucleotide;
	for(int i = k - 1; i >= 0; --i) {
		switch(num % 4) {
			case 0: nucleotide = 'A'; break;
			case 1: nucleotide = 'C'; break;
			case 2: nucleotide = 'T'; break;
			case 3: nucleotide = 'G'; break;
		}
		num >>= 2;
		pattern[i] = nucleotide;
	}
	return pattern;
}

/*
 * invoke in main and run in the terminal the following command: 
 * cat data/test* | ./a.out
 */
void test() {
	int k, L, t;
	string dna, tmp, tmp1, tmp2;
	while(getline(cin, dna)) {
		getline(cin, tmp);
		stringstream(tmp) >> k >> L >> t;
		vector<string> clumps, correct_clumps;
		FindClumps(dna, k, L, t, &clumps);
		getline(cin, tmp1);
		stringstream in(tmp1);
		while(in >> tmp2) {
			correct_clumps.push_back(tmp2);
		}
		sort(correct_clumps.begin(), correct_clumps.end());
		sort(clumps.begin(), clumps.end());
		if(clumps != correct_clumps) {
			cout << "FAILED!!!" << endl;
			return;
		}
	}
	cout << "PASSED!!!" << endl;
}
