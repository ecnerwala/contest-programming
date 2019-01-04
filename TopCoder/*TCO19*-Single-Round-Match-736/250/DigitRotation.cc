#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

int minv (int a, int m) {
	a %= m;
	assert(a);
	return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
}

struct num {
	static const int MOD = 998244353;
private:
	int v;
public:
	num() : v (0) {}
	num(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	friend num operator - (const num& o) {
		num res;
		res.v = (o.v ? MOD - o.v : 0);
		return res;
	}
	friend num inv(const num& o) {
		num res;
		res.v = minv(o.v, MOD);
		return res;
	}
	num& operator += (const num& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	num& operator -= (const num& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	num& operator *= (const num& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	num& operator /= (const num& o) {
		return *this *= inv(o);
	}
	friend num operator + (const num& a, const num& b) { return num(a) += b; }
	friend num operator - (const num& a, const num& b) { return num(a) -= b; }
	friend num operator * (const num& a, const num& b) { return num(a) *= b; }
	friend num operator / (const num& a, const num& b) { return num(a) /= b; }

	friend num operator == (const num& a, const num& b) { return a.v == b.v; }
	friend num operator != (const num& a, const num& b) { return a.v != b.v; }

	explicit operator int () const { return v; }
	friend std::ostream& operator << (std::ostream& out, const num& n) { return out << int(n); }
};

num powmod(num a, ll b) {
	assert(b >= 0);
	num r = 1;
	while(b) {
		if (b & 1) r *= a;
		a *= a;
		b /= 2;
	}
	return r;
}

const int MAXN = 600;
num p10[MAXN];

num c3 (num n) {
	return n * (n-1) * (n-2) / 6;
}

class DigitRotation {

public:

	int sumRotations(string X) {
		int N = int(X.size());
		p10[N-1] = 1;
		for (int i = N-1; i > 0; i--) {
			p10[i-1] = p10[i] * 10;
		}
		num res = 0;
		for (int i = 0; i < N; i++) {
			int v = X[i] - '0';
			res += v * c3(N-1) * p10[i];
			for (int z = 1; z < N; z++) {
				int j = (i + N - z) % N;
				res += v * (z - 1) * p10[j];
			}
		}
		num val = 0;
		for (int i = 0; i < N; i++) {
			val += p10[i] * (X[i] - '0');
		}
		for (int b = 1; b < N; b++) {
			for (int c = b+1; c < N; c++) {
				if (X[c] == '0') {
					// subtract off this thing
					res -= (val + p10[0] * (X[c] - X[0]) + p10[b] * (X[0] - X[b]) + p10[c] * (X[b] - X[c]));
				}
			}
		}
		return int(res);
	}
};

}  // namespace ecnerwala

using ecnerwala::DigitRotation;

// vim:ft=cpp
