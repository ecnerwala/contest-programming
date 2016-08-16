#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

	vector<int> x;
	for(int i = 1; i + 1 < argc; i+=2) {
		int v = atoi(argv[i]);
		int c = atoi(argv[i + 1]);
		for(int j = 0; j < c; j++) x.push_back(v++);
	}

	cout << int(x.size()) << '\n';
	for(int i = 0; i < int(x.size()); i++) {
		if(i) cout << ' ';
		cout << x[i];
	}
	cout << '\n';
}
