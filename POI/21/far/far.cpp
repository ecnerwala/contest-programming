#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

const int MAXN = 1e6;
int N;
ll C[MAXN];
vector<int> adj[MAXN];

// returns travel, install
pll dfs(int cur, int par = -1) {

	ll travel = 0;
	ll install = C[cur];

	vector<pll> ch;

	for(int i = 0; i < (int) adj[cur].size(); i++) {
		int nxt = adj[cur][i];
		if(nxt == par) continue;

		pll val = dfs(nxt, cur);

		val.first += 2;
		val.second += 1;

		travel += val.first;

		ch.push_back(pll(val.first - val.second, val.second));
	}

	sort(ch.begin(), ch.end());

	ll curtime = 0;
	for(int i = 0; i < (int) ch.size(); i++) {
		pll val = ch[i];

		curtime += val.second;
		install = max(install, curtime);

		curtime += val.first;
	}

	return pll(travel, install);
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%lld", &C[i]);
	}

	for(int i = 0; i < N - 1; i++) {
		int s, t;
		scanf("%d %d", &s, &t);
		s--, t--;
		adj[s].push_back(t);
		adj[t].push_back(s);
	}

	ll res = C[0];
	C[0] = 0;


	pll val = dfs(0);

	res += val.first;
	res = max(res, val.second);

	printf("%lld\n", res);

	return 0;
}
