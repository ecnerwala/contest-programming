#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
	registerValidation();

	int r = inf.readInt(1, 3000, "r");
	inf.readSpace();
	int c = inf.readInt(1, 3000, "c");
	inf.readSpace();
	int n = inf.readInt(1, 3000, "n");
	inf.readSpace();
	int k = inf.readInt(1, min(n, 10), "k");
	inf.readEoln();

	set<pair<int, int> > positions;
	for(int i = 0; i < n; i++) {
		int x = inf.readInt(1, r, "x_i");
		inf.readSpace();
		int y = inf.readInt(1, c, "y_i");
		inf.readEoln();

		ensuref(positions.count(make_pair(x, y)) == 0, "Positions must be distinct");
		positions.insert(make_pair(x, y));
	}

	inf.readEof();
}
