#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SecondLargestMultiple {

public:

	long long find(long long N, int B) {
		int L = (B+1)/2;
		int R = B - L;
		int64_t best1 = 0, best2 = -1;
		auto check = [&](int64_t v) -> void {
			if (v % N != 0) return;

			if (v == best1) return;
			if (v > best1) {
				best2 = best1;
				best1 = v;
				return;
			}
			if (v == best2) return;
			if (v > best2) best2 = v;
		};

		vector<pair<int64_t, int64_t>> buf; buf.reserve(720);

		vector<int> digs(B); std::iota(digs.begin(), digs.end(), 0);
		do {
			assert(std::is_sorted(digs.begin() + L, digs.end()));
			if (std::is_sorted(digs.begin(), digs.begin() + L)) {
				buf.clear();
				do {
					int64_t v = 0;
					int64_t a = 1;
					for (int i = L; i < B; i++, a *= B) {
						v += digs[i] * a;
						check(v);
					}
					int64_t m = N - v % N;
					if (m == N) m = 0;
					buf.push_back({m, v});
				} while (std::next_permutation(digs.begin() + L, digs.end()));

				sort(buf.begin(), buf.end());

				int64_t baseA = 1;
				for (int i = L; i < B; i++) baseA *= B;
				
				do {
					int64_t v = 0;
					int64_t a = baseA;
					for (int i = 0; i < L; i++, a *= B) {
						v += digs[i] * a;
						{
							int64_t m = v % N;
							auto it = upper_bound(buf.begin(), buf.end(), pair<int64_t, int64_t>{m+1, -1});
							if (it != buf.begin()) {
								--it;
								if (it->first == m) {
									check(v + it->second);
									if (it != buf.begin()) {
										--it;
										if (it->first == m) {
											check(v + it->second);
										}
									}
								}
							}
						}
					}
				} while (std::next_permutation(digs.begin(), digs.begin() + L));
			}
			assert(std::is_sorted(digs.begin() + L, digs.end()));
			std::reverse(digs.begin() + L, digs.end());
		} while (std::next_permutation(digs.begin(), digs.end()));
		return best2;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::SecondLargestMultiple;

// vim:ft=cpp
