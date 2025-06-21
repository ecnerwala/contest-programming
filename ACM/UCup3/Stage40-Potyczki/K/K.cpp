#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (a < b) a = b; }

struct frac_t {
	int64_t numer;
	int64_t denom;
	friend bool operator < (frac_t a, frac_t b) {
		return a.numer * b.denom < b.numer * a.denom;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int N = 5;
	int N_prob, M, Q; cin >> N_prob >> M >> Q;
	assert(N_prob <= N);

	auto choose = [&](int n, int r) -> int {
		int a = 1;
		for (int i = 0; i < r; i++) a *= n-i, a /= i+1;
		return a;
	};

	std::vector<std::vector<int>> poss_weights(N+1);
	poss_weights[0].push_back(0);
	for (int c = 0; c <= N; c++) {
		std::sort(poss_weights[c].begin(), poss_weights[c].end());
		poss_weights[c].resize(std::unique(poss_weights[c].begin(), poss_weights[c].end()) - poss_weights[c].begin());
		for (int d = 1; c + d <= N; d++) {
			for (auto v : poss_weights[c]) {
				poss_weights[c+d].push_back(2 * v + d);
			}
		}
	}
	std::vector<std::vector<std::vector<int>>> trans(N+1);
	for (int c = 0; c <= N; c++) {
		trans[c].resize(N-c+1);
		for (int d = 1; c + d <= N; d++) {
			trans[c][d].resize(poss_weights[c].size());
			for (int z = 0; z < int(poss_weights[c].size()); z++) {
				int v = poss_weights[c][z];
				trans[c][d][z] = int(std::lower_bound(poss_weights[c+d].begin(), poss_weights[c+d].end(), 2 * v + d) - poss_weights[c+d].begin());
				assert(poss_weights[c+d][trans[c][d][z]] == 2 * poss_weights[c][z] + d);
			}
		}
	}

	std::array<std::array<int, N>, N> weight;
	for (int i = 0; i < N; i++) {
		weight[i].fill(0);
	}

	for (int e = 0; e < M; e++) {
		int a, b, t; cin >> a >> b >> t; a--, b--;
		weight[a][b] += t;
	}

	std::vector<std::vector<int64_t>> dp(1 << N);
	for (int m = 0; m < (1 << N); m++) {
		dp[m].assign(poss_weights[__builtin_popcount(m)].size(), 0);
	}

	std::vector<frac_t> ans((1 << N), frac_t{0, 1});

	for (int m = 0; m < (1 << N); m++) {
		int c = __builtin_popcount(m);
		int not_m = (1 << N) - 1 - m;
		int nm = m;
		int64_t cur_cost = 0;
		for (int iter = not_m; iter != 0; iter = (iter-1) & not_m) {
			{
				int b = __builtin_ctz(iter);
				int d = (nm & (1 << b)) ? -1 : +1;
				for (int i = 0; i < N; i++) {
					if (i == b) {
						cur_cost += d * 2 * weight[i][i];
					} else if (m & (1 << i)) {
						cur_cost += d * (2 * weight[b][i] + weight[i][b]);
					} else if (nm & (1 << i)) {
						cur_cost += d * (weight[b][i] + weight[i][b]);
					}
				}
				nm ^= (1 << b);
			}
			assert((nm & m) == m);

			{
				int d = __builtin_popcount(nm ^ m);
				assert(d != 0);
				for (int z = 0; z < int(dp[m].size()); z++) {
					setmax(dp[nm][trans[c][d][z]], dp[m][z] * 2 + cur_cost);
				}
			}
		}
		for (int z = 0; z < int(dp[m].size()); z++) {
			setmax(ans[m], frac_t{dp[m][z], 2 * poss_weights[c][z]});
		}
		int64_t g = std::gcd(ans[m].numer, ans[m].denom);
		ans[m].numer /= g;
		ans[m].denom /= g;
	}

	for (int q = 0; q < Q; q++) {
		int m = 0;
		std::string s; cin >> s;
		assert(int(s.size()) == N_prob);
		for (int i = 0; i < N_prob; i++) {
			assert(s[i] == '0' || s[i] == '1');
			m += (s[i] - '0') << i;
		}
		cout << ans[m].numer << '/' << ans[m].denom << '\n';
	}

	return 0;
}
