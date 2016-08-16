#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	int A = atoi(argv[3]);
	int B = atoi(argv[4]);
	int q = atoi(argv[5]);

	cout << n << ' ' << k << ' ' << A << ' ' << B << ' ' << q << endl;

	for(int i = 0; i < q; i++) {
		int t = rnd.next(1, 2);
		if(t == 1) {
			int d = rnd.next(1, n);
			int a = rnd.next(1, A);
			cout << t << ' ' << d << ' ' << a << endl;
		} else {
			int d = rnd.next(1, n - k + 1);
			cout << t << ' ' << d << endl;
		}
	}
}
