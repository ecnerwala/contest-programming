#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EpicPartition {

public:

	string createPartition(int N) {
		if (N % 4) return "";

		string res(6 * N, 'b');

		int sumA = 6 * N * (6 * N + 1) / 8;
		vector<int> as;
		for (int i = 1; i <= 2*N; i++) {
			sumA -= i;
			as.push_back(i);
		}
		while (sumA > 0) {
			for (int z = int(as.size()) - 1; sumA > 0 && z >= 0; z--) {
				as[z]++;
				sumA --;
			}
		}
		for (auto a : as) res[a-1] = 'a';

		int sumC = 6 * N * (6 * N + 1) / 4;
		vector<int> cs;
		for (int i = 6*N; i > 4 * N; i--) {
			sumC -= i;
			cs.push_back(i);
		}
		while (sumC < 0) {
			for (int z = int(cs.size()) - 1; sumC < 0 && z >= 0; z--) {
				cs[z]--;
				sumC ++;
			}
		}
		for (auto c : cs) res[c-1] = 'c';
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int n = 1; n <= 100; n++) {
		string res = EpicPartition().createPartition(n);
		assert(res.empty() == (n % 4 != 0));
		if (!res.empty()) {
			assert(int(res.size()) == 6*n);
			int cnt[3] = {0,0,0};
			int sum[3] = {0,0,0};
			for (int i = 0; i < 6*n; i++) {
				cnt[res[i] - 'a']++;
				sum[res[i] - 'a'] += i+1;
			}
			//cerr << cnt[0] << ' ' << cnt[1] << ' ' << cnt[2] << '\n';
			//cerr << sum[0] << ' ' << sum[1] << ' ' << sum[2] << '\n';
			assert(cnt[0] == cnt[1]);
			assert(cnt[1] == cnt[2]);
			assert(sum[0] == sum[1]);
			assert(sum[2] == sum[0] * 2);
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::EpicPartition;

// vim:ft=cpp
