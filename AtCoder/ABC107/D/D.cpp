#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
int A[MAXN];

int bit[MAXN*2];
void reset() {
	for (int i = 0; i <= 2*N; i++) {
		bit[i] = 0;
	}
}
int query(int a) {
	assert(a <= 2 * N);
	int res = 0;
	for (; a; a -= a & (-a)) {
		res += bit[a];
	}
	return res;
}
void update(int a) {
	assert(a <= 2 * N);
	a++;
	for (; a <= 2 * N; a += a & (-a)) {
		bit[a] ++;
	}
}

ll go(ll v) {
	// we want to count # sequences where median is < v
	// # sequences where (#<v) is > (#>=v)
	ll res = 0;
	int pref = N;
	reset();
	update(pref);
	for (int i = 0; i < N; i++) {
		if (A[i] < v) {
			pref ++;
		} else {
			pref --;
		}
		assert(pref >= 0);
		res += query(pref);
		update(pref);
	}
	return res;
}

int M;
int V[MAXN];
int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	ll T = ll(N) * ll(N + 1) / 2;
	for (int i = 0; i < N; i++) {
		V[i] = A[i];
	}
	M = N;
	sort(V, V + M);
	M = int(unique(V, V + M) - V);
	int mi = 0, ma = M;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (go(V[md]) * 2 <= T) {
			mi = md;
		} else {
			ma = md;
		}
	}
	cout << V[mi] << '\n';

	return 0;
}
