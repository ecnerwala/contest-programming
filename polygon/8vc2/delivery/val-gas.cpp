#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
	registerValidation();

	int d = inf.readInt(1, int(1e9), "d");
	inf.readSpace();
	int n = inf.readInt(1, d, "n");
	inf.readSpace();
	int m = inf.readInt(1, 200000, "k");
	inf.readEoln();

	set<int> locs;
	for(int i = 0; i < m; i++) {
		int x = inf.readInt(1, d - 1, "x_i");
		inf.readSpace();
		int p = inf.readInt(1, int(1e9), "p_i");
		inf.readEoln();

		ensuref(locs.count(x) == 0, "Gas stations must be at distinct locations");

		locs.insert(x);
	}

	inf.readEof();
}
