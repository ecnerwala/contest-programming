#include <bits/stdc++.h>
using namespace std;

typedef double ld;
typedef long long ll;

struct bigval {
	static const int MOD = 1e9;
	double dval;
	int mval; // value mod MOD
	
	bool operator < (const bigval & a) const {
		return dval < a.dval;
	}
	
	bool operator > (const bigval & a) const {
		return a < *this;
	}
	
	bigval & operator *= (const ll & a) {
		dval += log(a);
		mval = a * mval % MOD;
		return *this;
	}

	bigval & operator ++ () {
		mval ++;
		mval %= MOD;
		return *this;
	}

	bigval(double d, long long m) : dval(d), mval(m % MOD) {}
	bigval(long long a) : dval(a), mval(a % MOD) {}
	bigval() : dval(0), mval(0) { }
};

const int MAXV = 4e7;
bool is_prime[MAXV];
bigval minl[MAXV];

bigval L(int N) {
	for(int i = 2; i <= N; i++) is_prime[i] = true;
	for(int i = 1; i <= N; i++) minl[i] = 1;

	for(int p = 2; p <= N; p++) {
		if(!is_prime[p]) continue;
		for(ll j = p + p; j <= N; j += p) {
			is_prime[j] = false;
		}
		for(ll v = p - 1; v <= N; v *= p) {
			for(ll j = v; j <= N; j += v) {
				minl[j] *= p;
			}
		}
	}

	for(int j = 2; j <= N; j += 2) {
		minl[j] *= 2;
	}
	
	bigval res = 0;
	for(int i = 1; i < N; i++) {
		if(minl[i] > res) res = minl[i];
	}
	++res;
	return res;
}

int main() {
	cerr << L(6).mval << '\n';
	cerr << L(100).mval << '\n';
	bigval res = L(int(2e7));
	cerr << res.dval << '\n';
	cerr << res.mval << '\n';
	return 0;
}
