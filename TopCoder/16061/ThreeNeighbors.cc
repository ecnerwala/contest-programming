#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ThreeNeighbors {

public:

	vector <string> construct(int N) {
		vector<string> ans(50, string(50, '.'));
		for (int i = 1; N >= 2; i += 3) {
			int v = min(48, N / 2);
			for (int j = 0; j <= v+1; j ++) {
				ans[i][j] = 'X';
			}
			N -= 2 * v;
		}

		if (N == 1) {
			ans[49][0] = ans[49][1] = ans[49][2] = 'X';
		}
		return ans;
	}
};

void my_tests() {
	auto cnt = [&](vector<string> a) -> int {
		int N = int(a.size());
		int M = int(a[0].size());

		int res = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				assert(a[i][j] == '.' || a[i][j] == 'X');
				if (a[i][j] == '.') {
					int nei = 0;
					for (int x = i-1; x <= i+1; x++) {
						for (int y = j-1; y <= j+1; y++) {
							if (0 <= x && x < N && 0 <= y && y < M) {
								nei += a[x][y] == 'X';
							}
						}
					}
					if (nei == 3) res++;
				}
			}
		}
		return res;
	};

	for (int N = 1; N <= 500; N++) {
		vector<string> a = ThreeNeighbors().construct(N);
		assert(cnt(a) == N);
	}
	// FILL IN TESTS
	for (string s : ThreeNeighbors().construct(499)) cerr << s << '\n';
	cerr << '\n';
	for (string s : ThreeNeighbors().construct(500)) cerr << s << '\n';
	cerr << '\n';
}

}  // namespace ecnerwala

using ecnerwala::ThreeNeighbors;

// vim:ft=cpp
