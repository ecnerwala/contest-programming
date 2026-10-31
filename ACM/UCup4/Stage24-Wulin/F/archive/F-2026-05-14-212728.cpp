#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, R; std::cin >> N >> R;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		std::vector<int64_t> C(N);
		for (auto& c : C) std::cin >> c;

		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = (pref[i] + A[i]) % R;
		}

		std::map<int, int> last_occ;
		std::set<std::pair<int, int>> last_occ_set;
		std::vector<int64_t> dp(N+2);
		std::vector<std::pair<int64_t, int>> stk; stk.reserve(N+2);
		int idx = 0;
		stk.push_back({dp[N+1] = 0, N+1});
		for (int i = N; i >= 1; i--) {
			auto it = last_occ.find(pref[i]);
			if (it != last_occ.end()) {
				last_occ_set.erase({it->second, it->first});
			}
			last_occ[pref[i]] = i;
			last_occ_set.insert({i, pref[i]});

			int first_full = int(last_occ_set.size()) == R ? last_occ_set.rbegin()->first : N+1;
			while (idx < int(stk.size()) && stk[idx].second > first_full) idx++;
			dp[i] = C[i-1] + stk[idx].first;

			while (int(stk.size()) > idx && dp[i] <= stk.back().first) stk.pop_back();
			stk.push_back({dp[i], i});
		}
		auto it = last_occ.find(0);
		int first_0 = it == last_occ.end() ? N+1 : it->second;
		std::cout << *std::min_element(dp.begin() + 1, dp.begin() + first_0 + 1) << '\n';
	}

	return 0;
}
