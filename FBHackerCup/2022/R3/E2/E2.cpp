#include<bits/stdc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

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

template <typename T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator < (const Point& a, const Point& b) { return std::tie(a.x, a.y) < std::tie(b.x, b.y); }
	friend bool operator <= (const Point& a, const Point& b) { return std::tie(a.x, a.y) <= std::tie(b.x, b.y); }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	T dist2() const { return x * x + y * y; }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return __gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.norm(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less(base, s, t); };
	}
	friend auto angle_cmp_center(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less(s, p, t) ? 1 : -1;
	}
};

using pt_t = Point<int64_t>;

std::mt19937_64 mt(48);
struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937_64::result_type pri = mt();
	std::array<pt_t, 2> seg;
	int nxt_region = -1;
};

std::vector<treap_node*> to_free;
void free_all() {
	for (auto n : to_free) delete n;
	to_free.clear();
}
treap_node* make_node(pt_t a, pt_t b, int region) {
	auto res = new treap_node;
	to_free.push_back(res);
	res->seg = {a, b};
	res->nxt_region = region;
	return res;
}
treap_node* make_clone(treap_node* n) {
	assert(n);
	auto res = new treap_node(*n);
	to_free.push_back(res);
	return res;
}
treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	if (a->pri < b->pri) {
		treap_node *r = make_clone(a);
		r->c[1] = merge(a->c[1], b);
		return r;
	} else {
		treap_node *r = make_clone(b);
		r->c[0] = merge(a, b->c[0]);
		return r;
	}
}
treap_node* get_first(treap_node* a) {
	assert(a);
	while (a->c[0]) a = a->c[0];
	return a;
}
treap_node* get_last(treap_node* a) {
	assert(a);
	while (a->c[1]) a = a->c[1];
	return a;
}

bool is_below(pt_t p, std::array<pt_t, 2> seg) {
	assert(seg[0] < p && p < seg[1]);
	auto a = cross3(seg[0], p, seg[1]);
	assert(a != 0);
	return a > 0;
}

treap_node* replace(treap_node* a, pt_t loc, pt_t ploc, pt_t nloc) {
	assert(a);
	assert(a->seg[0] <= loc && loc <= a->seg[1]);
	assert(a->seg[0] != loc);
	treap_node *r = make_clone(a);
	if (a->seg[1] == loc) {
		assert(a->seg[0] == ploc);
		r->seg = {loc, nloc};
		return r;
	} else if (is_below(loc, a->seg)) {
		r->c[0] = replace(a->c[0], loc, ploc, nloc);
	} else {
		r->c[1] = replace(a->c[1], loc, ploc, nloc);
	}
	return r;
}

std::pair<treap_node*, treap_node*> split_by_point(treap_node* a, pt_t loc) {
	if (!a) return {nullptr, nullptr};
	assert(a);
	assert(a->seg[0] < loc && loc < a->seg[1]);
	treap_node *x, *y;
	if (is_below(loc, a->seg)) {
		y = make_clone(a);
		std::tie(x, y->c[0]) = split_by_point(a->c[0], loc);
	} else {
		x = make_clone(a);
		std::tie(x->c[1], y) = split_by_point(a->c[1], loc);
	}
	return {x, y};
}

int num_deleted = 0;

treap_node* delete_by_point(treap_node* a, pt_t loc) {
	if (!a) return nullptr;
	assert(a);
	assert(a->seg[0] < loc && loc <= a->seg[1]);
	if (a->seg[1] == loc) {
		num_deleted ++;
		return merge(
			delete_by_point(a->c[0], loc),
			delete_by_point(a->c[1], loc)
		);
	} else if (is_below(loc, a->seg)) {
		treap_node* r = make_clone(a);
		r->c[0] = delete_by_point(a->c[0], loc);
		return r;
	} else {
		treap_node* r = make_clone(a);
		r->c[1] = delete_by_point(a->c[1], loc);
		return r;
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N; cin >> N;
		std::vector<int> par(N+1, -1);

		struct evt_t {
			pt_t loc;
			// op is: 0 for insert, 1 for change, 2 for delete
			int op;
			// polygon number
			int v;
			// 2 points to store in addition to loc
			pt_t p1, p2;
		};
		std::vector<evt_t> evts;
		for (int v = 1; v <= N; v++) {
			int M; cin >> M;
			std::vector<pt_t> pts(M);
			for (auto& p : pts) cin >> p;
			// we like CCW more than CW
			std::reverse(pts.begin(), pts.end());

			// rotate the min to the front
			{
				auto it = std::min_element(pts.begin(), pts.end());
				std::rotate(pts.begin(), it, pts.end());
			}
			auto it = std::max_element(pts.begin(), pts.end());
			assert(it != pts.end());
			std::vector<pt_t> lower(pts.begin(), it+1);
			std::vector<pt_t> upper(it, pts.end());
			upper.push_back(pts.front());
			std::reverse(upper.begin(), upper.end());

			for (int i = 0; i+1 < int(lower.size()); i++) assert(lower[i] < lower[i+1]);
			for (int i = 0; i+1 < int(upper.size()); i++) assert(upper[i] < upper[i+1]);
			assert(lower.front() == upper.front());
			assert(lower.back() == upper.back());

			evts.push_back({ lower.front(), 0, v, lower[1], upper[1] });
			evts.push_back({ lower.back(), 2, v, lower.end()[-1], upper.end()[-1] });
			for (int i = 1; i+1 < int(lower.size()); i++) {
				evts.push_back({ lower[i], 1, v, lower[i-1], lower[i+1] });
			}
			for (int i = 1; i+1 < int(upper.size()); i++) {
				evts.push_back({ upper[i], 1, v, upper[i-1], upper[i+1] });
			}
		}
		std::sort(evts.begin(), evts.end(), [&](const auto& a, const auto& b) { return a.loc < b.loc; });

		treap_node* root = nullptr;
		const int INF = int(1e9) + 10;
		root = merge(
			make_node({-1, -INF}, {INF, -INF}, 0),
			make_node({-1, INF}, {INF, INF}, -1)
		);
		std::vector<std::pair<pt_t, treap_node*>> roots;
		roots.push_back({{-INF, -INF}, root});
		for (const auto& evt : evts) {
			if (evt.op == 0) {
				// INSERT
				auto [x, y] = split_by_point(root, evt.loc);
				assert(x && y);
				int par_region = get_last(x)->nxt_region;
				par[evt.v] = par_region;
				treap_node *a = make_node(evt.loc, evt.p1, evt.v);
				treap_node *b = make_node(evt.loc, evt.p2, par_region);
				root = merge(merge(x, merge(a, b)), y);
			} else if (evt.op == 1) {
				// CHANGE
				root = replace(root, evt.loc, evt.p1, evt.p2);
			} else if (evt.op == 2) {
				// DELETE
				num_deleted = 0;
				root = delete_by_point(root, evt.loc);
				assert(num_deleted == 2);
			} else assert(false);
			roots.push_back({evt.loc, root});
		}

		auto get_region = [&](pt_t a) -> int {
			int mi = 0;
			int ma = int(roots.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				assert(a != roots[md].first);
				if (a < roots[md].first) ma = md;
				else mi = md;
			}
			treap_node* cur = roots[mi].second;
			assert(cur);
			treap_node* last_smaller = nullptr;
			while (cur) {
				if (is_below(a, cur->seg)) {
					cur = cur->c[0];
				} else {
					last_smaller = cur;
					cur = cur->c[1];
				}
			}
			assert(last_smaller);
			return last_smaller->nxt_region;
		};

		std::vector<std::vector<int>> ch(N+1);
		for (int i = 1; i <= N; i++) {
			assert(par[i] != -1);
			//cerr << "par " << i << " = " << par[i] << '\n';
			ch[par[i]].push_back(i);
		}

		using hsh = uint64_t;

		// second coord is size
		std::vector<hsh> subtree_hash(N+1);
		std::y_combinator([&](auto self, int cur) -> void {
			hsh tot = 1;
			for (int nxt : ch[cur]) {
				self(nxt);
				tot += subtree_hash[nxt];
			}
			subtree_hash[cur] = splitmix64_hash::splitmix64(tot);
		})(0);

		std::vector<hsh> tree_hash(N+1);
		std::y_combinator([&](auto self, int cur, hsh par_hash) -> void {
			tree_hash[cur] = splitmix64_hash::splitmix64(subtree_hash[cur] + par_hash * uint64_t(0x20980982351));
			for (int nxt : ch[cur]) {
				self(nxt, tree_hash[cur]);
			}
		})(0, hsh(0));

		auto query = [&](pt_t PA, pt_t PB) -> bool {
			int a = get_region(PA);
			int b = get_region(PB);
			//cerr << a << ' ' << b << '\n';
			return tree_hash[a] == tree_hash[b];
		};

		int Q; cin >> Q;
		int tot_ans = 0;
		int cur_xor = 0;
		while (Q--) {
			pt_t X, Y; cin >> X >> Y;
			X.x ^= cur_xor;
			X.y ^= cur_xor;
			Y.x ^= cur_xor;
			Y.y ^= cur_xor;
			assert(0 <= X.x && X.x <= int(1e9));
			assert(0 <= X.y && X.y <= int(1e9));
			assert(0 <= Y.x && Y.x <= int(1e9));
			assert(0 <= Y.y && Y.y <= int(1e9));
			int E; cin >> E;
			bool ans = query(X, Y);
			if (ans) cur_xor ^= E;
			tot_ans += ans;
		}
		cout << "Case #" << case_num << ": " << tot_ans << '\n';

		free_all();
	}

	return 0;
}
