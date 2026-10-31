#include<bits/stdc++.h>

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

struct tree_point {
	int a;
	tree_point() : a(0) {}
	tree_point(int a_) : a(a_) { assert(a >= 1); }

	operator int() const { return a; }

	template <typename F> void for_each(F f) const {
		for (int v = a; v > 0; v >>= 1) {
			f(v);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		// strictly greater than 0
		for (int L = log_2(a); L > 0; L--) {
			f(a >> L);
		}
	}

	template <typename F> void for_parents_up(F f) const {
		for (int v = a >> 1; v > 0; v >>= 1) {
			f(v);
		}
	}
};

struct tree_range {
	std::array<int, 2> r;
	tree_range() : r{0,0} {}
	tree_range(int a, int b) : tree_range(std::array<int, 2>{a,b}) {}
	tree_range(std::array<int, 2> r_) : r(r_) {
		assert(1 <= r[0] && r[0] <= r[1] && r[1] <= 2 * r[0]);
	}

	explicit operator std::array<int, 2>() const {
		return r;
	}

	const int& operator[] (int z) const {
		assert(z == 0 || z == 1);
		return r[z];
	}

	template <typename F> void for_each(F f) const {
		auto [a, b] = r;
		for (; a < b; a >>= 1, b >>= 1) {
			if (a & 1) f(a++, false);
			if (b & 1) f(--b, true);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		auto [a, b] = r;
		a >>= __builtin_ctz(a);
		b >>= __builtin_ctz(b);

		// TODO: dedup
		tree_point(a).for_parents_down(f);
		tree_point(b).for_parents_down(f);
	}

	template <typename F> void for_parents_up(F f) const {
		auto [a, b] = r;
		a >>= __builtin_ctz(a);
		b >>= __builtin_ctz(b);

		// TODO: dedup
		tree_point(a).for_parents_up(f);
		tree_point(b).for_parents_up(f);
	}
};

struct in_order_layout {
	int N, S;
	in_order_layout() : N(0), S(0) {}
	in_order_layout(int N_) : N(N_), S(next_pow_2(N)) {}

	tree_point get_point(int a) const {
		assert(0 <= a && a < N);
		a += S;
		return tree_point(a >= 2 * N ? a - N : a);
	}

	tree_range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		a += S, b += S;
		return tree_range((a >= 2 * N ? 2*(a-N) : a), (b >= 2 * N ? 2*(b-N) : b));
	}

	tree_range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_node_index(int a) const {
		assert(N <= a && a < 2 * N);
		return (a < S ? a + N : a) - S;
	}

	std::array<int, 2> get_node_bounds(int a) const {
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? (x>>1) + N : x) - S, (y >= 2 * N ? (y>>1) + N : y) - S};
	}

	int get_node_size(int a) const {
		assert(1 <= a && a < 2 * N);
		auto [x, y] = get_node_bounds(a);
		return y - x;
	}
};

struct circular_layout {
	int N;
	circular_layout() : N(0) {}
	circular_layout(int N_) : N(N_) {}

	tree_point get_point(int a) const {
		assert(0 <= a && a < N);
		return tree_point(N + a);
	}

	tree_range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		return tree_range(N + a, N + b);
	}

	tree_range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_node_index(int a) const {
		assert(N <= a && a < 2 * N);
		return a - N;
	}

	std::array<int, 2> get_node_bounds(int a) const {
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int S = next_pow_2(N);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? x >> 1 : x) - N, (y >= 2 * N ? y >> 1 : y) - N};
	}

	int get_node_size(int a) const {
		assert(1 <= a && a < 2 * N);
		// delegate to in order
		return in_order_layout(N).get_node_size(a);
	}

};

} // namespace seg_tree

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

using hsh = modnum<int(1e9)+9>;
const hsh BASE(48);

struct hsh_string {
	hsh val;
	hsh pow;

	hsh_string() : val(0), pow(1) {}
	explicit hsh_string(int v) : val(v), pow(BASE) {}
	hsh_string& operator += (const hsh_string& b) {
		val += pow * b.val;
		pow *= b.pow;
		return *this;
	}
	friend hsh_string operator + (const hsh_string& a, const hsh_string& b) {
		return hsh_string(a) += b;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q; cin >> N >> Q;
	using mnum = modnum<65536>;
	vector<mnum> A(N); for (auto& a : A) cin >> a;

	auto seg = seg_tree::circular_layout(N);

	vector<mnum> incr_seg(2*N);
	for (int i = 0; i < N; i++) incr_seg[seg.get_point(i)] = A[i];
	vector<hsh_string> diff_seg(2*N);
	for (int i = 1; i < N; i++) {
		diff_seg[seg.get_point(i)] = hsh_string(int(A[i] - A[i-1]));
	}
	auto update_node = [&](int a) {
		assert(1 <= a && a < N);
		diff_seg[a] = diff_seg[2*a] + diff_seg[2*a+1];
	};
	for (int a = N-1; a; a--) update_node(a);

	auto incr_range = [&](int l, int r) {
		assert(l < r);
		auto rng = seg.get_range(l, r);
		rng.for_each([&](int a, bool) { incr_seg[a]++; });
		if (l > 0) {
			auto& v = diff_seg[seg.get_point(l)].val;
			if (v == 65535) v = 0;
			else v++;
			seg.get_point(l).for_parents_up(update_node);
		}
		if (r < N) {
			auto& v = diff_seg[seg.get_point(r)].val;
			if (v == 0) v = 65535;
			else v--;
			seg.get_point(r).for_parents_up(update_node);
		}
	};

	auto query_range = [&](int l, int r) -> hsh {
		assert(l < r);

		mnum v0 = 0;
		seg.get_point(l).for_each([&](int a) { v0 += incr_seg[a]; });

		hsh_string lo, hi;
		lo = hsh_string(int(v0));

		seg.get_range(l+1, r).for_each([&](int a, bool d) {
			if (d == 0) lo = lo + diff_seg[a];
			else hi = diff_seg[a] + hi;
		});
		return (lo + hi).val;
	};

	while (Q--) {
		int op; cin >> op;
		if (op == 1) {
			int l, r; cin >> l >> r; l--;
			incr_range(l, r);
		} else if (op == 2) {
			int xl, yl, len; cin >> xl >> yl >> len; xl--, yl--;
			hsh a = query_range(xl, xl + len);
			hsh b = query_range(yl, yl + len);
			cout << (a == b ? "yes" : "no") << '\n';
		} else assert(false);
	}

	return 0;
}
