#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int64_t INF = int64_t(1e18) + 100;
struct sat_num {
	int64_t v = 0;
	sat_num() {}
	sat_num(int64_t a) : v(std::min<int64_t>(a, INF)) {}
	explicit operator int64_t() const { return v; }
	friend bool operator < (sat_num a, sat_num b) { return a.v < b.v; }
	friend bool operator <= (sat_num a, sat_num b) { return a.v <= b.v; }
	friend bool operator >= (sat_num a, sat_num b) { return a.v >= b.v; }
	friend bool operator > (sat_num a, sat_num b) { return a.v > b.v; }

	sat_num& operator += (sat_num o) { v += o.v; if (v > INF) v = INF; return *this; }
	sat_num& operator *= (sat_num o) { if (v > 0 && o.v > INF / v) v = INF; else v *= o.v; return *this; }
	friend sat_num operator + (sat_num a, sat_num b) { return sat_num(a) += b; }
	friend sat_num operator * (sat_num a, sat_num b) { return sat_num(a) *= b; }
};

class IntrospectiveNumbers {

public:

	string nth(long long index) {
		std::vector<std::vector<sat_num>> choose(46, std::vector<sat_num>(46));
		for (int i = 0; i <= 45; i++) {
			choose[i][0] = choose[i][i] = 1;
			for (int j = 1; j < i; j++) {
				choose[i][j] = choose[i-1][j-1] + choose[i-1][j];
			}
		}
		std::vector<sat_num> len_dp(45+1);
		len_dp[0] = 1;
		for (int d = 1; d <= 9; d++) {
			for (int j = 45; j >= d; j--) {
				len_dp[j] += choose[j][d] * len_dp[j-d];
			}
		}
		len_dp[0] = 0;

		int L = 0;
		while (index >= len_dp[L]) {
			index -= int64_t(len_dp[L]);
			L++;
		}
		assert(L >= 1);
		std::string res; res.reserve(L);
		std::array<int, 10> cnt; cnt.fill(0);
		auto query = [&]() -> sat_num {
			sat_num tot = 0;
			for (int m = 0; m < 1024; m += 2) {
				int s = 0;
				sat_num v = 1;
				for (int d = 1; d <= 9; d++) {
					int target_cnt = (m & (1 << d)) ? d : 0;
					if (target_cnt < cnt[d]) goto bad;
					int add = target_cnt - cnt[d];
					v *= choose[s+add][add];
					s += add;
				}
				if (s + int(res.size()) == L) {
					tot += v;
				}
bad:;
			}
			return tot;
		};
		assert(index < query());
		for (int z = 0; z < L; z++) {
			for (int d = 1; true; d++) {
				assert(d <= 9);
				res += char('0'+d);
				cnt[d]++;
				sat_num v = query();
				if (index < v) {
					break;
				}
				index -= int64_t(v);
				res.pop_back();
				cnt[d]--;
			}
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << IntrospectiveNumbers().nth(1e18) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::IntrospectiveNumbers;

// vim:ft=cpp
