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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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
using num = modnum<int(1e9)+7>;
num pow(num a, long long b) {
	num r = 1;
	while (b) { if (b & 1) r *= a; b >>= 1; a *= a; }
	return r;
}

using ll = long long;

struct line {
	ll a, b, c;
};

class TwoLineRegions {

public:

	int count(vector <int> A, vector <int> B, vector <int> C) {
		vector<line> rays;
		for (int i = 0; i < int(A.size()); i++) {
			rays.push_back(line{A[i], B[i], C[i]});
			rays.push_back(line{-A[i], -B[i], -C[i]});
		}

		num ans = 0;
		for (int z = 0; z < int(rays.size()); z++) {
			line cur = rays[z];
			ll dx = -cur.b, dy = cur.a;
			vector<pair<double, double>> lines;
			for (int i = 0; i < int(A.size()); i++) {
				line nxt{A[i], B[i], C[i]};
				// cur.a * x + cur.b * y = cur.c
				// nxt.a * x + nxt.b * y = nxt.c

				ll denom = cur.a * nxt.b - nxt.a * cur.b;
				if (denom == 0) continue;

				if (denom < 0) {
					nxt.a *= -1;
					nxt.b *= -1;
					nxt.c *= -1;
				}
				denom = cur.a * nxt.b - nxt.a * cur.b;
				assert(denom > 0);

				ll xnum = cur.c * nxt.b - nxt.c * cur.b;
				ll ynum = cur.a * nxt.c - nxt.a * cur.c;

				ll numer = xnum * dx + ynum * dy;

				double pos = double(numer) / double(denom);

				double d = atan2(nxt.b, nxt.a) - atan2(cur.b, cur.a);
				if (d < 0) {
					d += 2 * M_PI;
				}
				assert(0 <= d && d <= M_PI);

				lines.emplace_back(pos, M_PI - d);
			}
			assert(lines.size() + 1 == A.size());
			vector<double> xs, ys;
			for (auto it : lines) {
				xs.push_back(it.first);
				ys.push_back(it.second);
			}
			sort(xs.begin(), xs.end());
			sort(ys.begin(), ys.end());
			sort(lines.begin(), lines.end());
			vector<int> bit(ys.size() + 1, 0);
			for (auto it : lines) {
				// we just care about the y
				int v = int(lower_bound(ys.begin(), ys.end(), it.second) - ys.begin());
				//assert(ys[v] == it.second);
				v++;
				int cnt = 0;
				for (int a = v; a > 0; a -= a & (-a)) {
					cnt += bit[a];
				}
				int slocnt = 0;
				for (auto jt : lines) {
					if (it.first > jt.first && it.second > jt.second) slocnt++;
				}
				assert(cnt == slocnt);
				ans += pow(num(2), cnt);
				for (int a = v; a <= int(ys.size()); a += a & (-a)) {
					bit[a] ++;
				}
			}
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::TwoLineRegions;

// vim:ft=cpp
