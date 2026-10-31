#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 1.1e6;
int N;
int R[MAXN*2];
int nxt[MAXN * 2];
int prv[MAXN * 2];

const int MAXK = MAXN;
int K;
int lastColor[MAXK];

struct minset {
	int cnt;
	int last;
	minset& operator += (int v) {
		if (v) {
			cnt = 0;
			last = -1;
		}
		return *this;
	}
};
minset combine(const minset& l, const minset& r) {
	return minset{ l.cnt + r.cnt, r.cnt ? r.last : l.last };
}

const int S = 1 << 21;
struct data {
	int lazy;
	minset min;
} seg[2 * S];

void recompute(int i) {
	if (seg[i].lazy) {
		seg[i].min = {0, -1};
	} else if (i >= S) {
		seg[i].min = {1, i-S};
	} else {
		seg[i].min = combine(seg[2*i].min, seg[2*i+1].min);
	}
}

void init() {
	for (int i = 2*S-1; i > 0; i--) {
		seg[i].lazy = 0;
		recompute(i);
	}
}
void update(int l, int r, int v) {
	//cerr << "Update " << l << ' ' << r << ' ' << v << '\n';
	bool lhas = false;
	bool rhas = false;
	for (l += S, r += S; l && r; l /= 2, r /= 2) {
		if (lhas) {
			//cerr << "recompute " << l-1 << '\n';
			recompute(l-1);
		}
		if (rhas) {
			//cerr << "recompute " << r << '\n';
			recompute(r);
		}
		if (l & 1) {
			if (l < r) {
				//cerr << "lupdate " << l << '\n';
				seg[l].lazy += v;
				assert(seg[l].lazy >= 0);
				recompute(l);
				lhas = true;
			}
			l++;
		}
		if (r & 1) {
			r--;
			if (l <= r) {
				//cerr << "rupdate " << r << '\n';
				seg[r].lazy += v;
				assert(seg[r].lazy >= 0);
				recompute(r);
				rhas = true;
			}
		}
	}
}
minset query(int l, int r) {
	//cerr << "Query " << l << ' ' << r << '\n';
	assert(l <= r);
	assert(l < r);
	bool lhas = false; minset lres;
	bool rhas = false; minset rres;

	for (l += S, r += S; l && r; l /= 2, r /= 2) {
		if (lhas) {
			lres += seg[l-1].lazy;
		}
		if (l & 1) {
			if (l < r) {
				lres = lhas ? combine(lres, seg[l].min) : seg[l].min;
				lhas = true;
			}
			l++;
		}
		if (rhas) {
			rres += seg[r].lazy;
		}
		if (r & 1) {
			r--;
			if (l <= r) {
				rres = rhas ? combine(seg[r].min, rres) : seg[r].min;
				rhas = true;
			}
		}
	}
	assert(lhas || rhas);

	if (lhas && rhas) return combine(lres, rres);
	else if (lhas) return lres;
	else if (rhas) return rres;
	else assert(false);
}

int main() {
	(void) scanf("%d %d", &N, &K);
	for (int i = 0; i < N; i++) {
		(void) scanf("%d", &R[i]);
		R[i] --;
		R[i+N] = R[i];
	}

	memset(lastColor, -1, sizeof(lastColor));
	for (int i = 0; i < 2 * N; i++) {
		nxt[i] = 2 * N;
	}
	for (int i = 0; i < 2 * N; i++) {
		prv[i] = lastColor[R[i]];
		if (prv[i] >= 0) nxt[prv[i]] = i;
		lastColor[R[i]] = i;
	}
//

	for (int i = 0; i < 2 * N; i++) {
		//cerr << i << ' ' << R[i] << ' ' << prv[i] << ' ' << nxt[i] << '\n';
	}
	// first setup the segtree
	init();
	for (int i = 0; i < N; i++) {
		if (nxt[i] < N) update(i, nxt[i], 1);
	}
	int64_t totcnt = 0;
	int bestmid = 0;
	for (int i = 0; i < N; i++) {
		assert(N >= 2);
		minset ms = query(i, i + N / 2);
		//cerr << i << ' ' << ms.v << ' ' << ms.cnt << ' ' << ms.last << '\n';
		if (ms.cnt > 0) {
			int last = ms.last - i + 1;
			assert(last * 2 <= N);
			totcnt += 2 * ms.cnt - (last * 2 == N);
			bestmid = max(bestmid, last);
		}
		if (i + 1 < N) {
			// remove and update this thing
			if (nxt[i] < i + N) {
				update(i, nxt[i], -1);
			}
			if (prv[i + N] > i) {
				assert(nxt[prv[i+N]] == i+N);
				update(prv[i + N], i + N, 1);
			}
		}
	}

	assert(totcnt % 2 == 0);
	totcnt /= 2;
	printf("%" PRId64 " %d\n", totcnt, (N - bestmid) - bestmid);

	return 0;
}
