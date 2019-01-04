#include<bits/stdc++.h>
using namespace std;

const int MAXN = 5.1e5;
int N;
vector<int> adj[MAXN];
int par[MAXN];

struct state {
	int all, here;
} dpdown[MAXN], dpup[MAXN];

void dfsups(int cur, int prv = 0) {
	dpup[cur].all = 0;
	dpup[cur].here = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfsups(nxt, cur);

		dpup[cur].all = max(dpup[cur].all, dpup[nxt].all);

		int nh = dpup[nxt].here + 1;
		dpup[cur].all = max(dpup[cur].all, dpup[cur].here + nh);
		dpup[cur].here = max(dpup[cur].here, nh);
	}
}

void dfsdowns(int cur, int prv = 0) {
	int all0 = 0, all1 = 0; // best alls
	int here0 = 0, here1 = 0, here2 = 0; // best heres
	for (int nxt : adj[cur]) {
		state s = (nxt == prv) ? dpdown[cur] : dpup[nxt];
		s.here ++;

		if (s.all > all0) {
			all1 = all0;
			all0 = s.all;
		} else if (s.all > all1) {
			all1 = s.all;
		}

		if (s.here > here0) {
			here2 = here1;
			here1 = here0;
			here0 = s.here;
		} else if (s.here > here1) {
			here2 = here1;
			here1 = s.here;
		} else if (s.here > here2) {
			here2 = s.here;
		}

		assert(all0 >= all1);
		assert(here0 >= here1 && here1 >= here2);
	}

	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		state s = dpup[nxt];
		s.here ++;
		if (s.all == all0) {
			dpdown[nxt].all = all1;
		} else {
			dpdown[nxt].all = all0;
		}

		if (s.here == here0) {
			dpdown[nxt].all = max(dpdown[nxt].all, here1 + here2);
		} else if (s.here == here1) {
			dpdown[nxt].all = max(dpdown[nxt].all, here0 + here2);
		} else {
			dpdown[nxt].all = max(dpdown[nxt].all, here0 + here1);
		}

		if (s.here == here0) {
			dpdown[nxt].here = here1;
		} else {
			dpdown[nxt].here = here0;
		}

		dfsdowns(nxt, cur);
	}
}

int bestopt, optcur, optnxt, opta, optb;
int bestpes, pescur, pesnxt, pesa, pesb;

void dfsgo(int cur, int prv = 0) {
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		int nu = dpup[nxt].all, nd = dpdown[nxt].all;
		//cerr << cur << "->" << nxt << ' ' << nu << '\n';
		//cerr << nxt << "->" << cur << ' ' << nd << '\n';

		int opt = (nu + 1) / 2 + (nd + 1) / 2 + 1;
		opt = max(opt, max(nu, nd));
		//cerr << cur << ' ' << nxt << ' ' << opt << '\n';
		if (opt < bestopt) {
			bestopt = opt;
			optcur = cur;
			optnxt = nxt;
		}

		int pes = nu + nd + 1;
		//cerr << cur << ' ' << nxt << ' ' << pes << '\n';
		if (pes > bestpes) {
			bestpes = pes;
			pescur = cur;
			pesnxt = nxt;
		}

		dfsgo(nxt, cur);
	}
}

void remove(vector<int>& v, int a) {
	for (auto it = v.begin(); it != v.end(); it++) {
		if (*it == a) {
			v.erase(it);
			return;
		}
	}
	assert(false);
}

void addEdge(int a, int b) {
	adj[a].push_back(b);
	adj[b].push_back(a);
}
void removeEdge(int a, int b) {
	remove(adj[a], b);
	remove(adj[b], a);
}

pair<int, pair<int, int>> dfsfurthest(int cur, int prv = 0, int d = 0) {
	pair<int, pair<int, int>> res(d, pair<int, int>(cur, 0));
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		pair<int, pair<int, int>> cnd = dfsfurthest(nxt, cur, d + 1);
		res = max(res, cnd);
	}
	if (d == res.first / 2) {
		res.second.second = cur;
	}
	return res;
}

int getFurthest(int a) {
	return dfsfurthest(a).second.first;
}

int getCenter(int a) {
	a = getFurthest(a);
	return dfsfurthest(a).second.second;
}

int main() {
	(void) scanf("%d", &N);
	assert(N >= 2);
	for (int i = 0; i < N-1; i++) {
		int a, b; (void) scanf("%d %d", &a, &b);
		addEdge(a, b);
	}
	dfsups(1);
	dfsdowns(1);

	bestopt = N+1, optcur = 0, optnxt = 0;
	bestpes = -1, pescur = 0, pesnxt = 0;

	dfsgo(1);

	// take opt
	{
		removeEdge(optcur, optnxt);
		opta = getCenter(optcur);
		optb = getCenter(optnxt);
		addEdge(optcur, optnxt);
	}

	// take pes
	{
		removeEdge(pescur, pesnxt);
		pesa = getFurthest(pescur);
		pesb = getFurthest(pesnxt);
		addEdge(pescur, pesnxt);
	}

	printf("%d %d %d %d %d\n", bestopt, optcur, optnxt, opta, optb);
	printf("%d %d %d %d %d\n", bestpes, pescur, pesnxt, pesa, pesb);

	return 0;
}
