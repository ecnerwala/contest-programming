#include "testlib.h"
#include <iostream>
#include <unordered_set>

using namespace std;

const int MAXV = 1e9;

int main(int argc, char* argv[]){

	registerGen(argc, argv, 1);

	int N = atoi(argv[1]);
	unordered_set<int> vals;

	cout << N << endl;
	for(int i = 0; i < N; i++) {
		int v;
		do v = rnd.next(0, MAXV); while(vals.count(v));

		vals.insert(v);

		if(i) cout << ' ';
		cout << v;
	}
	cout << endl;

	return 0;
}
