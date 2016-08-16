#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class ExploringNumbers {

public:

	bool is_prime(int cur) {
		if(cur <= 1) return false;
		if(cur == 2) return true;
		if(!(cur & 1)) return false;
		for(int i = 3; i <= cur / i; i+=2) {
			if(cur % i == 0) return false;
		}
		return true;
	}

	inline int sq(int a) { return a * a; }

	int get_next(int cur) {
		int res = 0;
		while(cur) {
			res += sq(cur % 10);
			cur /= 10;
		}
		return res;
	}

	int numberOfSteps(int n) {
		set<int> vis;
		int cur = n;
		for(int t = 0; t < n; t++) {
			if(vis.count(cur)) return -1;
			if(is_prime(cur)) return t + 1;
			vis.insert(cur);
			cur = get_next(cur);
		}
		return -1;
	}
};

// vim:ft=cpp
