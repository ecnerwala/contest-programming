#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

int slow(string S) {
	int N = int(S.size());
	int numInversions = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (S[i] == 'A' && S[j] == 'B') {
				numInversions++;
			}
		}
	}

	set<int> possOps;
	for (int i = 0; i+1 < N; i++) {
		if (S[i] == 'A' && S[i+1] == 'B') {
			possOps.insert(i);
		}
	}

	int ans = 0;
	while (!possOps.empty()) {
		ans++;
		vector<int> ops;
		auto it = possOps.begin();
		while (it != possOps.end()) {
			ops.push_back(*it);
			numInversions--;
			++it;
		}
		possOps.erase(possOps.begin(), it);

		for (int i : ops) {
			assert(S[i] == 'A' && S[i+1] == 'B');
			swap(S[i], S[i+1]);
		}

		for (int i : ops) {
			assert(S[i] == 'B' && S[i+1] == 'A');
			if (i > 0 && S[i-1] == 'A') possOps.insert(i-1);
			if (i+2 < N && S[i+2] == 'B') possOps.insert(i+1);
		}
	}
	return ans;
}

class ABBAReplace {

public:
	int countStepsString(string S) {
		int N = int(S.size());
		vector<int> prefA(N+1);
		vector<int> suffB(N+1);
		for (int i = 0; i < N; i++) {
			prefA[i+1] = prefA[i] + (S[i] == 'A');
		}
		for (int i = N-1; i >= 0; i--) {
			suffB[i] = suffB[i+1] + (S[i] == 'B');
		}
		int ans = 0;
		for (int i = 0; i <= N; i++) {
			if (prefA[i] && suffB[i]) {
				ans = max(ans, prefA[i] + suffB[i] - 1);
			}
		}
		return ans;
	}

	int countSteps(string Sprefix, int N, int seed, int threshold) {
		long long state = seed;
		string S = Sprefix;
		while (int(S.size()) < N) {
			state = (state * 1103515245 + 12345) % (1ll << 31);
			if (state < threshold) {
				S += 'A';
			} else {
				S += 'B';
			}
		}
		assert(N == int(S.size()));
		return countStepsString(S);
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int l = 0; l <= 14; l++) {
		for (int m = 0; m < (1<<l); m++) {
			string S;
			for (int i = 0; i < l; i++) S += (m & (1<<i)) ? 'B' : 'A';
			int slowVal = slow(S);
			int myVal = ABBAReplace().countStepsString(S);
			if (slowVal != myVal) {
				cerr << S << ' ' << slowVal << ' ' << myVal << '\n';
				assert(false);
			}
		}
	}
	cerr << ABBAReplace().countStepsString(string(int(3.5e6), 'A') + string(int(3.5e6), 'B')) << '\n'; 
}

}  // namespace ecnerwala

using ecnerwala::ABBAReplace;

// vim:ft=cpp
