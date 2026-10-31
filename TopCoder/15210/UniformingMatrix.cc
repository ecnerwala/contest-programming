#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class UniformingMatrix {

	bool go(const vector<string>& M) {
		int a = int(M.size());
		int b = int(M[0].size());
		for (int i = 1; i < a; i++) {
			for (int j = 1; j < b; j++) {
				if ((M[i][j] ^ M[0][j] ^ M[i][0] ^ M[0][0]) & 1) {
					return false;
				}
			}
		}
		if ((a + b) & 1) {
			return true;
		}
		int tot = 0;
		for (int i = 0; i < a; i++) {
			if (M[i][0] == '0') {
				tot++;
			}
		}
		for (int j = 1; j < b; j++) {
			if (M[0][j] != M[0][0]) {
				tot++;
			}
		}
		return tot % 2 == 0;
	}

public:

	string isPossible(vector <string> M) {
		return go(M) ? "possible" : "impossible";
	}
};

}  // namespace ecnerwala

using ecnerwala::UniformingMatrix;

// vim:ft=cpp
