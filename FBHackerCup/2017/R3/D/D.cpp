#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 100;
const int MAXK = MAXN;
int N, K;
int L[MAXN];

ll go() {
	assert(1 <= N && N < MAXN);
	assert(0 <= K && K <= N);

	int NZ = 0, NO = 0;
	for (int i = 0; i < N; i++) {
		if (L[i] == 0) NZ ++;
		else if (L[i] == 1) NO ++;
		else assert(false);
	}
	assert(NZ + NO == N);
	assert(K >= NO);
	K -= NO;
	assert(0 <= K && K <= NZ);
	if (K == 0 || K == NZ) {
		return 0;
	}
	assert(0 < K && K < NZ);
	assert(NZ >= 2);

	assert(N >= 2);
	if (L[0] == 0 && L[1] == 0) {
		return -1;
	}

	if (K == NZ - 1) {
		for (int i = 0; i + 1 < N; i++) {
			if (L[i] == 0 && L[i + 1] == 0) {
				ll res = 1;
				res += 1ll << (N - 1 - (i + 1));
				return res;
			}
		}
		return 1;
	}
	int fz = 0;
	while (L[fz] != 0) fz++, assert(fz < N);
	assert(fz < N && L[fz] == 0);
	assert(fz + 1 < N);

	if (L[fz + 1] == 1) {
		L[fz] = 1; // do this as a dummy
		int nz = fz + 1;
		while (L[nz] != 0) nz++;
		if (nz + 1 == N || L[nz + 1] == 1) {
			L[nz] = 1;
			K--;
			if (K > 0) K--;
		} else {
			while (nz + 1 < N && L[nz + 1] == 0) nz++;
			L[nz] = 1;
			K--;
		}
		for (int i = N - 1, k = K; k > 0; i--) {
			if (L[i] != 0) continue;
			L[i] = 1;
			k--;
		}
	} else {
		int clz = 1;
		while (fz + 1 < N && L[fz + 1] == 0) {
			fz ++;
			clz++;
		}
		assert(fz >= 2);
		assert(L[fz - 1] == 0);
		assert(clz >= 2);
		if (K - 1 >= NZ - clz) {
			// 1011111xxxx1111111....
			for (int i = N - 1, k = K; k >= 0; i--) {
				if (L[i] != 0) continue;
				if (k != 1) {
					L[i] = 1;
				}
				k--;
			}
		} else {
			// 10xxxxxxxx0001111111...
			L[fz - 1] = 1;
			K--;
			for (int i = N - 1, k = K; k > 0; i--) {
				if (L[i] != 0) continue;
				L[i] = 1;
				k--;
			}
		}
	}

	ll res = 1;
	for (int i = N - 1; i >= 0; i--) {
		if (L[i] != 0) continue;
		res += (1ll << (N - 1 - i));
	}
	return res;
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N >> K;
		assert(N < MAXN);
		assert(1 <= N);
		assert(0 <= K && K <= N);
		for (int i = 0; i < N; i++) {
			cin >> L[i];
			assert(L[i] == 0 || L[i] == 1);
		}
		ll res = go();
		cout << "Case #" << T << ": " << res << '\n';
	}

	return 0;
}
