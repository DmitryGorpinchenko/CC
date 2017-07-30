#include <iostream>

using namespace std;

string complement(const string& dna_strand);
void test();

int main() {
	ios::sync_with_stdio(false);
	//test();
	string dna_strand;
	cin >> dna_strand;
	cout << complement(dna_strand) << endl;
	return 0;
}

string complement(const string& dna_strand) {
	char compl_nucl;
	string compl_strand;
	for(int i = dna_strand.size() - 1; i >= 0; --i) {
		switch(dna_strand[i]) {
			case 'A': compl_nucl = 'T'; break;
			case 'T': compl_nucl = 'A'; break;
			case 'C': compl_nucl = 'G'; break;
			case 'G': compl_nucl = 'C'; break;
		}
		compl_strand.push_back(compl_nucl);
	}
	return compl_strand;
}

void test() {
	string dna_strand;
	cin >> dna_strand;
	string correct_compl;
	cin >> correct_compl;
	if(correct_compl == complement(dna_strand)) {
		cout << "PASSED!!!" << endl;
	} else {
		cout << "FAILED!!!" << endl;
	}
}
