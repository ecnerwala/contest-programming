#include <bits/stdc++.h>

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
	//sat_num& operator *= (sat_num o) { if (v > 0 && o.v > INF / v) v = INF; else v *= o.v; return *this; }
	friend sat_num operator + (sat_num a, sat_num b) { return sat_num(a) += b; }
	//friend sat_num operator * (sat_num a, sat_num b) { return sat_num(a) *= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K; int64_t N; cin >> K >> N;
	assert(N <= INF);

	auto get_cnt = [&](std::vector<int> D) -> int64_t {
		int L = int(D.size());

		//cerr << "trying" << ' ';
		//for (int i = L-1; i >= 0; i--) { cerr << (D[i] == -1 ? '?' : char('0'+D[i])); } cerr << '\n';
		int C = (L+K-1) / K;
		sat_num tot_ans = 0;
		for (int s_mult = 1; s_mult <= C; s_mult++) {
			std::vector<sat_num> dp(1, sat_num(1));
			int target_val = 0;
			for (int m = 0; m < K; m++) {
				target_val += 9 * s_mult;
				for (int i = m; i < L; i += K) {
					if (D[i] == -1) {
						std::vector<sat_num> ndp(int(dp.size())+8);
						for (int v = 0; v < int(dp.size()); v++) {
							for (int d = 0; d <= 8; d++) {
								ndp[v+d] += dp[v];
							}
						}
						dp = std::move(ndp);
					} else if (0 <= D[i] && D[i] <= 8) {
						dp.insert(dp.begin(), D[i], sat_num(0));
					} else assert(false);
				}

				std::vector<sat_num> ndp(int(dp.size() + 9) / 10);
				for (int i = target_val % 10; i < int(dp.size()); i += 10) {
					ndp[i / 10] = dp[i];
				}
				dp = std::move(ndp);
				target_val /= 10;
			}
			if (target_val < int(dp.size())) {
				tot_ans += dp[target_val];
			}
		}
		//cerr << int64_t(tot_ans) << '\n';
		return int64_t(tot_ans);
	};

	int L = 0;
	while (get_cnt(std::vector<int>(L, -1)) < N) L++;
	std::vector<int> D(L, -1);
	for (int i = L-1; i >= 0; i--) {
		for (int d = 0; true; d++) {
			assert(d < 9);
			D[i] = d;
			int64_t cnt = get_cnt(D);
			if (N <= cnt) {
				break;
			} else {
				N -= cnt;
			}
		}
	}
	//for (int i = L-1; i >= 0; i--) { cerr << D[i]; } cerr << '\n';
	std::vector<int> ans;
	for (int i = 0; i+K <= L; i++) {
		int a = 9 * (D[i] % 10);
		a %= 10;
		if (a < 0) a += 10;
		ans.push_back(a);
		for (int v = 0; v < K; v++) {
			D[i+v] -= 9 * a;
		}
		assert(D[i] % 10 == 0);

		if (i+1 < L) {
			D[i+1] += D[i] / 10;
		} else {
			assert(D[i] == 0);
		}
	}
	while (ans.back() == 0) ans.pop_back();
	std::reverse(ans.begin(), ans.end());
	for (int i : ans) { cout << i; } cout << '\n';

	return 0;
}
