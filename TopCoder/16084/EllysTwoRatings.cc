#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

double arr[1000][1000];
double tmp[1001];
class EllysTwoRatings {

public:

	double getChance(int N, int A, int B) {
		A--, B--;
		memset(arr, 0, sizeof(arr));
		arr[B][A] = 1.;
		double ans = 0;
		for (int z = 0; z < 2 * N; z++) {
			for (int i = 0; i < 1000; i++) {
				memset(tmp, 0, sizeof(tmp));
				for (int j = 0; j < 1000; j++) {
					int lo = max(j - 100, 0);
					int hi = min(j + 100, 999);
					double v = arr[i][j] / (hi - lo + 1);
					tmp[lo] += v;
					tmp[hi+1] -= v;
				}
				double cur = 0;
				for (int j = 0; j < 1000; j++) {
					cur += tmp[j];
					arr[i][j] = cur;
				}
			}

			for (int i = 0; i < 1000; i++) {
				ans += arr[i][i];
				arr[i][i] = 0;
			}

			// transpose arr
			for (int i = 0; i < 1000; i++) {
				for (int j = i+1; j < 1000; j++) {
					swap(arr[i][j], arr[j][i]);
				}
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EllysTwoRatings;

// vim:ft=cpp
