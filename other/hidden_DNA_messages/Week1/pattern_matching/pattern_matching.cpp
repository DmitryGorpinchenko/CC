#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Matcher {
public:
	explicit Matcher(const string& pattern);
	void find(const string& text, vector<int>* matches) const;
	
private:
	long long PolyHash(const string& s) const;
	
	string pattern_;
	int p_;
	int x_;
	long long p_hash_;
	long long x_P_;
};

Matcher::Matcher(const string& pattern) : pattern_(pattern), p_(1000000007), x_(263) {
	x_P_ = 1;
	for(int i = 0; i < pattern.size(); ++i) {
		x_P_ = (x_P_ * x_) % p_;
	}
	p_hash_ = PolyHash(pattern);
}

void Matcher::find(const string& text, vector<int>* matches) const {
	int P = pattern_.size();
	int T = text.size();
	long long curr_hash = PolyHash(text.substr(T - P, P));
	int i = T - P;
	while(1) {
		if(p_hash_ == curr_hash && pattern_ == text.substr(i, P)) {
			matches->push_back(i);
		}
		if(--i < 0) {
			break;
		}
		curr_hash = (x_ * curr_hash + text[i] - x_P_ * text[i + P]) % p_;	
		curr_hash = curr_hash < 0 ? curr_hash + p_ : curr_hash;
	}
	reverse(matches->begin(), matches->end());
}

long long Matcher::PolyHash(const string& s) const {
	long long code = 0;
	for(int i = static_cast<int>(s.size() - 1); i >= 0; --i) {
		code = (code * x_ + s[i]) % p_;
	}
	return code;
}

void test();
void Vibrio();

int main() {
	ios::sync_with_stdio(false);
	//test();
	Vibrio();
	/*string dna;
	string pattern;
	cin >> pattern >> dna;
	Matcher matcher(pattern);	
	vector<int> matches;
	matcher.find(dna, &matches);
	for(auto& pos : matches) {
		cout << pos << " ";
	}
	cout << endl;*/
	return 0;
}

void Vibrio() {
	string pattern = "CTTGATCAT";
	string dna;
	cin >> dna;
	Matcher matcher(pattern);	
	vector<int> matches;
	matcher.find(dna, &matches);
	for(auto& pos : matches) {
		cout << pos << " ";
	}
	cout << endl;
}

void test() {
	string text;
	string pattern;
	cin >> pattern >> text;
	Matcher matcher(pattern);	
	vector<int> matches;
	matcher.find(text, &matches);
	int curr;
	for(int i = 0; i < matches.size(); ++i) {
		cin >> curr;
		if(curr != matches[i]) {
			cout << "FAILED!!!" << endl;
			return;
		}
	}
	cout << "PASSED!!!" << endl;
}
