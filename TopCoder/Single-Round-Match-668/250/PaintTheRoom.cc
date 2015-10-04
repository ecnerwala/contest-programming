#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class PaintTheRoom {

	bool is_good(int R, int C, int K) {
		if(R > C) swap(R, C);
		assert(R <= C);
		if(R == 1) {
			if(K == 1) return true;
			assert(K >= 2);
			if(C == 1) return false;
			if(C == 2) return true;
			assert(C >= 3);
			return false;
		}
		assert(R >= 2 && C >= 2);
		// if one dimension is even, we are good
		if(R % 2 == 0 || C % 2 == 0) return true;
		if(K == 1) return true;
		return false;
	}

public:

	string canPaintEvenly(int R, int C, int K) {
		return is_good(R, C, K) ? "Paint" : "Cannot paint";
	}
};

// vim:ft=cpp
