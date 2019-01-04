#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6;
const int MAXK = 1e6;
int N;

typedef pair<int, int> pii;
#define A first
#define B second

struct sol {
	int x, y;

	bool operator < (const sol s) const {
		if(max(x, y) < max(s.x, s.y)) {
			return true;
		} else if(max(x, y) == max(s.x, s.y)) {
			if(min(x, y) < max(s.x, s.y)) {
				return true;
			} else if(min(x, y) == max(s.x, s.y)) {
				if(x == s.x && y == s.y) return false;
				if(x < y) return true;
			}
		}
		return false;
	}

	sol(int a = -1, int b = -1) : x(a), y(b) {}
};

struct dset {
	int par[MAXN];

	int getpar(int a) {
		return (par[a] == a) ? a : (par[a] = getpar(par[a]));
	}

	inline int operator () (int a) {
		return getpar(a);
	}

	bool merge(int a, int b) {
		a = getpar(a), b = getpar(b);
		if(a == b) return false;
		par[a] = b;
		return true;
	}

	void reset() {
		for(int i = 0; i < N; i++) par[i] = i;
	}

	dset () {
		reset();
	}
};

int P[MAXN];

vector<int> children[MAXN];

int depth[MAXN];

int cycpos[MAXN];
int cyclen[MAXN];

dset cpar, tpar;

int K;
pii C[MAXK];

sol answer[MAXK];

vector<int> queries[MAXN]; // stores queries for each value
bool vis[MAXN];
dset lcapar;

void dfsdepth(int cur) {
	for(int i = 0; i < int(children[cur].size()); i++) {
		int ch = children[cur][i];
		depth[ch] = depth[cur] + 1;
		dfsdepth(ch);
	}
}

void dfslca(int cur) {
	for(int i = 0; i < int(children[cur].size()); i++) {
		int ch = children[cur][i];
		dfslca(ch);
		lcapar.merge(ch, cur);
		assert(lcapar(cur) == cur);
	}
	vis[cur] = true;

	for(int i = 0; i < int(queries[cur].size()); i++) {
		int q = queries[cur][i];
		assert(cur == C[q].A || cur == C[q].B);
		int nxt = C[q].A ^ C[q].B ^ cur;
		if(vis[nxt]) {
			int d = depth[lcapar(nxt)];
			answer[q] = sol(depth[C[q].A] - d, depth[C[q].B] - d);
		}
	}
}

int main() {
	cin >> N >> K;
	for(int i = 0; i < N; i++) cin >> P[i], P[i] --;
	for(int i = 0; i < K; i++) cin >> C[i].A >> C[i].B, C[i].A --, C[i].B --;

	cpar.reset();
	tpar.reset();

	memset(cycpos, -1, sizeof(cycpos));

	for(int i = 0; i < N; i++) {
		cpar.merge(i, P[i]);
	}


	for(int i = 0; i < N; i++) {
		if(cpar(i) == i) {
			int x = i, y = i;
			do {
				x = P[x];
				y = P[P[y]];
			} while(x != y);

			// x == y == start of cycle

			int t = 0;
			do {
				cycpos[x] = t++;
				x = P[x];
			} while(x != y);

			cyclen[i] = t;
		}
	}

	for(int i = 0; i < N; i++) {
		if(cycpos[i] == -1) {
			// i is not in cycle
			tpar.merge(i, P[i]);
			children[P[i]].push_back(i);
		}
	}

	for(int i = 0; i < N; i++) {
		if(cycpos[i] != -1) {
			dfsdepth(i);
			assert(tpar(i) == i);
		}
	}

	//cerr << "hi\n";

	//for(int i = 0; i < N; i++) cerr << i << ' ' << cpar(i) << ' ' << tpar(i) << '\n';

	for(int i = 0; i < K; i++) {
		int u = C[i].A, v = C[i].B;
		int p = cpar(u);
		if(cpar(v) != p) {
			answer[i] = sol(-1, -1);
		} else if (tpar(u) != tpar(v)) {
			int x = depth[u];
			int y = depth[v];

			u = cycpos[tpar(u)], v = cycpos[tpar(v)];
			assert(u != -1);
			assert(v != -1);
			assert(u != v);

			int dx, dy;
			if(u < v) {
				dx = v - u;
				dy = cyclen[p] - dx;
			} else {
				dy = u - v;
				dx = cyclen[p] - dy;
			}

			answer[i] = min(sol(x + dx, y), sol(x, y + dy));
		} else {
			// tpar(u) == tpar(v)
			answer[i] = sol(0, 0);
			queries[u].push_back(i);
			queries[v].push_back(i);
		}
	}

	lcapar.reset();

	for(int i = 0; i < N; i++) {
		if(cycpos[i] != -1) {
			dfslca(i);
		}
	}

	for(int i = 0; i < K; i++) {
		cout << answer[i].x << ' ' << answer[i].y << '\n';
	}

	return 0;
}
