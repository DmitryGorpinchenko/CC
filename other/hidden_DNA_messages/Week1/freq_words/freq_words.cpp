#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

void FindFreqPatterns(const string& dna, int k, vector<string>* freq);

int main() {
	ios::sync_with_stdio(false);
	string dna;
	int k;
	cin >> dna >> k;
	vector<string> freq;
	FindFreqPatterns(dna, k, &freq);
	for(auto& pattern : freq) {
		cout << pattern << " ";
	}
	cout << endl;
	return 0;
}

void FindFreqPatterns(const string& dna, int k, vector<string>* freq) {
	unordered_map<string, int> counts;
	for(int i = 0; i < dna.size() - k + 1; ++i) {
		counts[dna.substr(i, k)] += 1;
	}
	int max_count = 0;
	for(auto& kv : counts) {
		max_count = kv.second > max_count ? kv.second : max_count;
	}
	for(auto& kv : counts) {
		if(kv.second == max_count) {
			freq->push_back(kv.first);
		}
	}
}
