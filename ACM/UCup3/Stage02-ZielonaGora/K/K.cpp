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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N); for (auto& a : A) cin >> a;

	const int L = 20;
	const int S = (1 << L);

	std::mt19937_64 mt(48);
	std::vector<uint64_t> vals(S);
	std::vector<uint64_t> vals_pref(S+1);
	for (int i = 0; i < S; i++) {
		vals[i] = mt();
		assert(vals[i] != 0);
		vals_pref[i+1] = vals_pref[i] ^ vals[i];
	}

	struct seg_node {
		std::array<int, 2> ch{-1, -1};
		uint64_t v = 0;
	};

	std::vector<seg_node> nodes; nodes.reserve(2*S + 4 * N * L);
	nodes.resize(2*S);
	for (int i = 1; i < S; i++) {
		nodes[i].ch = {2*i+0, 2*i+1};
	}

	auto incr = std::y_combinator([&](auto self, int n, int lo, int hi, int k) -> std::pair<int, bool> {
		assert(lo <= k && k < hi);
		int r = int(nodes.size());
		nodes.push_back(nodes[n]);
		bool carry;
		if (hi - lo == 1) {
			assert(nodes[r].v == 0 || nodes[r].v == vals[k]);
			carry = (nodes[r].v != 0);
			nodes[r].v ^= vals[k];
		} else {
			auto [c0, c1] = nodes[r].ch;
			int md = (lo + hi) / 2;
			if (k < md) {
				std::tie(c0, carry) = self(c0, lo, md, k);
				if (carry) {
					std::tie(c1, carry) = self(c1, md, hi, md);
				}
			} else {
				std::tie(c1, carry) = self(c1, md, hi, k);
			}
			nodes[r].ch = {c0, c1};
			nodes[r].v = nodes[c0].v ^ nodes[c1].v;
		}
		return {r, carry};
	});

	std::vector<int> roots(N+1);
	roots[0] = 1;
	for (int i = 0; i < N; i++) {
		bool carry;
		std::tie(roots[i+1], carry) = incr(roots[i], 0, S, A[i]);
		assert(!carry);
	}

	// Dir 0 means prefix, dir 1 means suffix
	auto get_match = [&](int n0, int n1, int dir) -> std::pair<int, uint64_t> {
		assert(nodes[n0].v != nodes[n1].v);
		int lo = 0, hi = S;
		int len = 0; uint64_t val = 0;
		while (hi - lo > 1) {
			assert(nodes[n0].v != nodes[n1].v);
			int md = (lo + hi) / 2;
			assert(hi - lo > 1);
			bool z;
			if (nodes[nodes[n0].ch[dir]].v != nodes[nodes[n1].ch[dir]].v) {
				z = dir;
			} else {
				assert(md - lo == hi - md);
				len += (dir == 0) ? (md - lo) : (hi - md);
				val ^= nodes[nodes[n0].ch[dir]].v;
				z = !dir;
			}
			n0 = nodes[n0].ch[z];
			n1 = nodes[n1].ch[z];
			if (z == 0) hi = md;
			else lo = md;
		}
		return {len, val};
	};
	auto get_diff = [&](int n0, int n1) -> std::pair<int, bool> {
		assert(nodes[n0].v != nodes[n1].v);

		auto [pref_len, pref_val] = get_match(n0, n1, 0);
		auto [suff_len, suff_val] = get_match(n0, n1, 1);

		assert(pref_len + suff_len < S);
		uint64_t m0 = nodes[n0].v ^ pref_val ^ suff_val;
		uint64_t m1 = nodes[n1].v ^ pref_val ^ suff_val;

		int lo = pref_len, hi = S - suff_len;
		cerr << m0 << ' ' << m1 << '\n';

		bool is_plus_one = m1 == vals[hi-1] && m0 == (vals_pref[hi-1] ^ vals_pref[lo]);

		return {pref_len, is_plus_one};
	};

	for (int i = 0; i < N; i++) {
		for (int j = i+1; j <= N; j++) {
			cerr << "checking " << i << ' ' << j << '\n';
			auto [v2, is_plus_one] = get_diff(roots[i], roots[i+1]);
			cerr << v2 << ' ' << is_plus_one << '\n';
			if (j - i == 1) {
				assert(is_plus_one);
				assert(v2 == A[i]);
			}
		}
	}

	std::vector<int> nxt(N+1);
	std::vector<int> prv(N+1);
	for (int i = 0; i <= N; i++) {
		nxt[i] = i+1;
		prv[i] = i-1;
	}

	struct interval {
	};

	std::vector<std::array<int, 2>> edges; edges.reserve(N * 40);
	for (int i = 0; i < N; i++) {
	}

	return 0;
}
