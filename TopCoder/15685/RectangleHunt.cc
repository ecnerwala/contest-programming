#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

int sq(int a) {
	return a * a;
}

class RectangleHunt {

public:

	double largest(vector <int> x, vector <int> y) {
		int N = int(x.size());
		int ans = 0;
		for (int a = 0; a < N; a++) {
			for (int b = a+1; b < N; b++) {
				for (int c = 0; c < N; c++) {
					for (int d = c+1; d < N; d++) {
						if (x[a] + x[b] == x[c] + x[d] && y[a] + y[b] == y[c] + y[d] && sq(x[a] - x[b]) + sq(y[a] - y[b]) == sq(x[c] - x[d]) + sq(y[c] - y[d])) {
							// take the area
							int area = (x[a] * y[b] + x[b] * y[c] + x[c] * y[a]) - (y[a] * x[b] + y[b] * x[c] + y[c] * x[a]);
							ans = max(ans, area);
						}
					}
				}
			}
		}
		if (ans == 0) ans = -1;
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::RectangleHunt;

// vim:ft=cpp
