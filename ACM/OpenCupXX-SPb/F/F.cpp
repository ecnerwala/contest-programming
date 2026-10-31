#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
vector<int> ch[MAXN];
int P[MAXN];

ll D[MAXN];
ll W[MAXN];

map<ll, ll> dp[MAXN];
ll curW[MAXN];

void dfs(int cur) {
	auto insert = [cur](ll d, ll w) {
		dp[cur][d] += w;
		curW[cur] += w;
	};
	insert(D[cur], W[cur]);

	for (int nxt : ch[cur]) {
		dfs(nxt);
		if (dp[nxt].size() > dp[cur].size()) {
			swap(dp[cur], dp[nxt]);
			swap(curW[cur], curW[nxt]);
		}
		for (auto it : dp[nxt]) {
			insert(it.first, it.second);
		}
	}

	//cerr << "cur" << ' ' << cur << '\n';
	//for (auto it : dp[cur]) { cerr << it.first << ' ' << it.second << '\n'; }
	while (curW[cur] > W[cur]) {
		auto dw = curW[cur] - W[cur];
		if (dp[cur].begin()->second > dw) {
			dp[cur].begin()->second -= dw;
			curW[cur] -= dw;
		} else {
			dw = dp[cur].begin()->second;
			dp[cur].erase(dp[cur].begin());
			curW[cur] -= dw;
		}
	}
	assert(curW[cur] == W[cur]);
	//cerr << "cur" << ' ' << cur << '\n';
	//for (auto it : dp[cur]) { cerr << it.first << ' ' << it.second << '\n'; }
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> W[0];
	D[0] = 0;
	for (int i = 1; i <= N; i++) {
		cin >> D[i] >> P[i] >> W[i];
		ch[P[i]].push_back(i);
	}
	dfs(0);
	ll ans = 0;
	for (auto it : dp[0]) {
		ans += it.first * it.second;
	}
	cout << ans << '\n';

	return 0;
}
