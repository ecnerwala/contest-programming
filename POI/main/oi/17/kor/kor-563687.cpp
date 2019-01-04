#include<iostream>
#include<algorithm>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;

const ll BASE = 2e5 + 4;
//const ll MOD = 1e9 + 7;
const ll MOD1 = 1e9 + 7;
const ll MOD2 = 1e9 + 9;
const pii MOD(MOD1, MOD2);

ll inv(ll a, ll m) {
	if(a == 1) return 1;
	return m - (inv(m % a, a) * m) / a;
}

ll exp(ll a, ll b, ll m) {
	ll res = 1;
	while(b) {
		if(b & 1) res *= a; res %= m;
		b /= 2;
		a *= a; a %= m;
	}
	return res;
}

const int MAXN = 2e5 + 20;
int N;
ll A[MAXN];

pii pref[MAXN];
pii suff[MAXN];

ll mod(ll a, ll m) {
	a %= m;
	if(a < 0) a += m;
	return a;
}

pii operator +(pii a, pii b) {
	return pii(a.first + b.first, a.second + b.second);
}

pii operator +(pii a, ll b) {
	return a + pii(b, b);
}

pii operator -(pii a, pii b) {
	return pii(a.first - b.first, a.second - b.second);
}

pii operator *(pii a, pii b) {
	return pii(a.first * b.first, a.second * b.second);
}

pii operator *(pii a, ll b) {
	return a * pii(b, b);
}

pii operator *(ll a, pii b) {
	return b * a;
}

pii operator %(pii a, pii b) {
	return pii(mod(a.first, b.first), mod(a.second, b.second));
}

pii val[MAXN];

int maxVal;
int L;
int Kcnd[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(ll i = 0; i < N; i++) {
		cin >> A[i];
		A[i] --;
	}

	pref[0] = pii(0, 0);
	for(ll i = 0; i < N; i++) {
		pref[i + 1] = (BASE * pref[i] + A[i]) % MOD;
	}
	suff[N] = pii(0, 0);
	for(ll i = N; i > 0; i--) {
		suff[i - 1] = (BASE * suff[i] + A[i - 1]) % MOD;
	}

	maxVal = 0;
	L = 0;
	pii mult(1, 1);
	for(int k = 1; k <= N; k++) {
		mult = BASE * mult % MOD;
		int sz = N / k;
		for(ll i = 0, j = 0; i < sz; i++, j += k) {
			pii v1 = (pref[j + k] - pref[j] * mult) % MOD;
			pii v2 = (suff[j] - suff[j + k] * mult) % MOD;
			val[i] = min(v1, v2);
		}

		sort(val, val + sz);
		int cnd = unique(val, val + sz) - val;
		if(cnd > maxVal) {
			maxVal = cnd;
			L = 0;
		}
		if(cnd == maxVal) {
			Kcnd[L++] = k;
		}
	}
	cout << maxVal << ' ' << L << '\n';
	cout << Kcnd[0];
	for(int i = 1; i < L; i++) {
		cout << ' ' << Kcnd[i];
	}
	cout << '\n';
	return 0;
}
