#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6;
const int MAXM = 1e6;

int N, M;
int P[MAXN];

long long S[MAXN];

int Q[MAXN];
int st[MAXN];

inline int cnt(int v, int a, int b) {
	int *l = Q + st[v];
	int *r = Q + st[v + 1];
	return lower_bound(l, r, b) - lower_bound(l, r, a);
}

inline bool is_majority(int v, int a, int b) {
	return cnt(v, a, b) * 2 > (b - a);
}

const int L = 1 << 19;
int seg[MAXN * 2];

int build_tree(int i = 1) {
	if(i >= L) {
		seg[i] = P[i - L];
	} else {
		assert(i < L);
		int l = i, r = i + 1;
		while(l < L) l *= 2, r *= 2;

		l -= L, r -= L;

		seg[i] = -1;

		for(int t = 0; t < 2; t++) {
			int v = build_tree(2 * i + t);
			if(v >= 0 && is_majority(v, l, r)) {
				seg[i] = v;
			}
		}
	}

	return seg[i];
}

// inlcusive
int query(int a, int b) {

	b++;

	for(int l = a + L, r = b + L - 1; l <= r; l /= 2, r /= 2) {
		if(l % 2 == 1) {
			if(is_majority(seg[l], a, b)) return seg[l];
			l ++;
		}
		if(r % 2 == 0) {
			if(is_majority(seg[r], a, b)) return seg[r];
			r--;
		}
	}

	return -1;
}

int main() {

	scanf("%d%d", &N, &M);
	for(int i = 0; i < N; i++) {
		scanf("%d", &P[i]);
		P[i] --;
	}

	N = L;

	for(int i = 0; i < N; i++) {
		S[i] = ((long long)(P[i])) * N + i;
	}

	sort(S, S + N);

	for(int i = 0; i < N; i++) {
		Q[i] = S[i] % N;
		S[i] /= N;
	}

	for(int v = 0; v < N; v++) {
		int i = st[v];
		while(i < N && S[i] == v) i++;
		st[v + 1] = i;
	}

	build_tree();

	for(int i = 0; i < M; i++) {
		int a, b; scanf("%d%d", &a, &b);
		a--, b--;
		printf("%d\n", query(a, b) + 1);
	}
	return 0;
}
