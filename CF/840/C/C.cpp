#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = ll(1e9) + 7;

ll inv(ll a, ll m) {
	return (a == 1) ? 1 : m - inv(m % a, a) * m / a;
}

const int MAXN = 400;
ll ival[MAXN];
ll fact[MAXN];
ll ifact[MAXN];

int N;
int A[MAXN];

ll choose(int n, int r) {
	if (r < 0 || r > n) return 0;
	assert(n < MAXN);
	return fact[n] * ifact[r] % MOD * ifact[n - r] % MOD;
}

int main() {
	for (int i = 0; i < MAXN; i++) {
		ival[i] = inv(i, MOD);
	}
	fact[0] = ifact[0] = 1;
	for (int i = 1; i < MAXN; i++) {
		fact[i] = fact[i-1] * i % MOD;
		ifact[i] = ifact[i-1] * ival[i] % MOD;
	}

	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
	}
	sort(A, A + N);
	ll mult = 1;
	for (int i = 0; i < N; i++) {
		if (i && A[i] == A[i-1]) continue;
		int cnt = 0;
		for (int j = i; j < N && A[i] == A[j]; j++) cnt++;
		mult *= ifact[cnt]; mult %= MOD;
	}
	for (int i = 0; i < N; i++) {
		for (int v = 2; v * v <= A[i]; v++) {
			while (A[i] % (v * v) == 0) {
				A[i] /= v * v;
			}
		}
	}
	sort(A, A + N);
	vector<int> cnts;
	for (int i = 0; i < N; i++) {
		if (i && A[i] == A[i-1]) continue;
		int cnt = 0;
		for (int j = i; j < N && A[i] == A[j]; j++) cnt++;
		mult *= fact[cnt]; mult %= MOD;
		cnts.push_back(cnt);
	}
	sort(cnts.begin(), cnts.end());
	reverse(cnts.begin(), cnts.end());
	for (int cnt : cnts) {
	}

	return 0;
}
