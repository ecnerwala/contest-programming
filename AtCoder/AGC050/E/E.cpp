#include <bits/stdc++.h>

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

struct mod_t {
	int64_t v;
	int64_t m;
};

using mnum = modnum<998244353>;

auto solve_2(mod_t a, mod_t b) -> mnum {
	int64_t g = std::gcd(a.m, b.m);

	mnum ans = mnum(a.v / g * g) * mnum(b.v);
	a.v %= g;
	a.m = g;

	ans += mnum(b.v / g * g) * mnum(a.v);
	b.v %= g;
	b.m = g;

	ans += mnum(std::min(a.v, b.v)) * mnum(g);

	return ans;
}

auto solve_3(std::array<mod_t, 3> A) -> mnum {
	int64_t g = std::gcd(std::gcd(A[0].m, A[1].m), A[2].m);

	mnum ans = 0;
	for (int z = 0; z < 3; z++) {
		int x = (z+1)%3;
		int y = (z+2)%3;
		int64_t l = g * std::gcd(A[z].m / g, A[x].m / g) * std::gcd(A[z].m / g, A[y].m / g);
		ans += solve_2(A[x], A[y]) * mnum(A[z].v / l * l);
		A[z].v %= l;
		A[z].m = l;
	}

	std::sort(A.begin(), A.end(), [&](const auto& a, const auto& b) { return a.m < b.m; });

	int64_t v0 = std::gcd(A[1].m / g, A[2].m / g);
	int64_t v1 = std::gcd(A[2].m / g, A[0].m / g);
	int64_t v2 = std::gcd(A[0].m / g, A[1].m / g);

	assert(v0 >= v1 && v1 >= v2);
	assert(A[0].m * v0 == A[1].m * v1);
	assert(A[0].m * v0 == A[2].m * v2);

	mnum cur_ans = 0;
	// In the first g * v0 * v1 * v2 steps, how many are on?
	for (int64_t i = 0, j = 0; i < v1 && j < v2; ) {
		int64_t lo1 = A[1].m * i;
		int64_t hi1 = lo1 + A[1].v;

		int64_t lo2 = A[2].m * j;
		int64_t hi2 = lo2 + A[2].v;

		int64_t lo = std::max(lo1, lo2);
		int64_t hi = std::min(hi1, hi2);
		if (lo < hi) {
			cur_ans += (hi / A[0].m * A[0].v) + std::min(hi % A[0].m, A[0].v);
			cur_ans -= (lo / A[0].m * A[0].v) + std::min(lo % A[0].m, A[0].v);
		}

		if (hi1 < hi2) {
			i++;
		} else {
			j++;
		}
	}

	ans += cur_ans * g * g * v0 * v1 * v2;
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::array<mod_t, 3> A;
	for (auto& a : A) {
		cin >> a.v >> a.m;
		a.m += a.v;
	}

	cout << solve_3(A) << '\n';

	return 0;
}
