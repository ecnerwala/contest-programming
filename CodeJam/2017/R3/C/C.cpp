#include<bits/stdc++.h>
using namespace std;

const int MOD = 24;

const int MAXC = 2000;
const int MAXN = 2 * MAXC;

int C;
int N;
struct path {
	int S, E, L, D; // l is leaves, d is duration->arrives
} P[MAXN];

struct ds {
	int numC;
	int par[MAXN];
	void reset() {
		numC = N;
		for (int i = 0; i < N; i++) {
			par[i] = i;
		}
	}
	int getpar(int i) {
		return par[i] == i ? i : (par[i] = getpar(par[i]));
	}
	bool merge(int i, int j) {
		i = getpar(i), j = getpar(j);
		if (i == j) return false;
		par[i] = j;
		numC --;
		return true;
	}
};

int tdiff (int a, int b) {
	return (b - a + MOD) % MOD;
}

int go() {
	int res = 0;
	vector<path*> nxt[MAXC];
	vector<path*> prv[MAXC];
	for (int i = 0; i < N; i++) {
		nxt[P[i].S].push_back(&P[i]);
		prv[P[i].E].push_back(&P[i]);
		res += P[i].D;
		P[i].D += P[i].L;
		P[i].D %= MOD;
	}

	ds ds;
	ds.reset();

	typedef pair<int, int> pii;
	priority_queue<pair<int, pii>> pq;

	for (int i = 0; i < C; i++) {
		assert(nxt[i].size() == 2);
		assert(prv[i].size() == 2);
		int a = tdiff(prv[i][0]->D, nxt[i][0]->L) + tdiff(prv[i][1]->D, nxt[i][1]->L);
		int b = tdiff(prv[i][0]->D, nxt[i][1]->L) + tdiff(prv[i][1]->D, nxt[i][0]->L);
		if (i == 0) {
			// special case because we start at 0
			a = min(
					tdiff(0, nxt[i][0]->L) + tdiff(prv[i][1]->D, nxt[i][1]->L),
					tdiff(0, nxt[i][1]->L) + tdiff(prv[i][0]->D, nxt[i][0]->L));
			b = min(
					tdiff(0, nxt[i][1]->L) + tdiff(prv[i][1]->D, nxt[i][0]->L),
					tdiff(0, nxt[i][0]->L) + tdiff(prv[i][0]->D, nxt[i][1]->L));
		}
		if (a > b) {
			swap(nxt[i][0], nxt[i][1]);
			swap(a, b);
		}

		assert(a <= b);

		//cerr << "node " << i+1 << ": " << a << " vs " << b << '\n';
		//cerr << int(prv[i][0]-P) << "->" << int(nxt[i][0] - P) << '\n';
		//cerr << int(prv[i][1]-P) << "->" << int(nxt[i][1] - P) << '\n';
		ds.merge(int(prv[i][0] - P), int(nxt[i][0] - P));
		ds.merge(int(prv[i][1] - P), int(nxt[i][1] - P));

		res += a;

		pii e(int(prv[i][0] - P), int(prv[i][1] - P));
		//cerr << "edge " << a-b << ' ' << e.first << ' ' << e.second << '\n';
		pq.emplace(a - b, e);
	}

	//cerr << "distinct cycles: " << ' ' << ds.numC << '\n';

	while (ds.numC > 1) {
		assert(!pq.empty());
		auto it = pq.top(); pq.pop();
		if(ds.merge(it.second.first, it.second.second)) {
			res -= it.first;
		}
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> C;
		N = 2 * C;
		for (int i = 0; i < N; i++) {
			cin >> P[i].E >> P[i].L >> P[i].D;
			P[i].S = i / 2;
			P[i].E --;
			assert(P[i].S != P[i].E);
		}
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
