#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const vector<int> NO_SAD = vector<int>({-1});

class FixedPointReversals {

public:
	vector <int> sort(vector <int> A, int fixed) {
		int N = int(A.size());
		bool reversed = false;
		/*
		if (N-1-fixed < fixed) {
			reversed = true;
			reverse(A.begin(), A.end());
			for (auto& v : A) v = -v;
			fixed = N-1-fixed;
		}
		*/
		vector<int> ops;
		// do an inclusive swap
		auto do_op = [&](int a, int b) {
			assert(a <= b);
			if (a < b) {
				if (a <= fixed && fixed <= b) {
					assert(a + b == fixed + fixed);
				}
				if (reversed) {
					ops.emplace_back(N-1-b);
					ops.emplace_back(N-a);
				} else {
					ops.emplace_back(a);
					ops.emplace_back(b+1);

				}
				reverse(A.begin() + a, A.begin() + b + 1);
			}
		};
		for (int i = 0; i < N; i++) {
			if (i == fixed) {
				continue;
			}
			int best = i;
			for (int j = i+1; j < N; j++) {
				if (j == fixed) continue;
				if (A[j] < A[best]) best = j;
			}

			// swap this into place
			if (i > fixed) {
				do_op(i, best);
			} else if (best < fixed) {
				do_op(i, best);
			} else {
				assert(i < fixed && fixed < best);
				if (best - fixed == fixed - i) {
					do_op(i, best);
				} else if (best - fixed < fixed - i) {
					do_op(fixed - (best - fixed), best);
					do_op(i, fixed - (best - fixed));
				} else {
					do_op(fixed + (fixed - i), best);
					do_op(i, fixed + (fixed - i));
				}
			}
		}

		for (int i = 0; i < fixed; i++) {
			if (A[i] > A[fixed]) {
				return NO_SAD;
			}
		}
		for (int i = fixed+1; i < N; i++) {
			if (A[i] < A[fixed]) {
				return NO_SAD;
			}
		}
		return ops;
	}
};

vector<int> buildTest() {
	const int N = 50;
	const int fixed = 30;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		A[i] = i;
	}
	for (int i = N-1; i >= 0; i--) {
		if (i == N-1) {
			continue;
		} else if (i > fixed) {
			reverse(A.begin() + i, A.end());
		} else if (i == fixed) {
			continue;
		} else if (i == fixed-1) {
			reverse(A.begin() + i, A.begin() + fixed+2);
			reverse(A.begin() + i+2, A.end());
		} else {
			reverse(A.begin() + i, A.begin() + fixed);
			reverse(A.begin() + fixed-1, A.begin() + fixed+2);
		}
	}
	return A;
}

void my_tests() {
	// FILL IN TESTS
	auto res = FixedPointReversals().sort(vector<int>({5,4,1,3,2,0}), 3);
	for (auto v : res) { cerr << v << ' '; } cerr << '\n';

	/*
	mt19937 mt(48);
	vector<int> v(99);
	for (int i = 0; i < 99; i++) v[i] = i;
	reverse(v.begin(), v.end());
	v.push_back(99);
	shuffle(v.begin(), v.begin() + 49, mt);
	shuffle(v.begin() + 50, v.end(), mt);
	assert(v[49] == 49);

	for (auto a : v) { cerr << a << ", "; } cerr << '\n';
	res = FixedPointReversals().sort(v, 49);
	cerr << res.size() << '\n';
	*/

	auto v = buildTest();
	for (auto a : v) { cerr << a << ", "; } cerr << '\n';
	res = FixedPointReversals().sort(v, 30);
	cerr << res.size() << '\n';
}

}  // namespace ecnerwala

using ecnerwala::FixedPointReversals;

// vim:ft=cpp
