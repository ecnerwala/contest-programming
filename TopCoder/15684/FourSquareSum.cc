#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class FourSquareSum {

public:

	vector <int> DivideByTwo(int a, int b, int c, int d) {
		vector<int> v[2];
		v[a&1].push_back(a);
		v[b&1].push_back(b);
		v[c&1].push_back(c);
		v[d&1].push_back(d);
		vector<int> ans;
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < int(v[z].size()); i += 2) {
				int x = v[z][i], y = v[z][i+1];
				ans.push_back((x+y)/2);
				ans.push_back(abs(x-y)/2);
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FourSquareSum;

// vim:ft=cpp
