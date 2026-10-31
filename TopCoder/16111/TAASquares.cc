#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class TAASquares {

public:

	vector <string> construct(int N) {
		vector<string> res(N, string(N, '0'));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (i < j) res[i][j] = '2';
				else if (i > j) res[i][j] = '0';
				else res[i][j] = (i < N/2) ? '1' : '2';
			}
		}
		return res;
	}
};

int numSums(vector<string> v) {
	int N = int(v.size());
	vector<int> s;
	for (int i = 0; i < N; i++) {
		int cur = 0;
		for (int j = 0; j < N; j++) {
			cur += v[i][j] - '0';
		}
		s.push_back(cur);
	}
	for (int j = 0; j < N; j++) {
		int cur = 0;
		for (int i = 0; i < N; i++) {
			cur += v[i][j] - '0';
		}
		s.push_back(cur);
	}
	sort(s.begin(), s.end());
	//for (auto it : s) { cerr << it << " "; } cerr << '\n';
	return int(unique(s.begin(), s.end()) - s.begin());
}

vector<string> slow(int N) {
	vector<string> s(N, string(N, '0'));
	vector<string> bestGrid;
	int bestAns = -1;
	while (true) {
		int cnd = numSums(s);
		if (cnd == 2 * N) {
			bestAns = cnd;
			bestGrid = s;
			break;
		}

		int i;
		for (i = N*N-1; i >= 0; i--) {
			if (s[i/N][i%N] == '2') {
				s[i/N][i%N] = '0';
				continue;
			} else {
				s[i/N][i%N]++;
				break;
			}
		}
		if (i == -1) break;
	}
	cerr << N << ' ' << bestAns << '\n';
	for (string r : bestGrid) cerr << r << '\n';
	return bestGrid;
}

void my_tests() {
	// FILL IN TESTS
	for (int N = 1; N <= 7; N++) {
		//slow(N);
		cerr << N << ' ' << numSums(TAASquares().construct(N)) << '\n';
	}
}

}  // namespace ecnerwala

using ecnerwala::TAASquares;

// vim:ft=cpp
