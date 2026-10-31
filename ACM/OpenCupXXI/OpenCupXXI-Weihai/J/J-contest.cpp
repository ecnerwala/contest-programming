#include<bits/stdc++.h>

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

using hsh = modnum<int(1e9)+9>;
const hsh BASE(48);
const hsh INV_BASE_M1 = inv(BASE-1);

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
	hsh_string& operator += (int lazy_count) {
		val += (pow-1) * INV_BASE_M1 * hsh(lazy_count);
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
	vector<int> A(N);

	struct seg_node {
		int lazy;
		int mx;
		hsh_string hs;
	};
	vector<seg_node> seg(2*N);

	auto update_node = [&](int a) {
		if (a >= N) {
			seg[a].mx = (seg[a].lazy + A[a-N]) % 65536;
			seg[a].hs = hsh_string(seg[a].mx);
		} else {
			seg[a].mx = max(seg[2*a].mx, seg[2*a+1].mx);
			seg[a].hs = seg[2*a].hs + seg[2*a+1].hs;
			seg[a].mx += seg[a].lazy;
			seg[a].hs += seg[a].lazy;
		}
	};
	auto apply_lazy = [&](int a, int lazy = 1) {
		seg[a].lazy += lazy;
		seg[a].mx += lazy;
		seg[a].hs += lazy;
	};

	auto downdate_node = [&](int a) {
		assert(a < N);
		apply_lazy(2*a, seg[a].lazy);
		apply_lazy(2*a+1, seg[a].lazy);
		seg[a].lazy = 0;
	};
	auto update_all = [&](int a) {
		a >>= __builtin_ctz(a);
		for (a >>= 1; a; a >>= 1) {
			update_node(a);
		}
	};
	auto downdate_all = [&](int a_) {
		a_ >>= __builtin_ctz(a_); a_ >>= 1;
		std::y_combinator([&](auto self, int a) -> void {
			if (!a) return;
			self(a>>1);
			downdate_node(a);
		})(a_);
	};

	auto fix_mx = std::y_combinator([&](auto self, int a = 1) -> void {
		if (seg[a].mx < 65536) return;
		if (a >= N) {
			update_node(a);
			return;
		}
		downdate_node(a);
		self(2*a);
		self(2*a+1);
		update_node(a);
		assert(seg[a].mx < 65536);
	});

	auto incr_range = [&](int l, int r) {
		assert(l < r);
		for (int a = N+l, b = N+r; a < b; a /= 2, b /= 2) {
			if (a & 1) apply_lazy(a++);
			if (b & 1) apply_lazy(--b);
		}
		update_all(N+l);
		update_all(N+r);
		fix_mx();
	};

	auto query_range = [&](int l, int r) -> hsh {
		assert(l < r);
		hsh_string lo, hi;

		downdate_all(N+l);
		downdate_all(N+r);
		for (int a = N+l, b = N+r; a < b; a /= 2, b /= 2) {
			if (a & 1) lo = lo + seg[a++].hs;
			if (b & 1) hi = seg[--b].hs + hi;
		}
		return (lo + hi).val;
	};

	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int a = 2*N-1; a > 0; a--) update_node(a);

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
