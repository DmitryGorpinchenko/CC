#include <iostream>
#include <algorithm>

using namespace std;

void ApproxFreqWords(const string& dna, int k, int d, vector<string>* freq);
void AdjustNeighCounts(int pattern, int k, int d, vector<int>& counts, vector<bool>& visited, int fixed);
int PatternToNum(const string& pattern, int base, int k);
string NumToPattern(int num, int k);

int main() {
	ios_base::sync_with_stdio(false);
	string dna;
	int k, d;
	cin >> dna >> k >> d;
	vector<string> freq;
	ApproxFreqWords(dna, k, d, &freq);
	for(auto& w : freq) {
		cout << w << " ";
	} 
	cout << endl;
	return 0;
} 

void ApproxFreqWords(const string& dna, int k, int d, vector<string>* freq) {
	int N = 1 << (2*k);
	vector<int> counts(N);
	vector<bool> visited(N);
	for(int i = 0; i < dna.size() - k + 1; ++i) {
		fill(visited.begin(), visited.end(), false);
		AdjustNeighCounts(PatternToNum(dna, i, k), k, d, counts, visited, 0);
	}
	int max = *max_element(counts.begin(), counts.end());
	for(int i = 0; i < N; ++i) {
		if(counts[i] == max) {
			freq->push_back(NumToPattern(i, k));
		}
	}
}

void AdjustNeighCounts(int pattern, int k, int d, vector<int>& counts, vector<bool>& visited, int fixed) {
	++counts[pattern];
	visited[pattern] = true;
	if(d == 0) {
		return;	
	}
	for(int i = 0; i < k; ++i) {
		if(!((fixed >> i) & 1)) {
			int mask = (~(3 << (2*i))) & pattern;
			for(int j = 1; j <= 3; ++j) {
				int tmp = mask | ((((pattern >> (2*i)) + j) & 3) << (2*i));
				if(!visited[tmp]) {
					AdjustNeighCounts(tmp, k, d - 1, counts, visited, fixed | (1 << i));
				}
			}
		}
	}
}

int PatternToNum(const string& pattern, int base, int k) {
	int code = 0, digit;
	for(int i = base; i < base + k; ++i) {
		switch(pattern[i]) {
			case 'A': digit = 0; break;
			case 'C': digit = 1; break;
			case 'T': digit = 2; break;
			case 'G': digit = 3; break;
		}
		code = 4*code + digit;
	}
	return code;	
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
