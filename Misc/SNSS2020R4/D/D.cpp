#include<bits/extc++.h>

const double INF = 1e9;

void wssert(bool b) {
	if (!b) {
		std::cout << "GARBAGE" << '\n';
		exit(0);
	}
}

using namespace std;
template <typename flow_t = int, typename cost_t = long long>
struct MCMF_SSPA {

	int N;
	vector<vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
	};
	vector<edge_t> edges;

	vector<char> seen;
	vector<cost_t> pi;
	vector<int> prv;

	explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

	void addEdge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	const cost_t INF_COST = numeric_limits<cost_t>::max() / 4;
	const flow_t INF_FLOW = numeric_limits<flow_t>::max() / 4;
	vector<cost_t> dist;
	__gnu_pbds::priority_queue<pair<cost_t, int>> q;
	vector<typename decltype(q)::point_iterator> its;
	void path(int s) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({0, s});

		while (!q.empty()) {
			int i = q.top().second; q.pop();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						prv[j] = e;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		swap(pi, dist);
	}

	pair<flow_t, cost_t> maxflow(int s, int t) {
		assert(s != t);
		flow_t totFlow = 0; cost_t totCost = 0;
		while (path(s), pi[t] < INF_COST) {
			flow_t curFlow = numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = min(curFlow, edges[e].cap);
				cur = nxt;
			}
			totFlow += curFlow;
			totCost += pi[t] * curFlow;
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				edges[e].cap -= curFlow;
				edges[e^1].cap += curFlow;
				cur = nxt;
			}
		}
		return {totFlow, totCost};
	}
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
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	friend T norm(const Point& a) { return a.x * a.x + a.y * a.y; }
	friend auto abs(const Point& p) { return std::hypot(p.x, p.y); }
	friend T unit(const Point& a) { if (a == Point()) return a; return a / abs(a); }

	friend T int_norm(const Point& a) { return __gcd(a.x,a.y); }
	friend T int_unit(const Point& a) { if (a == Point()) return a; return a / int_norm(a); }

	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T area(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Returns conj(a) * b
	friend Point rotation(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }

	friend Point complex_prod(const Point& a, const Point& b) { return rotation(Point(a.x, -a.y), b); }

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	using pt = Point<double>;
	struct circ {
		pt c; int r;
	};

	array<circ, 2> circles;
	for (int z = 0; z < 2; z++) {
		int x, y, r; cin >> x >> y >> r;
		circles[z].c = pt(x, y);
		circles[z].r = r;
	}

	const double EPS = 1e-9;

	auto pt_seg_dist = [EPS](pt c, array<pt, 2> s) -> double {
		pt dir = s[1] - s[0];
		s[0] -= c;
		s[1] -= c;
		double v0 = dot(s[0], dir);
		double v1 = dot(s[1], dir);
		wssert(v0 <= v1);
		if (0 <= v0) {
			return abs(s[0]);
		} else if (v1 <= 0) {
			return abs(s[1]);
		} else {
			double l = abs(dir);
			if (l < EPS) {
				return abs(s[0]);
			}
			return abs(cross(s[0], s[1])) / l;
		}
	};

	auto intersect = [EPS](array<pt, 2> a, array<pt, 2> b) -> std::optional<pt> {
		double c0 = cross(a[1] - a[0], b[0] - a[0]);
		double c1 = cross(a[1] - a[0], b[1] - a[0]);
		if (abs(c1 - c0) < EPS) {
			return std::nullopt;
		}
		return b[0] + (b[1] - b[0]) / (c1 - c0) * (-c0);
	};

	auto get_tangent = [](pt p, circ c) -> array<pt, 2> {
		double l = abs(p - c.c);
		wssert(l >= c.r);
		return {p + complex_prod(pt(c.r, sqrt(l * l - c.r * c.r)), c.c - p), p + complex_prod(pt(c.r, -sqrt(l * l - c.r * c.r)), c.c - p)};
	};

	auto get_candidates = [&](pt p) -> array<pt, 4> {
		auto x = get_tangent(p, circles[0]);
		auto y = get_tangent(p, circles[1]);
		return {x[0], x[1], y[0], y[1]};
	};

	array<vector<pt>, 2> players;
	for (int z = 0; z < 2; z++) {
		players[z].resize(N);
		for (auto& p : players[z]) {
			int x, y; cin >> x >> y;
			p = pt(x, y);
		}
	}

	vector<vector<double>> costs(N, vector<double>(N, INF));

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			pt a = players[0][i];
			pt b = players[1][j];
			auto check_common = [&](pt c) {
				for (int z = 0; z < 2; z++) {
					if (circles[z].r - pt_seg_dist(circles[z].c, {a, c}) > EPS) return;
					if (circles[z].r - pt_seg_dist(circles[z].c, {b, c}) > EPS) return;
				}
				costs[i][j] = min(costs[i][j], abs(a-c) + abs(b-c));
			};

			check_common((a + b) / 2);
			auto la = get_candidates(a);
			auto lb = get_candidates(b);
			for (pt pa : la) {
				for (pt pb : lb) {
					auto p = intersect({a, pa}, {b, pb});
					if (!p) continue;
					check_common(*p);
				}
			}
		}
	}

	MCMF_SSPA<int, double> flower(2*N+2);
	int S = 2*N, T = 2*N+1;
	for (int i = 0; i < N; i++) {
		flower.addEdge(S, i, 1, 0);
		flower.addEdge(N+i, T, 1, 0);
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			flower.addEdge(i, N+j, 1, costs[i][j]);
		}
	}

	double res = flower.maxflow(S, T).second;
	if (res >= INF) cout << -1 << '\n';
	else cout << fixed << setprecision(10) << res << '\n';


	return 0;
}
