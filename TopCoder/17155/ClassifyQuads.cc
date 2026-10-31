#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

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
		res.v = mod_inv_in_range(v, MOD);
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

class ClassifyQuads {

public:

	vector <int> classify(int R, int C) {
		using num = modnum<int(1e9)+7>;
		std::array<num, 6> ans;
		num quad_lines = 0;
		std::vector<pair<int, int>> deltas; deltas.reserve(2*R*C);
		for (int dx = 0; dx < R; dx++) {
			for (int dy = 0; dy < C; dy++) {
				if (dx == 0 && dy == 0) continue;
				int g = __gcd(dx, dy);
				num weight = num(R-dx) * num(C-dy);
				if (dx > 0 && dy > 0) weight *= 2; // both slopes
				if (g >= 3) {
					ans[0] += weight;
					quad_lines += weight * num(g-1) * num(g-2) / num(2);
				}
				deltas.push_back({dx, dy});
				if (dx > 0 && dy > 0) {
					deltas.push_back({dx, -dy});
				}
			}
		}

		for (int a = 0; a < int(deltas.size()); a++) {
			for (int b = a+1; b < int(deltas.size()); b++) {
				int x1, y1; std::tie(x1, y1) = deltas[a];
				int x2, y2; std::tie(x2, y2) = deltas[b];

				int dx = std::max({0, x1, x2}) - std::min({0, x1, x2});
				if (dx >= R) continue;
				int dy = std::max({0, y1, y2}) - std::min({0, y1, y2});
				if (dy >= C) continue;

				num weight = num(R - dx) * num(C - dy);

				int area2 = abs(x1 * y2 - x2 * y1);
				if (area2 == 0) continue;
				int boundary = __gcd(abs(x1), abs(y1)) + __gcd(abs(x2), abs(y2)) + __gcd(abs(x1 - x2), abs(y1 - y2));
				int interior = area2 + 2 - boundary;
				assert(interior % 2 == 0);
				interior /= 2;

				if (boundary > 3) ans[1] += weight;
				ans[2] += weight * (boundary - 3);
				ans[3] += weight * interior;
			}
		}

		ans[4] = num(R*C) * num(R*C-1) * num(R*C-2) * num(R*C-3) / num(24);
		ans[4] -= ans[3];
		ans[4] -= quad_lines;
		ans[4] -= ans[2];

		ans[2] *= 2;
		ans[3] *= 3;
		ans[5] = 2 * ans[4];
		return std::vector<int>(ans.begin(), ans.end());
	}
};

void my_tests() {
	// FILL IN TESTS
	std::cerr << ClassifyQuads().classify(100, 30)[0] << '\n';
	std::cerr << ClassifyQuads().classify(2, 1500)[0] << '\n';
}

}  // namespace ecnerwala

using ecnerwala::ClassifyQuads;

// vim:ft=cpp
