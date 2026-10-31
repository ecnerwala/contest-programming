#include <bits/stdc++.h>

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
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(int64_t(v) * int64_t(o.v) % MOD);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	struct op_t {
		int t;
		int64_t x;
	};
	std::vector<op_t> ops(N);

	const int64_t INF = int(1e9) + 10;
	{
		int64_t tot_reduce = 0;
		for (auto& op : ops) {
			cin >> op.t;
			if (op.t == 1) {
				cin >> op.x;
			} else if (op.t == 2) {
				cin >> op.x;
				tot_reduce = std::min(tot_reduce + op.x, INF);
			} else if (op.t == 3) {
				op.x = tot_reduce;
				tot_reduce = std::min(tot_reduce + op.x, INF);
			} else assert(false);
		}
	}

	using num = modnum<998244353>;
	num ans = 0;

	// pow2 of num 0's
	num fac = 1;

	int64_t type_2_reduce = 0;
	std::vector<int64_t> type_3_reduce; type_3_reduce.reserve(N+1);
	type_3_reduce.push_back(INF);
	for (int i = N-1; i >= 0; i--) {
		int64_t x = ops[i].x;
		if (ops[i].t == 1) {
			x -= type_2_reduce;
			if (x <= 0) continue;
			num ways = 0;
			for (int z = 0; z < int(type_3_reduce.size()); z++) {
				ways *= 2;
				if (x > type_3_reduce[z]) {
					ways++;
					x -= type_3_reduce[z];
				}
			}
			ways++;
			ans += ways * fac;
		} else if (ops[i].t == 2) {
			type_2_reduce = std::min(type_2_reduce + x, INF);
		} else if (ops[i].t == 3) {
			if (x == 0) fac *= 2;
			else if (x < INF) type_3_reduce.push_back(x);
			else { /* do nothing */ }
		} else assert(false);
	}
	cout << ans << '\n';

	return 0;
}
