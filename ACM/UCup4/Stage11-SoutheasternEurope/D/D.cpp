#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> opts(N, {-1, -1});
	for (int e = 0; e < M; e++) {
		int u, v, x; cin >> u >> v >> x; u--, v--, x--;
		assert(u < v);
		if (opts[x][0] == -1) {
			opts[x] = {u, v};
		} else {
			// intersect opts[x] and {u, v}
			bool u_good = opts[x][0] == u || opts[x][1] == u;
			bool v_good = opts[x][0] == v || opts[x][1] == v;
			if (!(u_good || v_good)) {
				cout << 0 << '\n';
				exit(0);
			}
			opts[x] = {u_good ? u : v, v_good ? v : u};
		}
	}

	std::vector<int> par(N, -1);
	std::vector<int> cnt_e(N, -1);
	std::vector<int> has_loop(N, 0);

	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int u, int v) -> int {
		u = get_par(u), v = get_par(v);
		if (u == v) return u;
		if (par[u] > par[v]) std::swap(u, v);
		par[u] += par[v];
		par[v] = u;
		cnt_e[u] += cnt_e[v];
		has_loop[u] += has_loop[v];
		return u;
	};

	for (int x = 0; x < N; x++) {
		if (opts[x][0] == -1) continue;
		auto [u, v] = opts[x];
		int c = merge(u, v);
		assert(par[c] < 0);
		cnt_e[c]++;
		has_loop[c] += (u == v);
	}

	int num_trees = 0;
	num ans = 1;
	for (int c = 0; c < N; c++) {
		if (par[c] >= 0) continue;
		if (cnt_e[c] > 0) {
			cout << 0 << '\n';
			exit(0);
		} else if (cnt_e[c] == 0) {
			assert(has_loop[c] <= 1);
			if (has_loop[c]) ans += ans;
		} else if (cnt_e[c] == -1) {
			num_trees++;
			ans *= num(num_trees);
			ans *= num(-par[c]);
		} else assert(false);
	}
	cout << ans << '\n';

	return 0;
}
