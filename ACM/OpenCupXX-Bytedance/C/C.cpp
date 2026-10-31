#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto char_to_val = [](char c) -> int {
		if ('0' <= c && c <= '9') return c - '0';
		else if ('a' <= c && c <= 'f') return c - 'a' + 10;
		else assert(false);
	};

	using i64 = int64_t;
	int N, Q, B; cin >> N >> Q >> B;
	vector<int> S(N);
	{
		string S_; cin >> S_;
		assert(int(S_.size()) == N);
		for (int i = 0; i < N; i++) {
			S[i] = char_to_val(S_[i]);
		}
	}

	vector<vector<int>> transForward(1 << B, vector<int>(B));
	for (int m = 0; m < (1 << B); m++) {
		for (int v = 0; v < B; v++) {
			for (int i = 0; i < B; i++) {
				if (m & (1<<i)) {
					int nv = v + i; if (nv >= B) nv -= B-1;
					transForward[m][v] |= (1 << nv);
				}
			}
		}
	}

	using msk_t = uint32_t;
	vector<vector<i64>> totDp(1 << B, vector<i64>(B));

	{
		vector<pair<pair<msk_t, int>, int>> dp; dp.reserve(4 * B * B);
		for (int i = 0; i < N; i++) {
			dp.push_back({{0, 0}, 1});
			for (auto& it : dp) {
				it.first.first = transForward[it.first.first][S[i]] | (1 << it.first.second);
				it.first.second += S[i];
				if (it.first.second >= B) it.first.second -= B-1;
			}
			sort(dp.begin(), dp.end(), std::greater<pair<pair<msk_t, int>, int>>());
			int r = 0;
			for (int l = 0; l < int(dp.size()); l++) {
				if (r && dp[l].first == dp[r-1].first) {
					dp[r-1].second += dp[l].second;
				} else {
					if (r < l) {
						dp[r] = dp[l];
					}
					r++;
				}
			}
			dp.resize(r);

			for (const auto& it : dp) {
				totDp[it.first.first][it.first.second] += it.second;
			}
		}
	}

	i64 totCnt = i64(N) * i64(N+1) / 2;

	for (int i = 0; i < B; i++) {
		for (int m = 0; m < (1 << B); m++) {
			if (m & (1 << i)) continue;
			for (int v = 0; v < B; v++) {
				totDp[m | (1 << i)][v] += totDp[m][v];
			}
		}
	}

	vector<vector<i64>> ans(1 << B, vector<i64>(B, totCnt));
	for (int m = 0; m < (1 << B); m++) {
		for (int x = 0; x < B; x++) {
			int badSet = 0;
			for (int v = 0; v < B; v++) {
				if (transForward[m][v] & (1 << x)) {
					// this guy can make it
				} else {
					badSet |= (1 << v);
				}
			}
			ans[m][x] = totCnt;
			for (int v = 0; v < B; v++) {
				if (v == x) continue;
				ans[m][x] -= totDp[badSet][v];
			}
		}
	}

	while (Q--) {
		int x; vector<int> A;
		{
			char x_; cin >> x_; x = char_to_val(x_);

			string A_; cin >> A_;
			A.resize(A_.size());
			for (int i = 0; i < int(A.size()); i++) {
				A[i] = char_to_val(A_[i]);
			}
		}
		int m = 0;
		for (int v : A) m |= (1 << v);
		cout << ans[m][x] << '\n';
	}

	return 0;
}
