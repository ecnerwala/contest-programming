#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int leader(vector<int>& dsu, int idx) {
	return dsu[idx] == idx ? dsu[idx] : (dsu[idx] = leader(dsu, dsu[idx]));
}

bool merge(vector<int>& dsu, int a, int b) {
	a = leader(dsu, a);
	b = leader(dsu, b);
	if (a == b) {
		return false;
	} else {
		if (rnd.next(2) == 0) {
			dsu[a] = b;
		} else {
			dsu[b] = a;
		}
		return true;
	}
}

int main() {
	registerValidation();

	int n = inf.readInt(2, 200000, "n");
	inf.readSpace();
	inf.readInt(1, n, "k");
	inf.readEoln();

	for(int i = 1; i <= n; i++) {
		inf.readInt(1, 1000000, "a_" + to_string(i));
		if(i < n) inf.readSpace();
	}
	inf.readEoln();

	vector<int> dsu(n);
	for(int i = 0; i < n; i++) {
		dsu[i] = i;
	}

	set<pair<int,int> > edges;

	for(int i = 0; i < n - 1; i++) {
		int x = inf.readInt(1, n, "x_i");
		inf.readSpace();
		int y = inf.readInt(1, n, "y_i");
		inf.readEoln();

		ensuref(x != y, "Tree can't contain loops");
		ensuref(edges.count(make_pair(x, y)) == 0, "Tree can't contain multiple edges between a pair of vertices");

		edges.insert(make_pair(x, y));
		edges.insert(make_pair(y, x));

		ensuref(merge(dsu, x - 1, y - 1), "Tree can't contain cycles");
	}

	inf.readEof();
	return 0;
}
