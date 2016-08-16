#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
	registerValidation();

	int n = inf.readInt(1, 200000, "n");
	inf.readSpace();
	int k = inf.readInt(1, n, "k");
	inf.readSpace();
	int A = inf.readInt(1, 10000, "A");
	inf.readSpace();
	int B = inf.readInt(1, A - 1, "B");
	inf.readSpace();
	int q = inf.readInt(1, 200000, "q");
	inf.readEoln();

	for(int i = 0; i < q; i++) {
		int t = inf.readInt(1, 2, "t_i");
		inf.readSpace();
		if(t == 1) {
			int d = inf.readInt(1, n, "d_i");
			inf.readSpace();
			int a = inf.readInt(1, 10000, "a_i");
			inf.readEoln();
		} else {
			int d = inf.readInt(1, n - k + 1, "d_i");
			inf.readEoln();
		}
	}

	inf.readEof();
}
