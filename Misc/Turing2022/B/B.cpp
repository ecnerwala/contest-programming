#include <bits/stdc++.h>

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

	void div_2() {
		v = ((v & 1) ? v + MOD : v) >> 1;
	}
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	using num = modnum<998244353>;
	std::vector<num> V(N);
	for (int i = 0; i < N; i++) { V[i] = num(i+1); }

	num ans = 0;

	// We'll compute the centroid index by the start vertex, subtracting out by each imbalanced edge's destination.
	for (int i = 0; i < N; i++) {
		ans += V[i];
	}

	// We'll compute probabilities
	auto resp = std::y_combinator([&](auto self, int cur, int prv) -> std::tuple<std::vector<num>, std::vector<num>, std::vector<num>> {
		int sz = 1;
		std::vector<num> tree_ways(sz+1);
		std::vector<num> eq_ways(sz+1);
		std::vector<num> imba_ways(sz+1);
		tree_ways[1] = 1;

		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto ch_vals = self(nxt, cur);
			std::vector<num> &ch_tree_ways = std::get<0>(ch_vals), &ch_eq_ways = std::get<1>(ch_vals), &ch_imba_ways = std::get<2>(ch_vals);
			//cerr << "cur " << cur << ' ' << nxt << '\n';
			int ch_sz = int(ch_tree_ways.size()) - 1;

			std::vector<num> n_tree_ways(sz + ch_sz + 1);
			std::vector<num> n_eq_ways(sz + ch_sz + 1);
			std::vector<num> n_imba_ways(sz + ch_sz + 1);

			for (int a = 0; a <= sz; a++) {
				tree_ways[a].div_2();
				eq_ways[a].div_2();
				imba_ways[a].div_2();
			}

			// with 1/2 probability, these guys just end, so we should tally up the ch_eq_ways
			auto x = ch_eq_ways[0]; x.div_2();
			ans += x;
			for (int a = 0; a <= sz; a++) {
				n_tree_ways[a] += tree_ways[a];
				n_eq_ways[a] += eq_ways[a];
				n_imba_ways[a] += imba_ways[a];
			}

			// with 1/2 probability, we take this edge.
			// By default, we'll treat it as a -V[cur], but it transitions to 0 or -V[nxt] if it hits eq or imba

			for (int a = 0; a <= sz; a++) {
				for (int b = 0; b <= ch_sz; b++) {
					num p = tree_ways[a] * ch_tree_ways[b];
					n_tree_ways[a + b] += p;
					if (b - a >= 0) {
						ans += p * (-V[cur]);
						n_eq_ways[b-a] += p * V[cur];
						n_imba_ways[b-a] += p * (V[cur] - V[nxt]);
					} else {
						ans += p * (-V[nxt]);
					}

					if (b - a >= 0) {
						n_eq_ways[b-a] += tree_ways[a] * ch_eq_ways[b];
						n_imba_ways[b-a] += tree_ways[a] * ch_imba_ways[b];
					} else {
						ans += tree_ways[a] * ch_imba_ways[b];
					}

					if (a - b >= 0) {
						n_eq_ways[a-b] += eq_ways[a] * ch_tree_ways[b];
						n_imba_ways[a-b] += imba_ways[a] * ch_tree_ways[b];
					} else {
						ans += imba_ways[a] * ch_tree_ways[b];
					}

				}
			}
			tree_ways = std::move(n_tree_ways);
			eq_ways = std::move(n_eq_ways);
			imba_ways = std::move(n_imba_ways);
			sz += ch_sz;
		}
		return {tree_ways, eq_ways, imba_ways};
	})(0, -1);
	ans += std::get<1>(resp)[0];

	for (int i = 0; i < N-1; i++) {
		ans += ans;
	}
	cerr << ans << '\n';
	cout << ans << '\n';

	return 0;
}
