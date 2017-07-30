#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

void min_skew(const string& dna, vector<int>* mins);
void test();

int main() {
	ios_base::sync_with_stdio(false);
	//test();
	string dna;
	cin >> dna;
	vector<int> min_skews;
	min_skew(dna, &min_skews);
	for(auto pos : min_skews) {
		cout << pos << " ";
	}
	cout << endl;
	return 0;
}

void min_skew(const string& dna, vector<int>* mins) {
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
	int min_skew = *min_element(skews.begin(), skews.end());
	for(int i = 0; i < N; i++) {
		if(skews[i] == min_skew) {
			mins->push_back(i);
		}
	}
}

void test() {
	string dna, tmp;
	int pos;
	while(getline(cin, dna)) {
		vector<int> min_skews, correct_min_skews;
		min_skew(dna, &min_skews);
		getline(cin, tmp);
		stringstream in(tmp);
		while(in >> pos) {
			correct_min_skews.push_back(pos);
		}
		sort(correct_min_skews.begin(), correct_min_skews.end());
		sort(min_skews.begin(), min_skews.end());
		if(correct_min_skews != min_skews) {
			cout << "FAILED!!!" << endl;
			return;
		}
	}
	cout << "PASSED!!!" << endl;
}
