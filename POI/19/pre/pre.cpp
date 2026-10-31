#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> hash_t;
#define A first
#define B second

const hash_t MOD(1e9 + 7, 1e9 + 9);
const hash_t BASE(26, 26);

ll add(ll a, ll b, ll m) {
	a += b; if(a >= m) a -= m;
	return a;
}

ll sub(ll a, ll b, ll m) {
	a -= b; if(a < 0) a += m;
	return a;
}

ll mult(ll a, ll b, ll m) {
	return a * b % m;
}

hash_t operator + (hash_t a, hash_t b) { return hash_t(add(a.A, b.A, MOD.A), add(a.B, b.B, MOD.B)); }
hash_t operator - (hash_t a, hash_t b) { return hash_t(sub(a.A, b.A, MOD.A), sub(a.B, b.B, MOD.B)); }
hash_t operator * (hash_t a, hash_t b) { return hash_t(mult(a.A, b.A, MOD.A), mult(a.B, b.B, MOD.B)); }

const int MAXN = 1e6 + 20;
int N;
char str[MAXN];

hash_t expb[MAXN];
hash_t pref[MAXN];

hash_t gethash(int a, int b) {
	return pref[b] - pref[a] * expb[b - a];
}

int main() {
	scanf("%d %s", &N, str);
	expb[0] = hash_t(1, 1);
	pref[0] = hash_t(0, 0);
	for(int i = 0; i < N; i++) {
		expb[i + 1] = expb[i] * BASE;
		pref[i + 1] = pref[i] * BASE + hash_t(str[i], str[i]);
	}

	int res = 0;
	for(int i = N / 2, l = 0; i >= 0; i--, l += 2) {
		while(2 * (i + l) > N) l--;
		while(gethash(i, i + l) != gethash(N - i - l, N - i)) l--;
		if(gethash(0, i) == gethash(N - i, N)) {
			res = max(res, i + l);
		}
	}
	printf("%d\n", res);
	return 0;
}
