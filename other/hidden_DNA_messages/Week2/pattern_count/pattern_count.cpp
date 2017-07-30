#include <iostream>

using namespace std;

int count(const string& p, const string& dna, int d);
int hamming(const string& p, const string& s, int base);

int main() {
	ios_base::sync_with_stdio(false);
	string p, dna;
	int d;
	cin >> p >> dna >> d;
	cout << count(p, dna, d) << endl;
	return 0;
}

int count(const string& p, const string& dna, int d) {
	int count = 0;
	for(int i = 0; i < dna.size() - p.size() + 1; ++i) {
		if(hamming(p, dna, i) <= d) {
			++count;
		}
	}
	return count;
}

int hamming(const string& p, const string& s, int base) {
	int dist = 0;
	for(int i = 0; i < p.size(); ++i) {
		dist += (p[i] != s[i + base]);
	}
	return dist;
}
