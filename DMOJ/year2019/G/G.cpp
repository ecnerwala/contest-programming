#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll MOD = int(1e9)+7;

const int MAXN = 3e5;
int N;
using bs = bitset<MAXN>;
int deg[MAXN];
bs adj[MAXN];

const int MAXM = 3.1e5;
int M;
pair<int, int> A[MAXM];

int main() {
	(void) scanf("%d %d", &N, &M);
	for (int i = 0; i < M; i++) {
		int a, b; (void) scanf("%d %d", &a, &b);
		a--, b--;
		A[i] = {a,b};
		deg[a]++;
		deg[b]++;
		adj[a].set(b);
		adj[b].set(a);
	}
	int res = 0;
	for (int i = 0; i < M; i++) {
		int a = A[i].first, b = A[i].second;
		int common = (adj[a] & adj[b]).count();
		res = (res + (ll(deg[a] - 1) * ll(deg[b] - 1) - common-1) % MOD * common) % MOD;
	}
	cout << res << '\n';

	return 0;
}
