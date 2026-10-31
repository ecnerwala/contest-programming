#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<int(1e9)+7>;

class ExpectedValue {

public:

	int solve(int N) {
		pair<num, num> a(1, 0);
		pair<num, num> b(0, 0);
		for (int i = 2; i <= N; i++) {
			pair<num, num> c;
			c.first = num(i-1) * (a.first + b.first);
			c.second = num(i-1) * (a.second + a.first + b.first + b.second);

			a = b;
			b = c;
		}
		return int(b.second / b.first);
	}

	int slow(int N) {
		std::vector<int> p(N); iota(p.begin(), p.end(), 0);
		int tot = 0;
		int cnt = 0;
		do {
			bool is_der = true;
			for (int i = 0; i < N; i++) {
				if (p[i] == i) is_der = false;
			}
			if (!is_der) continue;
			std::vector<int> q = p;
			int val = 0;
			for (int i = 0; i < N; i++) {
				while (q[i] != i) {
					val++;
					swap(q[i], q[q[i]]);
				}
			}

			cnt++;
			tot += val;
		} while (next_permutation(p.begin(), p.end()));
		return int(num(tot) / num(cnt));
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int N = 2; N <= 9; N++) {
		cerr << ExpectedValue().solve(N) << ' ' << ExpectedValue().slow(N) << '\n';
	}
}

}  // namespace ecnerwala

using ecnerwala::ExpectedValue;

// vim:ft=cpp
