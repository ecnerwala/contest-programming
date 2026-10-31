#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

const int MAXN = int(1e3) + 20;

struct bignum {
	static const ll BASE = 1e9;
	int sz;
	int val[MAXN];

	bignum& operator *= (ll v) {
		ll carry = 0;
		for(int i = 0; i < sz; i++) {
			carry += ll(val[i]) * v;
			val[i] = int(carry % BASE);
			carry /= BASE;
		}
		if (carry) {
			val[sz++] = int(carry);
		}
		return *this;
	}

	bignum& operator /= (ll v) {
		ll carry = 0;
		for(int i = sz - 1; i >= 0; i--) {
			carry *= BASE;
			carry += val[i];
			val[i] = int(carry / v);
			carry %= v;
		}
		while(sz && !val[sz - 1]) sz--;
		return *this;
	}

	ll operator % (ll v) const {
		ll res = 0;
		for(int i = sz - 1; i >= 0; i--) {
			res *= BASE;
			res += val[i];
			res %= v;
		}
		return res;
	}

	explicit bignum(int v = 0) {
		memset(val, 0, sizeof(val));
		val[0] = v;
		sz = bool(v);
	}

	friend std::ostream& operator << (ostream& o, const bignum& b) {
		if (!b.sz) {
			o << '0';
		} else {
			o << b.val[b.sz-1];
			for (int i = b.sz-2; i >= 0; i--) {
				o << setw(9) << setfill('0') << b.val[i];
			}
		}
		return o;
	}
};

int N;
int P[MAXN];

string ans[MAXN];

bignum snum(1);
bignum sden(1);

inline ll gcd(ll a, const bignum& b) {
	return std::gcd(b % a, a);
}

void go() {
	for(int i = N - 1; i >= 0; i--) {
		ll p = P[i];

		ll m = gcd(p, snum);
		snum /= m;
		sden *= p / m;

		ostringstream strm;
		strm << snum << '/' << sden;
		ans[i] = strm.str();

		if(p == 1) {
			for (int j = i-1; j >= 0; j--) {
				ans[j] = "0/1";
			}
			return;
		} else {
			ll n = gcd(p - 1, sden);
			snum *= (p-1) / n;
			sden /= n;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> P[i];

	go();

	for(int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}
}
