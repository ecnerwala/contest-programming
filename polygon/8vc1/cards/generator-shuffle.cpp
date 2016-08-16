#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	vector<int> vals;
	for(int v = 1; v <= 3; v++) {
		for(int i = 0; i < atoi(argv[v]); i++) {
			vals.push_back(v);
		}
	}
	shuffle(vals.begin(), vals.end());
	cout << vals.size() << endl;
	for(int i = 0; i < int(vals.size()); i++) {
		if(i) cout << ' ';
		cout << vals[i];
	}
	cout << endl;
}
