#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SortInversions {

public:

	long long count(int N_) {
		int64_t N = N_;
		assert(N >= 1);

		int64_t res = 0;

		vector<int64_t> p10(15);
		p10[0] = 1;
		for (int i = 1; i < int(p10.size()); i++) p10[i] = p10[i-1] * 10;

		int L = int(to_string(N).size());
		assert(p10[L-1] <= N && N < p10[L]);

		for (int l1 = 1; l1 <= L; l1++) {
			for (int l2 = l1+1; l2 <= L; l2++) {
				int64_t lo1 = p10[l1-1];
				int64_t hi1 = p10[l1];
				int64_t lo2 = p10[l2-1];
				int64_t hi2 = min(N+1, p10[l2]);

				assert(lo2 <= hi2);
				res += (hi1-1) * (hi2 - lo2)
					- ((hi2 % p10[l2-l1]) * (hi2 / p10[l2-l1]) + (lo2 / p10[l2-l1] + hi2 / p10[l2-l1] - 1) * (hi2 - hi2 % p10[l2-l1] - lo2) / 2);
			}
		}

		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	auto test = [](int N) {
		int64_t res = SortInversions().count(N);
		vector<string> a(N);
		for (int i = 0; i < N; i++) {
			a[i] = to_string(i+1);
		}
		sort(a.begin(), a.end());
		vector<int> b(N);
		for (int i = 0; i < N; i++) {
			b[i] = stoi(a[i]);
		}
		int64_t slow = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				slow += b[i] > b[j];
			}
		}
		assert(slow == res);
	};
	for (int N = 1; N <= 100; N++) test(N);
	cerr << SortInversions().count(1e9) << '\n';
	cerr << SortInversions().count(int(1e9)-1) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::SortInversions;

// vim:ft=cpp
