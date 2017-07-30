#include <iostream>
#include <algorithm>

using namespace std;

void ApproxFreqWords(const string& dna, int k, int d, vector<string>* freq);
void AdjustNeighCounts(int pattern, int k, int d, vector<int>& counts, vector<bool>& visited, int fixed);
void AdjustCountsForCompl(vector<int>& counts, int N, int k);
void FillFreqWords(const vector<int>& counts, int N, int k, vector<string>* freq);
int PatternToNum(const string& pattern, int base, int k);
string NumToPattern(int num, int k);
int min_skew(const string& dna);
void EColi();
void Salmonella();

int main() {
	ios_base::sync_with_stdio(false);
	//EColi();
	Salmonella();
	/*string dna;
	int k, d;
	cin >> dna >> k >> d;
	vector<string> freq;
	ApproxFreqWords(dna, k, d, &freq);
	for(auto& w : freq) {
		cout << w << " ";
	} 
	cout << endl;*/
	return 0;
} 

void EColi() {
	string dna;
	cin >> dna;
	int L = 500, k = 9, d = 1;
	string oriC = dna.substr(min_skew(dna), L);
	vector<string> DnaA_candidates;
	ApproxFreqWords(oriC, k, d, &DnaA_candidates);
	for(auto& DnaA : DnaA_candidates) {
		cout << DnaA << " ";
	} 
	cout << endl;
}

void Salmonella() {
	string dna;
	string line;
	while(cin >> line) {
		dna += line;
	}
	int L = 500, k = 9, d = 1;
	string oriC = dna.substr(min_skew(dna) - L , 2*L);
	vector<string> DnaA_candidates;
	ApproxFreqWords(oriC, k, d, &DnaA_candidates);
	for(auto& DnaA : DnaA_candidates) {
		cout << DnaA << " ";
	} 
	cout << endl;
}

void ApproxFreqWords(const string& dna, int k, int d, vector<string>* freq) {
	int N = 1 << (2*k);
	vector<int> counts(N);
	vector<bool> visited(N);
	for(int i = 0; i < dna.size() - k + 1; ++i) {
		fill(visited.begin(), visited.end(), false);
		AdjustNeighCounts(PatternToNum(dna, i, k), k, d, counts, visited, 0);
	}
	AdjustCountsForCompl(counts, N, k);
	FillFreqWords(counts, N, k, freq);
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

void AdjustCountsForCompl(vector<int>& counts, int N, int k) {
	vector<bool> considered(N);
	for(int i = 0; i < N; ++i) {
		if(!considered[i]) {
			int rev_compl = 0, templ = i;
			for(int j = 0; j < k; ++j) {
				rev_compl <<= 2;
				//'% 4' to isolate single nucleotide, '+ 2' to complement
				rev_compl += (templ + 2) % 4;
				templ >>= 2;
			}
			counts[i] += counts[rev_compl];
			counts[rev_compl] = counts[i];
			considered[i] = considered[rev_compl] = true;
		}
	}
}

void FillFreqWords(const vector<int>& counts, int N, int k, vector<string>* freq) {
	int max = *max_element(counts.begin(), counts.end());
	for(int i = 0; i < N; ++i) {
		if(counts[i] == max) {
			freq->push_back(NumToPattern(i, k));
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

int min_skew(const string& dna) {
	int N = dna.size() + 1;
	vector<int> skews(N);
	for(int i = 1; i < N; ++i) {
		if(dna[i - 1] == 'C') {
			skews[i] = skews[i - 1] - 1;
		} else if(dna[i - 1] == 'G') {
			skews[i] = skews[i - 1] + 1;
		} else {
			skews[i] = skews[i - 1];
		}
	}
	int oriC = 0, min = skews[0];
	for(int i = 1; i < N; ++i) {
		if(skews[i] < min) {
			min = skews[i];
			oriC = i;
		}
	}
	return oriC;
}
