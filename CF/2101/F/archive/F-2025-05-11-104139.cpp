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

using num = modnum<998244353>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int V = N;
		std::vector<std::vector<int>> adj(2*N-1);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(N+e);
			adj[v].push_back(N+e);
			adj[N+e].push_back(u);
			adj[N+e].push_back(v);
		}
		N = 2*N-1;
		std::vector<num> p2(V+1);
		p2[0] = 1;
		for (int i = 1; i <= V; i++) p2[i] = p2[i-1] * 2;
		std::vector<num> p3(V+1);
		p3[0] = 1;
		for (int i = 1; i <= V; i++) p3[i] = p3[i-1] * 3;

		num ans = 0;

		// 2 cases: diameter is red only, or diameter is red/blue
		for (int rt = 0; rt < N; rt++) {
			std::vector<num> ways_must_red(N, 0);
			std::vector<num> ways_must_red_must_blue(N, 0);
			std::vector<int> by_dist_tot(N);
			by_dist_tot[0] += (rt < V);
			{
				std::vector<int> by_dist_cur(N);
				for (int ch : adj[rt]) {
					by_dist_cur.assign(N, 0);
					[&](this auto&& self, int cur, int prv, int depth) -> void {
						by_dist_tot[depth] += (cur < V);
						by_dist_cur[depth] += (cur < V);
						for (int nxt : adj[cur]) {
							if (nxt == prv) continue;
							self(nxt, cur, depth+1);
						}
					}(ch, rt, 1);
					for (int d = !(rt < V); d < N; d += 2) {
						ways_must_red[d] -= p2[by_dist_cur[d]] - 1;
						ways_must_red_must_blue[d] -= p3[by_dist_cur[d]] - 2 * p2[by_dist_cur[d]] + 1;
					}
				}
			}

			num ways_pref_red_blue = 1;
			num ways_red_only = 0;
			num ans_red_only = 0;
			for (int d = !(rt < V); d < N; d += 2) {
				ways_must_red[d] += p2[by_dist_tot[d]] - 1;
				ways_must_red_must_blue[d] += p3[by_dist_tot[d]] - 2 * p2[by_dist_tot[d]] + 1;

				// case 1: red/blue diameter
				ans += num(2 * d) * ways_pref_red_blue * ways_must_red_must_blue[d];
				// case 2: red only diameter
				ans += num(2) * (ans_red_only + num(d) * ways_red_only) * ways_must_red[d];

				ways_red_only *= p2[by_dist_tot[d]];
				ans_red_only *= p2[by_dist_tot[d]];

				ways_red_only += ways_pref_red_blue * (p3[by_dist_tot[d]] - p2[by_dist_tot[d]]);
				ans_red_only += num(d) * ways_pref_red_blue * (p3[by_dist_tot[d]] - p2[by_dist_tot[d]]);

				ways_pref_red_blue *= p3[by_dist_tot[d]];
			}
			assert(ways_pref_red_blue == p3[V]);
		}

		ans /= 2;
		cout << ans << '\n';
	}

	return 0;
}
