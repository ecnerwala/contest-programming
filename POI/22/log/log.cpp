#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXP = 1.1e6;
int P;
int L[MAXP]; // license value

const int MAXQ = 1.1e6;
int Q;
struct query {
	char t;
	int n;
	int val;
} queries[MAXQ];

const int MAXV = MAXQ;
int V;
int vals[MAXV];

struct data {
	int cnt;
	ll sum;
} bit[MAXV + 2];

void update(int a, int vc, ll vs) {
	assert(0 <= a && a < V);
	a ++;
	for (int i = a; i <= V; i += i & (-i)) {
		bit[i].cnt += vc;
		bit[i].sum += vs;
	}
}

// query strictly less than a
data query(int a) {
	assert(0 <= a && a <= V);
	data res{0, 0};
	for (int i = a; i > 0; i -= i & (-i)) {
		res.cnt += bit[i].cnt;
		res.sum += bit[i].sum;
	}
	return res;
}

int main() {
	(void) scanf("%d %d", &P, &Q);
	for (int i = 0; i < Q; i++) {
		(void) scanf(" %c %d %d", &queries[i].t, &queries[i].n, &queries[i].val);
	}

	for (int i = 0; i < Q; i++) {
		vals[i] = queries[i].val;
	}
	vals[Q] = 0;
	V = Q+1;
	sort(vals, vals + V);
	V = int(unique(vals, vals + V) - vals);
	assert(vals[0] == 0);

	for (int i = 0; i < Q; i++) {
		char t = queries[i].t;
		int n = queries[i].n;
		int val = queries[i].val;
		int ind = int(lower_bound(vals, vals + V, val) - vals);
		if (t == 'U') {
			assert(1 <= n && n <= P);
			int pval = L[n];
			int pind = int(lower_bound(vals, vals + V, pval) - vals);
			update(pind, 1, -pval);

			L[n] = val;
			update(ind, -1, val);
		} else if (t == 'Z') {
			data pref = query(ind); // everything strictly less than ind
			n -= min(n, pref.cnt); // all drivers that can drive the whole shift should
			ll tot = n;
			tot *= val;
			tot -= min(tot, pref.sum); // remove all hours
			(void) printf(tot == 0 ? "TAK\n" : "NIE\n");
		} else assert(false);
	}

	return 0;
}
