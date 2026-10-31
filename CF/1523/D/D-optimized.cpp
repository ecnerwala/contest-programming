#include <bits/stdc++.h>

static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
std::mt19937 mt(FIXED_RANDOM);

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, P; cin >> N >> M >> P;
	vector<int64_t> msk(N);
	vector<int> cnt(M);
	for (auto& a : msk) {
		string s; cin >> s;
		for (int i = 0; i < M; i++) {
			cnt[i] += (s[i] - '0');
			a += (int64_t(s[i] - '0')) << i;
		}
	}

	int64_t all_msk = 0;
	for (int i = 0; i < M; i++) {
		if (cnt[i] * 2 >= N) {
			all_msk |= int64_t(1) << i;
		}
	}
	for (auto& a : msk) {
		a &= all_msk;
	}

	sort(msk.begin(), msk.end());

	vector<pair<int64_t, int>> dp(N);
	for (int i = 0; i < N; i++) {
		if (!dp.empty() && dp.back().first == msk[i]) dp.back().second++;
		else dp.push_back({msk[i], 1});
	}

	int ans = 0;
	int64_t best_msk = 0;
	auto fix_dp = [&]() {
		int L = 0;
		for (int i = 0; i < int(dp.size()); i++) {
			auto [m, v] = dp[i];
			int ppc = __builtin_popcountll(m);
			if (ppc <= ans) {
				continue;
			} else if (v * 2 >= N) {
				ans = ppc;
				best_msk = m;
			} else {
				dp[L++] = {m, v};
			}
		}
		dp.resize(L);
	};
	fix_dp();

	std::vector<int64_t> bits; bits.reserve(__builtin_popcountll(all_msk));
	for (int b = 0; b < M; b++) {
		if (!(all_msk & (int64_t(1) << b))) continue;
		bits.push_back(int64_t(1) << b);
	}
	std::shuffle(bits.begin(), bits.end(), mt);

	for (int64_t pb : bits) {
		vector<pair<int64_t, int>> ndp; ndp.reserve(dp.size()*2);
		for (int i = 0, j = 0; i < int(dp.size()) || j < int(dp.size()); ) {
			if (i < int(dp.size()) && !(dp[i].first & pb)) {
				i++;
				continue;
			}
			if (i < int(dp.size()) && j < int(dp.size()) && (dp[i].first ^ pb) == dp[j].first) {
				ndp.push_back({dp[j].first, dp[i].second + dp[j].second});
				i++, j++;
			} else if (i < int(dp.size()) && (j == int(dp.size()) || (dp[i].first ^ pb) < dp[j].first)) {
				ndp.push_back({dp[i].first ^ pb, dp[i].second});
				i++;
			} else {
				ndp.push_back(dp[j]);
				j++;
			}
		}
		dp = std::move(ndp);
		fix_dp();
	}

	for (int b = 0; b < M; b++) {
		cout << ((best_msk >> b) & 1);
	}
	cout << '\n';

	return 0;
}
