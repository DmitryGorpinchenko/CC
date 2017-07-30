#include <iostream>

using namespace std;

int hamming(const string& s1, const string& s2);

int main() {
	ios_base::sync_with_stdio(false);
	string s1, s2;
	cin >> s1 >> s2;
	cout << hamming(s1, s2) << endl;
	return 0;
}

int hamming(const string& s1, const string& s2) {
	int dist = 0;
	for(int i = 0; i < s1.size(); ++i) {
		if(s1[i] != s2[i]) {
			++dist;
		}
	}
	return dist;
}
