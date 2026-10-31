#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class DivNim {

public:

	vector <int> solve(long long lo, long long hi, vector<long long> board) {
		const int V = int(1e6);
		std::vector<int> primes; primes.reserve(V/2);
		std::vector<int> pfac(V+1);

		std::vector<int64_t> cur_val(hi-lo+1);
		std::iota(cur_val.begin(), cur_val.end(), lo);
		std::vector<int> num_fac(hi-lo+1);

		for (int i = 2; i <= int(1e6); i++) {
			if (!pfac[i]) {
				primes.push_back(i);
				pfac[i] = i;
				for (int64_t v = (lo + i - 1) / i * i; v <= hi; v += i) {
					while (cur_val[v - lo] % i == 0) {
						cur_val[v - lo] /= i;
						num_fac[v - lo]++;
					}
				}
			}

			for (int p : primes) {
				if (int64_t(i) * p > V) break;
				pfac[i*p] = p;
				if (p == pfac[i]) break;
			}
		}
		for (int i = 0; i < int(cur_val.size()); i++) {
			if (cur_val[i] > 1) num_fac[i] ++;
		}

		int tot_xor = 0;
		int cnt_1 = 0;
		int cnt_2 = 0;
		for (int64_t v : board) {
			int nimber = num_fac[v - lo];
			tot_xor ^= nimber;
			if (nimber == 1) cnt_1 ++;
			else if (nimber >= 2) cnt_2++;
		}
		int normal_win = 0;
		int misere_win = 0;
		for (int cnd : num_fac) {
			if ((tot_xor ^ cnd) == 0) {
				normal_win++;
			}
			if (cnt_2 + (cnd >= 2) == 0) {
				misere_win += (tot_xor ^ cnd) == 1;
			} else {
				misere_win += (tot_xor ^ cnd) == 0;
			}
		}
		return std::vector<int>({normal_win, misere_win});
	}
};

void my_tests() {
	// FILL IN TESTS
	int64_t lo = int64_t(1e12) - int64_t(1e5);
	int64_t hi = int64_t(1e12);
	auto r = DivNim().solve(lo, hi, {});
	cerr << r[0] << ' ' << r[1] << '\n';
}

}  // namespace ecnerwala

using ecnerwala::DivNim;

// vim:ft=cpp
