#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;


namespace ecnerwala {

using ll = long long;

class LexicographicPartition {

public:
	vector<int> go(vector<int> A) {
		int sum = 0;
		for (int v : A) sum += v;
		cerr << sum << '\n';

		if (sum <= 0) return {};

		vector<int> B;
		int lastVal = 0;
		int curVal = 0;
		int curLen = 0;
		for (int v : A) {
			curVal += v;
			curLen ++;
			if (lastVal < curVal && curVal < sum) {
				B.push_back(curLen);
				curLen = 0;
				lastVal = curVal;
			}
		}
		assert(curVal == sum);
		B.push_back(curLen);

		return B;
	}

	vector <int> positiveSum(int n, vector <int> Aprefix, int seed, int Arange) {
		vector<int> A = Aprefix;
		long long state = seed;
		A.resize(n);
		for (int i = int(Aprefix.size()); i < n; i++) {
			state = (1ll * 1103515245 * state + 12345);
			A[i] = int(state % (2*Arange+1) - Arange);
			state &= (1ll << 31) - 1;
		}

		vector<int> B = go(A);
		if (B.empty()) {
			return {-1};
		} else {
			vector<int> res;
			res.push_back(int(B.size()));
			res.insert(res.end(), B.begin(), B.begin() + min(int(B.size()), 200));
			return res;
		}
	}
};

void my_tests() {
	LexicographicPartition().positiveSum(200000, {}, 129804373, 0);
	vector<int> badPref;
	for (int i = 0; i < 500; i++) badPref.push_back(int(1e6));
	const int n = int(2e5) - 500;
	ll sum = 0;
	vector<int> dq(n);
	ll state = 91283500;
	const int Arange = int(1e6);
	ll mabs = 0;
	for (ll i = 0; true; i++) {
		state = (1ll * 1103515245 * state + 12345);
		int v = int(state % (2*Arange+1) - Arange);
		state &= (1ll << 31) - 1;

		sum -= dq[i % n]; dq[i % n] = v; sum += dq[i % n];
		mabs = max(mabs, abs(sum));
		if (sum + int(5e8) >= (1ll << 31) || sum - int(5e8) < -(1ll << 31)) {
			cout << "found " << i << '\n';
			return;
		}
		if (i % int(1e8) == 0) {
			cerr << "round " << i << ' ' << sum << ' ' << mabs << '\n';
		}
	}
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::LexicographicPartition;

// vim:ft=cpp
