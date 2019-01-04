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

	friend bool operator == (const num& a, const num& b) { return a.v == b.v; }
	friend bool operator != (const num& a, const num& b) { return a.v != b.v; }

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

// given that n is a power of two,
// black magic to bit-reversal permute a[0]..a[n-1]
// e.g. swap a[001101] with a[101100]
template<typename T> void bitReversalPermute(T *a, int n) {
	int i = 0;
	for(int j = 1; j < n-1; j++) {
		// after each increment of j, we want to change i by flipping
		// bits up to and including the first (most-significant) 0
		for (int k = n/2; k > (i^=k); k/=2);
		// now i, j are bit reversals
		if (j < i) swap(a[i], a[j]);
	}
}

struct IntFFT {
	int n;
	num root, rootinv, ninv;
	// n = length; m = modulus; root = mth root of unity
	// rootinv, ninv = inverses of root, n respectively mod m
	IntFFT(int nn, num r) {
		n = nn;
		root = r;
		rootinv = inv(root);
		ninv = inv(num(n));
		assert(root * rootinv == 1);
		assert(n * ninv == 1);
		assert(powmod(root, n/2) == -1);
	}

	void operator()(num *a, bool invert) {
		num cr = invert ? root : rootinv; // sign convention?
		for(int h = n/2; h; cr *= cr, h /= 2) {
			num w = 1;
			for (int i=0; i<h; i++, w *= cr) {
				for (int j = i; j < n; j += 2*h) {
					int k = j + h;
					num x = a[j] - a[k];
					a[j] = a[j] + a[k];
					a[k] = w*x;
				}
			}
		}
		bitReversalPermute(a, n);
		if (invert) {
			for (int i = 0; i < n; i++) a[i] *= ninv;
		}
	}
};

const int S = 1 << 20;
IntFFT ifft(S, 646);

const int MAXN = 500010;
int N;
num X[S];
num Y[S];
num pow2[MAXN];

class Subpolygon {
public:

	int sumOfAreas(int n) {
		N = n * 4;
		memset(X, 0, sizeof(X));
		memset(Y, 0, sizeof(Y));

		X[0] = 0;
		Y[0] = 0;
		for (int i = 0; i < N; i++) {
			int quad = i / (N / 4);
			int v = i % (N / 4);
			int dx, dy;
			switch(quad) {
				case 0:
					dx = N/4 - v;
					dy = v;
					break;
				case 1:
					dx = -v;
					dy = N/4 - v;
					break;
				case 2:
					dx = -(N/4-v);
					dy = -v;
					break;
				case 3:
					dx = v;
					dy = -(N/4-v);
					break;
				default: assert(false);
			}
			assert(abs(dx) + abs(dy) == N/4);
			X[i+1] = X[i] + dx;
			Y[i+1] = Y[i] + dy;
		}
		assert(X[N] == X[0]);
		assert(Y[N] == Y[0]);
		X[N] = Y[N] = 0; // revert it

		//for (int i = 0; i < N; i++) { cerr << X[i] << ' ' << Y[i] << '\n'; } cerr << '\n';

		X[N] = 0;
		reverse(X, X + N+1); // reverse X

		ifft(X, false);
		ifft(Y, false);
		for (int i = 0; i < S; i++) {
			X[i] *= Y[i];
		}
		ifft(X, true);

		// make it cyclic
		for (int i = N; i < 2 * N; i++) {
			X[i-N] += X[i];
			X[i] = 0;
		}

		num res = 0;

		pow2[0] = 1;
		for (int i = 1; i <= N; i++) {
			pow2[i] = pow2[i-1] + pow2[i-1];
		}
		for (int d = 1; d < N; d++) {
			// points at a distance of d apart
			num cross = X[d] - X[N-d];
			res += cross * pow2[N - (d+1)];
		}

		// we have to divide by 2 in the end
		return int(res / 2);
	}
};

}  // namespace ecnerwala

using ecnerwala::Subpolygon;

// vim:ft=cpp
