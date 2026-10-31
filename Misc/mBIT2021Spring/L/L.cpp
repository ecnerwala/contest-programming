#include <bits/stdc++.h>

template <typename T>
using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int signum(double a) {
	return (a > 0) - (a < 0);
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

using pt_t = Point<double>;
struct circ_t {
	pt_t p;
	double r;
	int d;
};

const double PI2 = atan2(1, 0);
const double PI = acos(-1);
const double TAU = 2 * PI;

struct treap_node {
	std::mt19937::result_type pri;
	int idx;
	pt_t p;
	std::array<treap_node*, 2> c{nullptr, nullptr};
	int val;
	int sum;
	bool deleted;
	void update() {
		sum = val + (c[0] ? c[0]->sum : 0) + (c[1] ? c[1]->sum : 0);
	}
};

pt_t treap_dir;

treap_node* merge(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

std::pair<treap_node*, treap_node*> split(treap_node* r, double v, int target_idx = -1) {
	if (!r) return {nullptr, nullptr};
	if (r->idx == target_idx) {
		assert(!r->deleted);
		std::pair<treap_node*, treap_node*> res{r->c[0], r->c[1]};
		r->c[0] = r->c[1] = nullptr;
		r->deleted = true;
		return res;
	}
	treap_node *a, *b;
	if (dot(r->p, treap_dir) < v) {
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], v, target_idx);
	} else {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], v, target_idx);
	}
	r->update();
	return {a, b};
}

std::pair<int, treap_node*> treap_insert(treap_node* a, treap_node* n) {
	assert(n->deleted);
	n->deleted = false;
	n->c[0] = n->c[1] = nullptr;
	n->update();
	auto [l, r] = split(a, dot(treap_dir, n->p));
	int v = l ? l->sum : 0;
	return {v, merge(merge(l, n), r)};
}

std::pair<int, treap_node*> treap_remove(treap_node* a, treap_node* n) {
	assert(!n->deleted);
	assert(a);
	auto [l, r] = split(a, dot(treap_dir, n->p), n->idx);
	assert(n->deleted);
	int v = l ? l->sum : 0;
	return {v, merge(l, r)};
}

const double INF = 1e99;
void validate_treap(treap_node* a, double LO = -INF, double HI = INF) {
	if (!a) return;
	double v = dot(treap_dir, a->p);
	assert(a->sum == a->val + (a->c[0] ? a->c[0]->sum : 0) + (a->c[1] ? a->c[1]->sum : 0));
	assert(LO <= v && v <= HI);
	validate_treap(a->c[0], LO, v);
	validate_treap(a->c[1], v, HI);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	N += 2;
	vector<circ_t> circles(N);
	for (int i = 0; i < N; i++) {
		auto& c = circles[i];
		if (i < 2) {
			cin >> c.p;
			c.r = 0, c.d = 0;
		} else {
			cin >> c.p >> c.r >> c.d;
		}
	}

	// sa == sb is external, else internal
	auto get_tangent = [](circ_t a, circ_t b, int sa, int sb) -> array<double, 3> {
		assert(sa == -1 || sa == 1);
		assert(sb == -1 || sb == 1);
		double d = abs(b.p - a.p);
		double theta = arg(b.p - a.p) + asin((b.r * sb - a.r * sa) / d);
		if (theta <= -PI) theta += TAU;
		if (theta > PI) theta -= TAU;
		assert(-PI <= theta && theta <= PI);

		double ta = theta + PI2 * sa;
		if (ta <= -PI) ta += TAU;
		if (ta > PI) ta -= TAU;

		double tb = theta + PI2 * sb;
		if (tb <= -PI) tb += TAU;
		if (tb > PI) tb -= TAU;

		return {ta, tb, theta};
	};
	auto get_pt = [](circ_t a, double theta) -> pt_t {
		return a.p + pt_t(std::polar(a.r, theta));
	};

	int V = 2;
	vector<vector<pair<double, int>>> angles(N);
	vector<vector<pair<int, double>>> adj(V); adj.reserve(8*N*N);

	auto get_vtx = [&](int i, double d) -> int {
		if (i < 2) return i;
		int resp = V++;
		adj.emplace_back();
		angles[i].push_back({d, resp});
		return resp;
	};

	{
		pt_t pi = circles[0].p;
		pt_t pj = circles[1].p;
		pt_t dir = pj - pi;
		double dir_len = abs(dir);
		double xi = dot(dir, pi);
		double xj = dot(dir, pj);
		auto intersects = [&](circ_t c) -> bool {
			if (abs(c.p - pi) < c.r) return true;
			if (abs(c.p - pj) < c.r) return true;
			double xc = dot(dir, c.p);
			if (signum(xc - xi) * signum(xc - xj) >= 0) return false;
			return abs(cross(dir, c.p - pi)) / dir_len < c.r;
		};
		double cost = abs(dir);
		for (int k = 2; k < N; k++) {
			if (intersects(circles[k])) cost += circles[k].d;
		}

		adj[0].push_back({1, cost});
		adj[1].push_back({0, cost});
	}

	mt19937 mt(48);
	vector<treap_node> nodes(N);
	for (int i = 0; i < N; i++) {
		nodes[i].pri = mt();
		nodes[i].p = circles[i].p;
		nodes[i].val = circles[i].d;
		nodes[i].idx = i;
		nodes[i].deleted = true;
	}
	for (int i = 0; i < N; i++) {
		auto ci = circles[i];
		for (int si : {-1, 1}) {
			if (i < 2 && si == 1) continue;
			int j0 = 2;
			vector<tuple<double, int, int, array<int, 2>, double>> evts;
			evts.reserve(2*(N-j0));
			treap_dir = pt_t(-1, 0);
			treap_node* ROOT = nullptr;
			for (int j = j0; j < N; j++) {
				if (j == i) continue;
				auto cj = circles[j];
				for (int sj : {-1, 1}) {
					auto [di, dj, dtangent] = get_tangent(ci, cj, si, sj);

					int vi, vj;
					if (i < j) {
						vi = get_vtx(i, di);
						vj = get_vtx(j, dj);
					} else {
						vi = -1, vj = -1;
					}

					pt_t pi = get_pt(ci, di);
					pt_t pj = get_pt(cj, dj);
					evts.push_back({dtangent, j, sj, {vi, vj}, abs(pj - pi)});
				}
				double width = get<0>(evts.end()[-1]) - get<0>(evts.end()[-2]);
				if (width < 0) width += TAU;
				assert(0 <= width && width <= PI);
				if (get<0>(evts.end()[-2]) > get<0>(evts.end()[-1])) {
					// insert j right now
					ROOT = treap_insert(ROOT, &nodes[j]).second;
				}
			}
			sort(evts.begin(), evts.end());
			for (auto [d, j, sj, verts, len] : evts) {
				validate_treap(ROOT);
				treap_dir = pt_t(std::polar(1., d));
				validate_treap(ROOT);
				int c;
				if (sj == -1) {
					std::tie(c, ROOT) = treap_insert(ROOT, &nodes[j]);
				} else {
					std::tie(c, ROOT) = treap_remove(ROOT, &nodes[j]);
				}
				if (verts[0] == -1) continue;
				//cerr << i << ',' << verts[0] << ' ' << j << ',' << verts[1] << ' ' << c << ' ' << len << '\n';
				adj[verts[0]].push_back({verts[1], c + len});
				adj[verts[1]].push_back({verts[0], c + len});
			}
			for (int j = j0; j < N; j++) {
				if (!nodes[j].deleted) nodes[j].deleted = true;
			}
		}
	}

	for (int k = 2; k < N; k++) {
		sort(angles[k].begin(), angles[k].end());
		for (int i = 0; i < int(angles[k].size()); i++) {
			auto [da, va] = angles[k][i];
			auto [db, vb] = angles[k][(i+1) % int(angles[k].size())];
			if (i + 1 == int(angles[k].size())) db += TAU;
			double d = min(db - da, TAU - (db - da)) * circles[k].r;
			adj[va].push_back({vb, d});
			adj[vb].push_back({va, d});
		}
	}

	vector<double> dist(V, INF);
	vector<bool> vis(V);
	min_pq<pair<double, int>> pq;
	pq.push({dist[0] = 0., 0});

	while (true) {
		auto [d, cur] = pq.top(); pq.pop();
		if (vis[cur]) continue;
		vis[cur] = true;
		if (cur == 1) break;
		for (auto [nxt, l] : adj[cur]) {
			l += d;
			if (l < dist[nxt]) {
				pq.push({dist[nxt] = l, nxt});
			}
		}
	}

	cout << fixed << setprecision(10) << dist[1] << '\n';

	return 0;
}
