#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.6e5;
int N;
pair<pair<int, int>, int> bridges[MAXN];

multiset<ll, greater<ll>> dp[MAXN];

void dfs(int& i) {
	int cur = i;
	//cerr << cur << ' ' << bridges[cur].first.first << ' ' << -bridges[cur].first.second << ' ' << bridges[cur].second << '\n';
	++i;
	while (true) {
		if (i == N || bridges[i].first.first >= -bridges[cur].first.second) {
			break;
		}
		int nxt = i;
		dfs(i);
		//cerr << cur << ' ' << bridges[cur].first.first << ' ' << -bridges[cur].first.second << ' ' << bridges[cur].second << '\n';
		if (dp[nxt].size() > dp[cur].size()) {
			swap(dp[cur], dp[nxt]);
		}
		auto curit = dp[cur].begin();
		auto nxtit = dp[nxt].begin();
		while (curit != dp[cur].end() && nxtit != dp[nxt].end()) {
			auto v = *curit + *nxtit;
			curit = dp[cur].erase(curit);
			++nxtit;
			dp[cur].insert(v);
		}
	}
	dp[cur].insert(bridges[cur].second);
	//cerr << cur << ": ";
	//for (auto it : dp[cur]) { cerr << it << ' '; } cerr << '\n';
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		int S, E, V; cin >> S >> E >> V;
		bridges[i] = {{S, -E}, V};
	}
	bridges[N++] = {{-1, -int(1.1e6)}, 0};
	sort(bridges, bridges + N);
	int v = 0;
	dfs(v);

	auto it = dp[0].begin();
	ll ans = 0;
	for (int i = 0; i < N-1; i++) {
		if (it != dp[0].end()) {
			ans += *it;
			++it;
		}
		cout << ans << " \n"[i+1==N-1];
	}

	return 0;
}
