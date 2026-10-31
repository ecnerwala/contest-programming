#include<bits/stdc++.h>
using namespace std;

const int S = 1 << 17;
int N, Q;

const int MAXL = 22;
int L;
string P;
int failure[MAXL][26];

struct seg_node {
	int lazy[MAXL];
	int cnt[MAXL];
} seg[2*S];

void applyUpdate(int n, const int v[MAXL]) {
	for (int i = 0; i <= L; i++) {
		seg[n].lazy[i] = v[seg[n].lazy[i]];
	}
	int nxtCnt[MAXL];
	for (int i = 0; i <= L; i++) nxtCnt[i] = 0;
	for (int i = 0; i <= L; i++) {
		nxtCnt[v[i]] += seg[n].cnt[i];
	}
	for (int i = 0; i <= L; i++) {
		seg[n].cnt[i] = nxtCnt[i];
	}
}

void downdateNode(int n) {
	if (n < S) {
		applyUpdate(2*n, seg[n].lazy);
		applyUpdate(2*n+1, seg[n].lazy);
	}
	for (int i = 0; i <= L; i++) {
		seg[n].lazy[i] = i;
	}
}

void updateNode(int n) {
	if (n >= S) {
		return;
	}
	for (int i = 0; i <= L; i++) {
		seg[n].cnt[i] = 0;
	}
	for (int i = 0; i <= L; i++) {
		seg[n].cnt[seg[n].lazy[i]] += (seg[2*n].cnt[i] + seg[2*n+1].cnt[i]);
	}
}

void downdateAll(int n) {
	if (!n) return;
	downdateAll(n/2);
	downdateNode(n);
}

void updateAll(int n) {
	if (!n) return;
	updateNode(n);
	updateAll(n/2);
}

void applyRange(int l, int r, const int v[MAXL]) {
	assert(l <= r);
	downdateAll(S+l);
	downdateAll(S+r);
	for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			applyUpdate(a++, v);
		}
		if (b & 1) {
			applyUpdate(--b, v);
		}
	}
	updateAll(S+l);
	updateAll(S+r);
}

int queryRange(int l, int r) {
	assert(l <= r);
	downdateAll(S+l);
	downdateAll(S+r);
	int ans = 0;
	for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			ans += seg[a++].cnt[L];
		}
		if (b & 1) {
			ans += seg[--b].cnt[L];
		}
	}
	return ans;
}

void init() {
	for (int n = 1; n < 2 * S; n++) {
		for (int v = 0; v <= L; v++) {
			seg[n].lazy[v] = v;
		}
	}
	for (int i = 0; i < N; i++) {
		seg[S+i].cnt[0] = 1;
	}
	for (int n = S-1; n; n--) updateNode(n);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> P;
	L = int(P.size());
	for (int i = 0; i < L; i++) {
		int f = i+1;
		swap(f, failure[i][P[i] - 'a']);
		memcpy(failure[i+1], failure[f], sizeof(failure[i+1]));
	}

	cin >> N >> Q;

	init();
	while (Q--) {
		int t, l, r; cin >> t >> l >> r;
		l--, r--;
		if (t == 1) {
			string s; cin >> s;
			int v[MAXL];
			for (int i = 0; i <= L; i++) {
				int cur = i;
				for (char c : s) {
					cur = failure[cur][c-'a'];
				}
				v[i]=cur;
			}
			applyRange(l,r,v);
		} else if (t == 2) {
			cout << queryRange(l, r) << '\n';
		} else assert(false);
	}

	return 0;
}
