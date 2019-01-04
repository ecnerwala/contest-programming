#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

const ll INF = 1e18;

const int MAXN = 1.2e6;
int N;
ll M;
pll P[MAXN];
ll dp[MAXN];

ll go() {
	P[N] = pll(-INF, -INF);
	sort(P, P + N+1);
	assert(P[0] == pll(-INF, -INF));

	deque<int> deepest;
	multiset<ll> cnds;
	int l = 0;
	dp[0] = 0;
	for (int i = 1; i <= N; i++) {
		assert(P[l].first <= P[i].first - M);
		while (P[l+1].first <= P[i].first - M) l++;
		assert(P[l].first <= P[i].first - M);
		assert(l < i);

		while (!deepest.empty() && deepest.front() <= l) {
			int a = deepest.front();
			deepest.pop_front();
			if (!deepest.empty()) {
				int b = deepest.front();
				ll v = dp[a] + P[b].second;
				cnds.erase(cnds.find(v));
			}
		}
		while (!deepest.empty() && P[deepest.back()].second <= P[i].second) {
			int b = deepest.back();
			deepest.pop_back();
			if (!deepest.empty()) {
				int a = deepest.back();
				ll v = dp[a] + P[b].second;
				cnds.erase(cnds.find(v));
			}
		}
		if (!deepest.empty()) {
			int a = deepest.back();
			int b = i;
			ll v = dp[a] + P[b].second;
			cnds.insert(v);
		}
		deepest.push_back(i);
		assert(!deepest.empty());
		dp[i] = dp[l] + P[deepest.front()].second;
		if (!cnds.empty()) {
			dp[i] = min(dp[i], *cnds.begin());
		}
	}

	return dp[N];
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		ll S; int K;
		cin >> N >> S >> M >> K;
		for (int r = 0; r < 2; r++) {
			int n = 0;
			for (int k = 0; k < K; k++) {
				ll l, a, x, y, z; cin >> l >> a >> x >> y >> z;
				for (int i = 0; i < l; i++) {
					(r == 0 ? P[n].first : P[n].second) = a;
					n++;
					if (i + 1 < l) {
						a = ((x * a + y) % z) + 1;
					}
				}
			}
			assert(n == N);
		}
		/*
		for (int i = 0; i < N; i++) {
			cerr << P[i].first << ' ' << P[i].second << '\n';
		}
		*/
		for (int i = 0; i < N; i++) {
			P[i].second += S;
		}
		M = 2 * M + 1;
		ll res = go();
		cout << "Case #" << t << ": " << res << '\n';
	}

	return 0;
}
