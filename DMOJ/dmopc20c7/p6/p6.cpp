#include <bits/extc++.h>

template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<
	K, V, Comp,
	__gnu_pbds::rb_tree_tag
>;

template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;
namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct UF {
	vi e;
	UF(int n) : e(n, -1) {}
	bool sameSet(int a, int b) { return find(a) == find(b); }
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b]; e[b] = a;
		return true;
	}
};

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

typedef Point<ll> P;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<kactl::P> pts(N);
	for (auto& p : pts) cin >> p.x >> p.y;

	using kactl::P;
	sort(all(pts), [](P a, P b) { return a.y < b.y; });

	auto is_good = [&](int64_t min_dist) -> bool {
		kactl::UF color_uf(2*N);
		order_statistic_map<kactl::P, int> S;
		int j = 0;
		P d{1 + (int64_t)sqrt(min_dist), 0};
		for (int i = 0; i < N; i++) {
			P p = pts[i];
			while (pts[j].y <= p.y - d.x) S.erase(pts[j++]);
			auto lo = S.lower_bound(p - d), hi = S.upper_bound(p + d);
			for (; lo != hi; ++lo) {
				if ((lo->first - p).dist2() < min_dist) {
					int x = i, y = lo->second;
					if (color_uf.sameSet(2*x, 2*y)) return false;
					color_uf.join(2*x, 2*y+1);
					color_uf.join(2*x+1, 2*y);
				}
			}
			S.insert({p, i});
		}
		return true;
	};

	int64_t mi = 1;
	int64_t ma = 8.01e18;
	while (ma - mi > 1 && ma > 4 * mi) {
		int64_t md = int64_t(mi * sqrt(ma / mi));
		assert(mi < md && md < ma);
		if (is_good(md)) mi = md;
		else ma = md;
	}
	//assert(!is_good(ma));

	vector<pair<int64_t, pair<int, int>>> edges;

	{
		order_statistic_map<kactl::P, int> S;
		int j = 0;
		P d{1 + (int64_t)sqrt(ma), 0};
		for (int i = 0; i < N; i++) {
			P p = pts[i];
			while (pts[j].y <= p.y - d.x) S.erase(pts[j++]);
			auto lo = S.lower_bound(p - d), hi = S.upper_bound(p + d);
			for (; lo != hi; ++lo) {
				int64_t len = (lo->first - p).dist2();
				if (len < ma) {
					int a = lo->second, b = i;
					edges.push_back({len, {a, b}});
				}
			}
			S.insert({p, i});
		}
	}

	kactl::UF uf(N);
	kactl::UF color_uf(2*N);
	sort(edges.begin(), edges.end());
	int64_t ans = ma;
	for (auto [l, p] : edges) {
		auto [i, j] = p;
		assert(l == (pts[i] - pts[j]).dist2());
		//cerr << l << ' ' << i << '-' << j << '\n';
		if (uf.join(i, j)) {
			color_uf.join(2*i, 2*j+1);
			color_uf.join(2*i+1, 2*j);
		} else {
			assert(color_uf.sameSet(2*i, 2*j) || color_uf.sameSet(2*i, 2*j+1));
			if (color_uf.sameSet(2*i, 2*j)) {
				ans = l;
				break;
			}
		}
	}

	assert(ans < ma);
	cout << ans << '\n';

	return 0;
}
