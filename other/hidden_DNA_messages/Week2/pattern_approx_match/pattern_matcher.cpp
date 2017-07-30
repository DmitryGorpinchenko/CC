#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

void match(const string& p, const string& dna, int d, vector<int>* pos);
int hamming(const string& p, const string& s, int base);
void test();

int main() {
	ios_base::sync_with_stdio(false);
	//test();
	string p, dna;
	int d;
	cin >> p >> dna >> d;
	vector<int> positions;
	match(p, dna, d, &positions);
	for(auto pos : positions) {
		cout << pos << " ";
	}
	cout << endl;
	return 0;
}

void match(const string& p, const string& dna, int d, vector<int>* pos) {
	for(int i = 0; i < dna.size() - p.size() + 1; ++i) {
		if(hamming(p, dna, i) <= d) {
			pos->push_back(i);
		}
	}
}

int hamming(const string& p, const string& s, int base) {
	int dist = 0;
	for(int i = 0; i < p.size(); ++i) {
		dist += (p[i] != s[i + base]);
	}
	return dist;
}

void test() {
	int d, pos;
	string p, dna, tmp;
	while(getline(cin, p)) {
		getline(cin, dna);
		getline(cin, tmp);
		stringstream in(tmp);
		in >> d;
		getline(cin, tmp);
		stringstream in_(tmp);
		vector<int> correct;
		while(in_ >> pos) {
			correct.push_back(pos);
		}
		vector<int> positions;
		match(p, dna, d, &positions);
		if(positions != correct) {
			cout << "FAILED!!!" << endl;
			return;
		}
	}
	cout << "PASSED!!!" << endl;
}	
