#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> hash;
#define A first
#define B second

const hash MOD(1e9 + 7, 1e9 + 9);
const hash BASE(26, 26);

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

hash operator + (hash a, hash b) { return hash(add(a.A, b.A, MOD.A), add(a.B, b.B, MOD.B)); }
hash operator - (hash a, hash b) { return hash(sub(a.A, b.A, MOD.A), sub(a.B, b.B, MOD.B)); }
hash operator * (hash a, hash b) { return hash(mult(a.A, b.A, MOD.A), mult(a.B, b.B, MOD.B)); }

const int MAXN = 1e6 + 20;
int N;
char str[MAXN];

hash expb[MAXN];
hash pref[MAXN];

hash gethash(int a, int b) {
	return pref[b] - pref[a] * expb[b - a];
}

int main() {
	scanf("%d %s", &N, str);
	expb[0] = hash(1, 1);
	pref[0] = hash(0, 0);
	for(int i = 0; i < N; i++) {
		expb[i + 1] = expb[i] * BASE;
		pref[i + 1] = pref[i] * BASE + hash(str[i], str[i]);
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
