#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> hash;
#define A first
#define B second

const hash MOD(1e9 + 7, 1e9 + 9);
const hash BASE(26, 26);

inline ll sub(ll a, ll b, ll m) {
	a -= b; if(a < 0) a += m;
	return a;
}

inline ll add(ll a, ll b, ll m) {
	a += b; if(a >= m) a -= m;
	return a;
}

inline ll mult(ll a, ll b, ll m) {
	return a * b % m;
}

inline ll inv(ll a, ll m) {
	return m - inv(m % a, a) * m / a;
}

inline hash inv(hash a) {
	return hash(inv(a.A, MOD.A), inv(a.B, MOD.B));
}

inline hash operator + (hash a, hash b) {
	return hash(add(a.A, b.A, MOD.A), add(a.B, b.B, MOD.B));
}

inline hash operator - (hash a, hash b) {
	return hash(sub(a.A, b.A, MOD.A), sub(a.B, b.B, MOD.B));
}

inline hash operator * (hash a, hash b) {
	return hash(mult(a.A, b.A, MOD.A), mult(a.B, b.B, MOD.B));
}

const int MAXN = 1e6;
int N;
char S[MAXN];

hash expb[MAXN];
hash pref[MAXN];

// 0-indexed inclusive/exclusive, 1-indexed exclusive/inclusive
inline hash gethash(int a, int b) {
	//assert(b >= a);
	return pref[b] - pref[a] * expb[b - a];
}

inline bool isperiod(int a, int b, int l) {
	//assert((b - a) % l == 0);
	return gethash(a, b - l) == gethash(a + l, b);
}

int fac[MAXN];

int main() {
	scanf("%d %s", &N, S);
	expb[0] = hash(1, 1);
	pref[0] = hash(0, 0);
	for(int i = 1; i <= N; i++) {
		expb[i] = expb[i - 1] * BASE;
		pref[i] = pref[i - 1] * BASE + hash(S[i - 1], S[i - 1]);
	}

	for(int i = 1; i <= N; i++) fac[i] = i;
	for(int i = 2; i <= N; i++) {
		if(fac[i] == i) {
			for(int j = i + i; j <= N; j+=i) fac[j] = i;
		}
	}

	int Q; scanf("%d", &Q);
	for(int q = 0; q < Q; q++) {
		int a, b; scanf("%d %d", &a, &b); a--;
		int l = b - a;
		for(int v = l; v > 1; v /= fac[v]) {
			int p = fac[v];
			if(isperiod(a, b, l / p)) l /= p;
		}
		printf("%d\n", l);
	}
	return 0;
}
