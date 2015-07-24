#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class Bitwisdom {

public:

	double expectedActions(vector <int> p) {
		double res = 1;
		for(int i = 0; i < int(p.size()); i++) {
			res *= p[i];
			res /= 100;
		}

		for(int i = 0; i + 1 < int(p.size()); i++) {
			res += double(p[i] * (100 - p[i + 1])) / 10000;
			res += double(p[i + 1] * (100 - p[i])) / 10000;
		}
		return res;
	}
};

// vim:ft=cpp
