#include<iostream>
#include<cassert>

using namespace std;

typedef long long ll;

const int MAXN = 6e5;
const int MAXA = 6e5;

int N;
ll M;
int A[MAXN];

ll numLower[MAXN];
ll numEqual[MAXN];

int bit[MAXA];
void insert(int v) {
	for(; v <= MAXA; v += v & (-v)) {
		bit[v - 1] ++;
	}
}
int query(int v) {
	int res = 0;
	for(; v > 0; v -= v & (-v)) {
		res += bit[v - 1];
	}
	return res;
}

ll exp(ll a, ll e, ll m) {
	ll res = 1;
	while(e) {
		if(e & 1) res *= a, res %= m;
		a *= a, a %= m;
		e /= 2;
	}
	return res;
}

// inverse mod q = p^e
ll inv(ll a, ll q, ll p) {
	return exp(a, q / p * (p - 1) - 1, q);
}

// solve mod q = p^e
ll solve(ll q, ll p) {
	ll res = 1;
	ll num = 1;
	ll pow = 0;
	for(int i = N - 1; i >= 0; i--) {
		ll v = numEqual[i];
		while(v % p == 0) v /= p, pow --;
		num *= inv(v, q, p);
		num %= q;
		v = numLower[i];
		for(int j = 0; j < -pow; j++) {
			assert(v % p == 0);
			v /= p;
		}
		for(int j = 0; v && j < pow; j++) {
			v *= p;
			v %= q;
		}
		res += num * v;
		res %= q;
		v = N - i;
		while(v % p == 0) v /= p, pow++;
		num *= v;
		num %= q;
	}
	return res;
}

ll crt(ll a, ll m, ll b, ll q, ll p) {
	ll mod = m * q;
	ll res = inv(m, q, p) * m * (b - a) + a;
	res %= mod;
	if(res < 0) res += mod;
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++) cin >> A[i];

	for(int i = N - 1; i >= 0; i--) {
		insert(A[i]);
		numLower[i] = query(A[i] - 1);
		numEqual[i] = query(A[i]) - numLower[i];
	}

	ll res = 0;
	ll mod = 1;

	for(ll p = 2; p * p <= M; p++) {
		if(M % p == 0) {
			ll q = 1;
			while(M % p == 0) {
				M /= p, q *= p;
			}
			res = crt(res, mod, solve(q, p), q, p);
			mod *= q;
		}
	}

	if(M > 1) {
		res = crt(res, mod, solve(M, M), M, M);
	}

	cout << res << '\n';
	return 0;
}
