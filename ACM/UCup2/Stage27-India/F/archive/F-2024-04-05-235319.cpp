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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int C; cin >> C;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> sz(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			sz[cur] = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				sz[cur] += sz[nxt];
			}
		})(0, -1);
		std::vector<int64_t> tot_dist(N);
		tot_dist[0] = std::accumulate(sz.begin() + 1, sz.end(), int64_t(0));
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				tot_dist[nxt] = tot_dist[cur] + N - 2 * sz[nxt];
				self(nxt, cur);
			}
		})(0, -1);

		using num = modnum<998244353>;
		num orig_ans = 0;
		std::y_combinator([&](auto self, int cur, int prv) -> num {
			num tot_sub_ways = 0;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				num sub_ways = self(nxt, cur);
				// this edge with itself
				orig_ans += sz[nxt];
				// this edge with something in it
				orig_ans += sub_ways * 2 * (N - sz[nxt]);
				// this edge with just a vertex
				sub_ways += sz[nxt];
				orig_ans += sub_ways * tot_sub_ways * 2;
				tot_sub_ways += sub_ways;
			}
			return tot_sub_ways;
		})(0, -1);
		cerr << "orig ans " << orig_ans << '\n';

		auto cube = [](num x) -> num {
			return x * x * x;
		};

		num tot_ans = cube(orig_ans);

		// The total cost of adding a new one is (N-1) + 2 * tot_length[i] + # new ones
		std::vector<int64_t> leaves; leaves.reserve(N);
		for (int i = 0; i < N; i++) {
			if (adj[i].size() == 1) leaves.push_back((N-1) + 2 * tot_dist[i]);
		}

		std::vector<int64_t> diffs; diffs.reserve(C);

		int lidx = 0;
		int didx = 0;

		num cur_ans = orig_ans;
		for (int c = N; c <= C; c++) {
			cur_ans += num(c - N);
			int64_t a;
			if (lidx < int(leaves.size()) && (didx == int(diffs.size()) || leaves[lidx] < diffs[didx])) {
				a = leaves[lidx++];
			} else {
				a = diffs[didx++];
			}
			cur_ans += num(a);
			diffs.push_back(a + N-1);
			cerr << "c = " << c << ' ' << "cur_ans = " << cur_ans << '\n';
			tot_ans += cube(cur_ans);
		}
		cout << tot_ans << '\n';
	}

	return 0;
}
