#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e5) + 200;
int N[2];

const int MAXL = MAXN * 2 + 200;
int L;
int X[MAXL];

const int MAXT = 150;
int T;
int val_cnt[MAXT];

int lval[MAXL]; // value stored on left
int rval[MAXL]; // value stored on right

const int MAXV = MAXL + 200;
int V;
int pref_cnt[MAXV];

const int MAXP = MAXL + 200;
pair<int, int> pts[MAXP]; // stores first and last index
pair<int, int> pts2[MAXP]; // stores first and last index
int nval[MAXP];

bool go() {
	scanf("%d %d", &N[0], &N[1]);
	L = 0;
	X[0] = 0;
	for (int z = 0; z < 2; z++) {
		if (z) X[++L] = 0;
		for (int i = 1; i <= N[z]; i++) {
			scanf("%d", &X[++L]);
			assert(X[L] > 0);
		}
	}
	X[L+1] = 0;

	memset(val_cnt, 0, sizeof(val_cnt));
	for (int i = 1; i <= L; i++) {
		assert(0 <= X[i] && X[i] < MAXT);
		val_cnt[X[i]] = true;
	}
	assert(val_cnt[0]);
	T = 0;
	for (int i = 0; i < MAXT; i++) T += bool(val_cnt[i]);

	V = 0;
	// initialize lval and rval for i = 1
	for (int i = 1; i <= L; i++) {
		lval[i] = rval[i] = X[i];
		V = max(V, lval[i] + 1);
	}

	for (int t = 2; t <= T-1; t++) {
		int P = 0;
		memset(val_cnt, 0, sizeof(val_cnt));
		for (int num_objs = 0, l = 1, r = 0; true;) {
			while (r < L && (num_objs < t || val_cnt[X[r+1]] > 0)) {
				r++;
				if (val_cnt[X[r]] == 0) num_objs ++;
				val_cnt[X[r]] ++;
			}
			if (num_objs < t) break;
			pts[P++] = pair<int, int>(l,r);
			//cerr << l << ' ' << r << '\n';
			while (l <= r && num_objs == t) {
				val_cnt[X[l]] --;
				if (val_cnt[X[l]] == 0) num_objs --;
				l++;
			}
			assert(num_objs < t);
		}
		assert(P);
		if (t == T-1) {
			if (pts[0] != pair<int, int>(1, N[0])) return false;
			if (pts[P-1] != pair<int, int>(N[0]+2, L)) return false;
		}

		for (int i = 0; i + 1 < P; i++) {
			assert(pts[i].first < pts[i+1].first);
			assert(pts[i].second < pts[i+1].second);
		}

		// now we bucketsort
		for (int i = 0; i <= V+1; i++) pref_cnt[i] = 0;
		for (int i = 0; i < P; i++) {
			nval[i] = rval[pts[i].second];
			assert(nval[i] != -1);
			assert(nval[i] < V);
			pref_cnt[nval[i] + 1] ++;
		}
		for (int i = 1; i <= V; i++) {
			pref_cnt[i+1] += pref_cnt[i];
		}
		assert(pref_cnt[V+1] == P);
		// do the transfer
		for (int i = 0; i < P; i++) {
			pts2[pref_cnt[nval[i]] ++] = pts[i];
		}
		assert(pref_cnt[V] == P);

		// repeat on the first element
		for (int i = 0; i <= V+1; i++) pref_cnt[i] = 0;
		for (int i = 0; i < P; i++) {
			nval[i] = lval[pts2[i].first];
			assert(nval[i] != -1);
			assert(nval[i] < V);
			pref_cnt[nval[i] + 1] ++;
		}
		for (int i = 1; i <= V; i++) {
			pref_cnt[i+1] += pref_cnt[i];
		}
		assert(pref_cnt[V+1] == P);
		// do the transfer
		for (int i = 0; i < P; i++) {
			pts[pref_cnt[nval[i]] ++] = pts2[i];
		}
		assert(pref_cnt[V] == P);

		V = 0;
		pair<int, int> prv(-1, -1);
		for (int i = 0; i < P; i++) {
			pair<int, int> cur(lval[pts[i].first], rval[pts[i].second]);
			if (cur != prv) {
				prv = cur;
				V++;
			}
			assert(V);
			nval[i] = V-1;
		}

		//memset(lval, -1, sizeof(lval));
		//memset(rval, -1, sizeof(rval));
		for (int i = 0; i < P; i++) {
			lval[pts[i].first] = rval[pts[i].second] = nval[i];
		}

		/*
		for (int i = 1; i <= L; i++) {
			if (lval[i] == -1) cerr << '-' << ' ';
			else cerr << lval[i] << ' ';
		}
		cerr << '\n';
		for (int i = 1; i <= L; i++) {
			if (rval[i] == -1) cerr << '-' << ' ';
			else cerr << rval[i] << ' ';
		}
		cerr << '\n';
		*/

		if (t == T-1) {
			return lval[1] == rval[L];
		}
	}
	assert(false);
	return false;
}

int main() {
	int Q; scanf("%d", &Q);
	while(Q--) {
		bool res = go();
		printf("%d\n", int(res));
	}
	return 0;
}
