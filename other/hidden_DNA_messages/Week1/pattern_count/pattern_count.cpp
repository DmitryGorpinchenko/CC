#include <iostream>

using namespace std;

class Matcher {
public:
	explicit Matcher(const string& pattern);
	int count(const string& text) const;
	
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

int Matcher::count(const string& text) const {
	int count = 0;
	int P = pattern_.size();
	int T = text.size();
	long long curr_hash = PolyHash(text.substr(T - P, P));
	int i = T - P;
	while(1) {
		if(p_hash_ == curr_hash && pattern_ == text.substr(i, P)) {
			++count;
		}
		if(--i < 0) {
			break;
		}
		curr_hash = (x_ * curr_hash + text[i] - x_P_ * text[i + P]) % p_;	
		curr_hash = curr_hash < 0 ? curr_hash + p_ : curr_hash;
	}
	return count;
}

long long Matcher::PolyHash(const string& s) const {
	long long code = 0;
	for(int i = static_cast<int>(s.size() - 1); i >= 0; --i) {
		code = (code * x_ + s[i]) % p_;
	}
	return code;
}

int main() {
	ios::sync_with_stdio(false);
	string dna;
	string pattern;
	cin >> dna >> pattern;
	Matcher matcher(pattern);	
	cout << matcher.count(dna) << endl;
	return 0;
}
