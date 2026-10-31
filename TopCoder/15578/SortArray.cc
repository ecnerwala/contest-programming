#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

#define ALL(a) (a).begin(), (a).end()

class SortArray {

public:

	vector <int> verify(int N, vector <int> commands) {
		vector<vector<int>> suffixes;
		for (auto cmd : commands) {
			suffixes.emplace_back();
			for (int v = cmd; v; v &= (v-1)) {
				suffixes.back().push_back(v);
			}
			suffixes.back().push_back(0);
			reverse(ALL(suffixes.back()));
		}
		for (int m = 0; m < (1 << N); m++) {
			int cur = m;
			for (const auto& suff : suffixes) {
				int sub = cur & suff.back();
				int l = __builtin_popcount(sub);
				cur = cur - sub + suff[l];
			}
			int mcnt = __builtin_popcount(m);
			int tgt = (1 << N) - (1 << (N - mcnt));
			if (cur != tgt) {
				// return m
				vector<int> bad;
				for (int i = 0, a = 0, b = N - mcnt; i < N; i++) {
					if (m & (1 << i)) bad.push_back(b++);
					else bad.push_back(a++);
				}
				return bad;
			}
		}
		return vector<int>();
	}
};

void my_tests() {
	// FILL IN TESTS
}

#undef ALL

}  // namespace ecnerwala

using ecnerwala::SortArray;

// vim:ft=cpp
