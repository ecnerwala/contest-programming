#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SortTwoArrays {

public:

	vector <int> twoSorts(int N, vector <int> A, vector <int> B) {
		std::vector<int> ops; ops.reserve(6*N);
		for (int i = 0; i < N; i++) {
			std::pair<int, int> mi(A[i], i);
			for (int j = i; j < N; j++) {
				mi = std::min(mi, std::pair<int, int>(A[j], j));
				mi = std::min(mi, std::pair<int, int>(B[j], N+j));
			}
			if (mi.second < N) {
				ops.push_back(mi.second);
				ops.push_back(i);
				std::swap(A[mi.second], B[i]);

				mi = {A[i], i};
				for (int j = i+1; j < N; j++) {
					mi = std::min(mi, std::pair<int, int>(A[j], j));
					mi = std::min(mi, std::pair<int, int>(B[j], N+j));
				}
				if (mi.second < N) {
					// swap it somewhere in the middle
					ops.push_back(mi.second);
					ops.push_back(N-1);
					std::swap(A[mi.second], B[N-1]);
					ops.push_back(i);
					ops.push_back(N-1);
					std::swap(A[i], B[N-1]);
				} else {
					ops.push_back(i);
					ops.push_back(mi.second - N);
					std::swap(A[i], B[mi.second - N]);
				}
			} else {
				ops.push_back(i);
				ops.push_back(mi.second - N);
				std::swap(A[i], B[mi.second - N]);
				mi = {B[i], N+i};
				for (int j = i+1; j < N; j++) {
					mi = std::min(mi, std::pair<int, int>(A[j], j));
					mi = std::min(mi, std::pair<int, int>(B[j], N+j));
				}
				if (mi.second < N) {
					ops.push_back(mi.second);
					ops.push_back(i);
					std::swap(A[mi.second], B[i]);
				} else {
					ops.push_back(N-1);
					ops.push_back(mi.second - N);
					std::swap(A[N-1], B[mi.second - N]);
					ops.push_back(N-1);
					ops.push_back(i);
					std::swap(A[N-1], B[i]);
				}
			}
		}
		assert(std::is_sorted(A.begin(), A.end()));
		assert(std::is_sorted(B.begin(), B.end()));
		assert(int(ops.size()) <= 6*N);
		return ops;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::SortTwoArrays;

// vim:ft=cpp
