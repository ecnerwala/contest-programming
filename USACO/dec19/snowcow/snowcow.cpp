#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = 1 << 17;

ll seg[2*S];
ll lazy[2*S];
void update(int i) {
	if (i < S) {
		seg[i] = seg[2*i] + seg[2*i+1];
	} else {
		seg[i] = 0;
	}

	seg[i] += lazy[i] * (1 << (__builtin_clz(i) - __builtin_clz(S)));
}

void downdate(int i) {
	if (i < S) {
		if (lazy[i]) {
			lazy[2*i] += lazy[i];
			lazy[2*i+1] += lazy[i];
			lazy[i] = 0;

			update(2*i);
			update(2*i+1);
		}
	}
}

void downdate_all(int i) {
	if (!i) return;
	downdate_all(i/2);
	downdate(i);
}

void insert(int l, int r, int v) {
	assert(l < r);
	for (int a = l+S, b = r+S; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			lazy[a] += v;
			update(a);
			a++;
		}
		if (b & 1) {
			--b;
			lazy[b] += v;
			update(b);
		}
	}
	for (int a = l+S; a; a /= 2) {
		update(a);
	}
	for (int b = r+S-1; b; b /= 2) {
		update(b);
	}
}

ll query(int l, int r) {
	assert(l < r);
	downdate_all(l+S);
	downdate_all(r+S-1);

	ll ans = 0;
	for (int a = l+S, b = r+S; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			ans += seg[a];
			a++;
		}
		if (b & 1) {
			--b;
			ans += seg[b];
		}
	}
	return ans;
}

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];
int st[MAXN];
int en[MAXN];
int ord[MAXN];

void dfs(int cur, int prv) {
	ord[st[cur]] = cur;
	en[cur] = st[cur] + 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		st[nxt] = en[cur];
		dfs(nxt, cur);
		en[cur] = en[nxt];
	}
}

const int MAXC = 1.1e5;
set<int> nodes[MAXC];

int main() {
	if(fopen("snowcow.in", "r")) {
		freopen("snowcow.in", "r", stdin);
		freopen("snowcow.out", "w", stdout);
	}

	int Q;
	cin >> N >> Q;
	for (int i = 0; i < N-1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	dfs(0, -1);

	while (Q--) {
		int t; cin >> t;
		if (t == 1) {
			int x, c; cin >> x >> c; x--, c--;
			auto it = nodes[c].upper_bound(st[x]);
			if (it != nodes[c].begin()) {
				auto jt = it;
				--jt;
				int p = ord[*jt];
				if (st[x] < en[p]) {
					// we're already covered
					continue;
				}
			}

			while (it != nodes[c].end() && *it < en[x]) {
				insert(*it, en[ord[*it]], -1);
				it = nodes[c].erase(it);
			}
			insert(st[x], en[x], 1);
			nodes[c].insert(st[x]);
		} else if (t == 2) {
			int x; cin >> x; x--;
			cout << query(st[x], en[x]) << '\n';
		} else assert(false);
	}

	return 0;
}
