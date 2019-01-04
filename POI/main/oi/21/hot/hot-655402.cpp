#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e4;

int N;
vector<int> adj[MAXN];

ll pows[MAXN][4];
ll cnt[MAXN];

void dfs(int cur, int prv, int dist = 1) {
	cnt[dist] ++;
	dist++;
	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;

		dfs(nxt, cur, dist);
	}
}

ll go(int a) {
	memset(pows, 0, sizeof(pows));
	for(vector<int>::iterator it = adj[a].begin(); it != adj[a].end(); it++) {
		memset(cnt, 0, sizeof(cnt));
		dfs(*it, a);

		for(int d = 1; d < N; d++) {
			ll v = 1;
			for(int e = 0; e <= 3; e++, v *= cnt[d]) {
				pows[d][e] += v;
			}
		}
	}

	ll res = 0;
	for(int d = 1; d < N; d++) {
		ll p1 = pows[d][1];
		ll p2 = pows[d][2];
		ll p3 = pows[d][3];
		// p1^3 - p3 = 3a^2b + 6 abc
		// p1 p2 = a^2b + a^3
		// p1^3 - p3 - 3(p1p2 - p3) = 6 abc
		// (p1^3 - 3 p1 p2 + 2 p3) = 6 abc

		ll v = p1 * p1 * p1 - 3 * p1 * p2 + 2 * p3;
		assert(v % 6 == 0);
		v /= 6;
		res += v;
	}
	return res;
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N - 1; i++) {
		int a, b;

		scanf("%d%d", &a, &b);
		a--, b--;

		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	ll res = 0;
	for(int i = 0; i < N; i++) {
		res += go(i);
	}

	printf("%lld\n", res);

	return 0;
}
