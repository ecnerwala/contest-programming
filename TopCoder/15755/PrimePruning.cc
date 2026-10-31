#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int MAXV = 1.1e6;
bool isPrime[MAXV];

class PrimePruning {

public:

	string maximize(int N, int E) {
		int K = N - E;
		memset(isPrime, true, sizeof(isPrime));
		isPrime[0] = isPrime[1] = false;
		string ans;
		int i = 0;
		for (int p = 2; i < N && (int(ans.size()) < K || ans.back() < 'z'); p++) {
			if (!isPrime[p]) continue;
			for (int j = p + p; j < MAXV; j += p) { isPrime[j] = false; }

			char v = char('a' + p % 26);
			while (!ans.empty() && v > ans.back() && N - i - 1 >= K - int(ans.size())) {
				ans.pop_back();
			}
			ans.push_back(v);

			i++;
		}
		return ans.substr(0, K);
	}
};

void my_tests() {
	// FILL IN TESTS
	assert(PrimePruning().maximize(int(1e9), int(1e9) - 1000) == string(1000, 'z'));
	for (int E = 1; E <= 1000; E++) {
		for (int K : {1, 2, 3, 5, 10, 26, 1000}) {
			string s = PrimePruning().maximize(E + K, E);
			assert(int(s.size()) == K);
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::PrimePruning;

// vim:ft=cpp
