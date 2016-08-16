#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	int n = atoi(argv[1]);
	int k = atoi(argv[2]);

	int center = rnd.next(1, n);

	vector<int> vals;
	for(int i = 0; i < n; i++) {
		if(i < k) {
			vals.push_back(rnd.next(500001, 1000000));
		} else {
			vals.push_back(rnd.next(1, 500000));
		}
	}
	shuffle(vals.begin(), vals.end());

	cout << n << ' ' << k << endl;
	for(int i = 1; i <= n; i++) {
		cout << vals[i - 1];
		if(i == n) cout << endl;
		else cout << ' ';
	}
	for(int i = 1; i <= n; i++) {
		if(i == center) continue;
		cout << center << ' ' << i << endl;
	}
}
