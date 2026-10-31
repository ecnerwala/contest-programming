#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<string> S(N);
	for (int i = 0; i < N; i++) {
		cin >> S[i];
		reverse(S[i].begin(), S[i].end());
	}
	sort(S.begin(), S.end(), [](const string& a, const string& b) { return a.size() > b.size(); });

	int V = 1;
	vector<array<int, 26>> ch;
	vector<array<int, 26>> dp;
	vector<int> par;

	ch.emplace_back(); ch.back().fill(0);
	dp.emplace_back(); dp.back().fill(0);
	par.push_back(-1);

	int64_t ans = 0;
	for (string s : S) {
		char last = s.back();
		s.pop_back();
		int cur = 0;
		for (char c : s) {
			if (!ch[cur][c-'a']) {
				int ind = V++;
				ch.emplace_back(); ch.back().fill(0);
				dp.emplace_back(); dp.back().fill(0);
				par.push_back(cur);
				ch[cur][c-'a'] = ind;
			}
			cur = ch[cur][c-'a'];
		}
		ans += dp[cur][last-'a'];

		dp[cur][last-'a']++;

		array<bool, 26> seen; seen.fill(false);
		seen[last-'a'] = true;

		for (int z = int(s.size())-1; z >= 0; z--) {
			seen[s[z]-'a'] = true;
			cur = par[cur];
			for (int i = 0; i < 26; i++) {
				if (seen[i]) {
					dp[cur][i]++;
				}
			}
		}
	}
	cout << ans << '\n';

	return 0;
}

// S_i -> S_j iff S_i.endswith(S_j[1:]) and S_j[0]
