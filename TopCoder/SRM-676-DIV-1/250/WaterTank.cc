#include <bits/stdc++.h>
using namespace std;

typedef long double ld;

class WaterTank {

	ld C;
	vector<int> X;
	vector<int> T;

public:

	bool is_good(ld R) {
		ld val = 0;
		for(int i = 0; i < int(X.size()); i++) {
			val += (X[i] - R) * T[i];
			if(val < 0) val = 0;
			if(val > C) return false;
		}
		return true;
	}

	double minOutputRate(vector <int> t, vector <int> x, int c) {
		X = x, T = t, C = c;
		ld mi = 0;
		ld ma = 1e6 + 10;
		assert(is_good(ma));
		for(int i = 0; i < 2000; i++) {
			ld md = (mi + ma) / 2;
			if(is_good(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return ma;
	}
};

// vim:ft=cpp
