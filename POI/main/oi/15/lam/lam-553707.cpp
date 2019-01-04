#include<iostream>
#include<cstdio>
#include<cstring>

using namespace std;

typedef long long ll;

const int MAXN = 2e3;

struct bignum {
	static const ll BASE = 1e9;
	static const int D = MAXN;
	int sz;
	ll val[D];

	bignum& operator = (ll v) {
		memset(val, 0, sizeof(val));
		val[0] = v;
		sz = bool(v);
		return *this;
	}

	bignum& operator = (bignum v) {
		memcpy(val, v.val, sizeof(val));
		sz = v.sz;
		return *this;
	}

	bignum& operator *= (ll v) {
		for(int i = 0; i < sz; i++) val[i] *= v;
		for(int i = 0; i < sz; i++) {
			val[i + 1] += val[i] / BASE;
			val[i] %= BASE;
		}
		if(val[sz]) sz++;
		return *this;
	}

	bignum& operator /= (ll v) {
		ll carry = 0;
		for(int i = D - 1; i >= 0; i--) {
			carry *= BASE;
			carry += val[i];
			val[i] = carry / v;
			carry %= v;
		}
		while(sz && !val[sz - 1]) sz--;
		return *this;
	}

	ll operator % (ll v) {
		ll res = 0;
		for(int i = D - 1; i >= 0; i--) {
			res *= BASE;
			res += val[i];
			res %= v;
		}
		return res;
	}

	bignum& operator += (bignum v) {
		if(v.sz > sz) sz = v.sz;
		for(int i = 0; i < sz; i++) val[i] += v.val[i];
		for(int i = 0; i < sz; i++) {
			if(val[i] >= BASE) val[i] -= BASE, val[i + 1] ++;
		}
		if(val[sz]) sz++;
		return *this;
	}

	bignum& operator -= (bignum v) {
		for(int i = 0; i < sz; i++) val[i] -= v.val[i];
		for(int i = 0; i < sz; i++) {
			if(val[i] < 0) val[i] += BASE, val[i + 1] --;
		}
		while(sz && !val[sz - 1]) sz--;
		return *this;
	}

	bignum(int v = 0) {
		memset(val, 0, sizeof(val));
		val[0] = v;
		sz = bool(v);
	}

	bool is0() {
		for(int i = 0; i < D; i++) if(val[i]) return false;
		return true;
	}

	void print() {
		int st = sz - 1;
		if(st == -1) {
			printf("0");
			return;
		}
		printf("%d", val[st]);
		for(int i = st - 1; i >= 0; i--) {
			printf("%.9lld", val[i]);
		}
	}
};

int N;
ll P[MAXN];

bignum num[MAXN];
bignum den[MAXN];

bignum snum(0);
bignum sden(1);

ll gcd(ll a, ll b) { return (a ? gcd(b % a, a) : b); }

inline ll gcd(ll a, bignum b) {
	return gcd(b % a, a);
}

void go() {
	for(int i = N - 1; i >= 0; i--) {
		ll p = P[i];
		num[i] = sden;
		num[i] -= snum;
		ll m = gcd(p, num[i]);
		num[i] /= m;

		sden *= p / m;
		den[i] = sden;

		snum *= p / m;
		snum += num[i];

		if(p == 1) {
			snum = 1;
			sden = 1;
		} else {
			ll n = gcd(p - 1, sden);
			snum /= n;
			sden /= n;
		}
	}
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) scanf("%lld", &P[i]);

	go();

	for(int i = 0; i < N; i++) {
		num[i].print();
		printf("/");
		den[i].print();
		printf("\n");
	}
}

