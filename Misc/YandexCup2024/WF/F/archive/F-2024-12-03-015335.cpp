#include <bits/stdc++.h>

#ifdef ECNERWALA
#include "geodeb.h"
#endif

namespace kactl {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, top = me;
	for (auto [y, e] : ed[at]) if (e != par) {
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(e);
		} else {
			int si = sz(st);
			int up = dfs(y, e, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(e);
				f(vi(st.begin() + si, st.end()));
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else { /* e is a bridge */ }
		}
	}
	return top;
}

template<class F>
void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) dfs(i, -1, f);
}

#undef rep
#undef all
#undef sz
}

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

using pt_t = Point<int>;

using std::cerr;

struct union_find {
	std::vector<int> par;
	union_find() {}
	union_find(int N) : par(N, -1) {}
	int get_par(int a) {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	}
	bool merge(int a, int b) {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
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

double solve(std::vector<pt_t> P, std::vector<pt_t> Q) {
	int NP = int(P.size());
	int NQ = int(Q.size());
	int N = NP + NQ;
	std::vector<pt_t> pts; pts.reserve(N);
	pts.insert(pts.end(), P.begin(), P.end());
	pts.insert(pts.end(), Q.begin(), Q.end());

#ifdef ECNERWALA
	for (auto p : P) {
		GD_POINT(p.x, p.y, "black");
	}
	for (auto p : Q) {
		GD_POINT(p.x, p.y, "red");
	}
#endif

	std::vector<std::pair<int, std::array<int, 2>>> segs;
	segs.reserve(N * (N-1) / 2);
	for (int p1 = 0; p1 < N; p1++) {
		for (int p2 = p1+1; p2 < N; p2++) {
			segs.push_back({(pts[p1] - pts[p2]).dist2(), {p2, p1}});
		}
	}
	std::sort(segs.begin(), segs.end());
	union_find must_take(N);
	std::vector<std::vector<int>> adj(N);
	union_find all_edges(N);

	int real_cnt = 0;
	auto get_contrib = [&](int p1, int p2) -> std::pair<double, bool> {
		if (all_edges.get_par(p1) != all_edges.get_par(p2)) {
			return {1, true};
		}

		union_find cur_must = must_take;
		if (p1 >= NP) for (int nxt : adj[p1]) cur_must.merge(p1, nxt);
		if (p2 >= NP) for (int nxt : adj[p2]) cur_must.merge(p2, nxt);

		if (cur_must.get_par(p1) == cur_must.get_par(p2)) {
			return {0, false};
		}
		real_cnt++;

#ifdef ECNERWALA
		GD_LAYER();
#endif
		std::vector<bool> vis(N);
		std::vector<std::pair<int, int>> cc_edges;
		cc_edges.push_back({p1, p2});
		std::y_combinator([&](auto self, int cur) -> void {
			if (vis[cur]) return;
			vis[cur] = true;
			for (int nxt : adj[cur]) {
				cc_edges.push_back({cur, nxt});
				self(nxt);
			}
		})(p1);
		assert(vis[p2]);
		std::vector<std::pair<int, int>> bcc_edges;
		{
			kactl::ed = std::vector<std::vector<std::pair<int, int>>>(N);
			for (int e = 0; e < int(cc_edges.size()); e++) {
				auto [cur, nxt] = cc_edges[e];
				cur = cur_must.get_par(cur);
				nxt = cur_must.get_par(nxt);
				if (cur != nxt) {
					kactl::ed[cur].push_back({nxt, e});
					kactl::ed[nxt].push_back({cur, e});
				}
			}
			kactl::bicomps([&](auto lst) -> void {
				if (std::find(lst.begin(), lst.end(), 0) != lst.end()) {
					assert(bcc_edges.empty());
					for (auto e : lst) {
						if (e == 0) continue;
						bcc_edges.push_back(cc_edges[e]);
					}
				}
			});
		}
		std::set<int> bigs;
		for (auto vs : bcc_edges) {
			for (auto u : {vs.first, vs.second}) {
				if (u >= NP && u != p1 && u != p2) bigs.insert(u);
			}
		}

#ifdef ECNERWALA
		for (auto [i, j] : bcc_edges) {
			GD_SEGMENT(pts[i].x, pts[i].y, pts[j].x, pts[j].y);
		}
		GD_SEGMENT(pts[p1].x, pts[p1].y, pts[p2].x, pts[p2].y, "green bold");
#endif

		cerr << "have " << int(bigs.size()) << " points to brute force" << '\n';

		std::vector<int> big_id(N, -1);
		{
			int c = 0;
			for (auto v : bigs) {
				big_id[v] = c++;
			}
		}

		if (int(bigs.size()) >= 31) {
			cerr << "WARNING! TOO BIG!!!!!" << '\n';
			return {1, true};
		}

		int ways = 0;
		for (int msk = 0; msk < (1 << int(bigs.size())); msk++) {
			union_find cur = cur_must;
			for (auto [u, v] : bcc_edges) {
				if (big_id[u] != -1 && !((msk >> big_id[u]) & 1)) {
					// don't take this guy
					continue;
				}
				if (big_id[v] != -1 && !((msk >> big_id[v]) & 1)) {
					// don't take this guy
					continue;
				}
				cur.merge(u, v);
			}
			if (cur.get_par(p1) != cur.get_par(p2)) {
				ways++;
			}
		}

		int num_big = int(bigs.size());
		cerr << "prob " << ways << '/' << (1 << num_big) << '\n';

		return {ways / double(1 << num_big), true};
	};

	double ans = 0;
	for (int cur_idx = 0; cur_idx < int(segs.size()); cur_idx++) {
		auto [p1, p2] = segs[cur_idx].second;
		auto [prob, ever_used] = get_contrib(p1, p2);
		if (p1 >= NP) prob /= 2;
		if (p2 >= NP) prob /= 2;
		ans += std::sqrt(segs[cur_idx].first) * prob;

		if (ever_used) {
			cerr << "adding " << p1 << ' ' << p2 << ' ' << std::sqrt(segs[cur_idx].first) << ' ' << prob << '\n';
			all_edges.merge(p1, p2);
			if (p1 < NP && p2 < NP) {
				must_take.merge(p1, p2);
			}
			adj[p1].push_back(p2);
			adj[p2].push_back(p1);
		}
	}

	cerr << "real_cnt " << real_cnt << '\n';

	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	cerr << fixed << setprecision(20);
	cout << fixed << setprecision(20);

	int T; cin >> T;
#ifdef ECNERWALA
	GD_INIT("test" + std::to_string(T / 10) + std::to_string(T % 10) + ".html");
	GD_LOG_TO_STDERR(false);
#endif
	int N; cin >> N;
	std::vector<pt_t> P(N);
	for (auto& p : P) cin >> p;
	int M; cin >> M;
	std::vector<pt_t> Q(M);
	for (auto& q : Q) cin >> q;

	if (false && T == 1) {
		cout << "6.38908729652601138" << '\n';
	} else if (false && T == 2) {
		cout << (N-1) + double(M) * 0.5 << '\n';
	} else {
		double res = solve(P, Q);
		cerr << res << '\n';
		cout << res << '\n';
	}

	return 0;
}
