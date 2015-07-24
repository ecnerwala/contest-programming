#include<bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXS = 1.1e7;

const int MAXP = 1e6;
int P;
int fac[MAXS];
int primes[MAXP];

const int MAXN = 2e5;
int N;

const int MAXV = MAXN + MAXP;
int V;
vector<int> adj[MAXV];

const int MAXE = MAXN * 10; // each value has at most 8 prime factors
int E; // number of edges

// vis = 0: never visited
// vis =-1: visited on floodfill
// vis > 0: depth of a node
int vis[MAXV];

int lowval[MAXV];

inline bool issong(int v) { return v < N; }

int ans = MAXN;

// return size
int dfs(int cur, int par, int ccsz) {
	//cerr << cur << ' ' << par << ' ' << ccsz << '\n';
	assert(vis[cur] > 0);
	lowval[cur] = vis[cur];

	int totsz = issong(cur);

	int upsz = ccsz - issong(cur);
	int val = 0;

	assert(int(adj[cur].size()) > 0);

	for(int nxt : adj[cur]) {
		if(nxt == par) continue;

		if(vis[nxt] > 0) {
			lowval[cur] = min(lowval[cur], vis[nxt]);
		} else {
			vis[nxt] = vis[cur] + 1;

			int s = dfs(nxt, cur, ccsz);
			totsz += s;

			lowval[cur] = min(lowval[cur], lowval[nxt]);

			if(lowval[nxt] >= vis[cur]) {
				// s forms a biconnected component if this is deleted
				val = max(val, s);
				upsz -= s;
			}
		}
	}

	val = max(val, upsz);

	if(issong(cur)) {
		ans = min(ans, val);
	}
	
	return totsz;
}

int flood(int cur) {
	if(vis[cur]) return 0;
	vis[cur] = -1;
	int res = issong(cur);
	for(int nxt : adj[cur]) {
		res += flood(nxt);
	}
	return res;
}

void precomp() {
	memset(fac, -1, sizeof(fac));

	for(int i = 2; i < MAXS; i++) {
		if(fac[i] == -1) {
			primes[P] = i;
			for(int j = i; j < MAXS; j += i) {
				fac[j] = P;
			}
			P++;
		}
	}
	assert(P < MAXP);
}

int main() {
	ios_base::sync_with_stdio(0);

	precomp();

	cin >> N;

	V = N + P;

	for(int i = 0; i < N; i++) {
		int S; cin >> S;
		assert(S < MAXS);
		while(S > 1) {
			int v = fac[S];
			int p = primes[v];
			while(S % p == 0) S /= p;

			v += N;

			//cerr << v << ' ' << i << '\n';
			adj[v].push_back(i);
			adj[i].push_back(v);

			E++;
		}
	}
	//cerr << '\n';

	vector<pii> cc;
	for(int i = 0; i < V; i++) {
		if(vis[i]) continue;
		int s = flood(i);
		cc.push_back(pii(s, i));
	}

	sort(cc.begin(), cc.end());
	reverse(cc.begin(), cc.end());

	assert(!cc.empty());

	for(int i = 0; i < int(cc.size()); i++) {
		if(cc[i].A == cc[0].A) {
			//cerr << cc[i].A << ' ' << cc[i].B << '\n';
			vis[cc[i].B] = 1;
			dfs(cc[i].B, -1, cc[i].A);
		}
	}
	if(int(cc.size()) >= 2) ans = max(ans, cc[1].A);
	cout << ans << '\n';
}
