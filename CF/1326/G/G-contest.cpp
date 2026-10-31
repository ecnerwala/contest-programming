#include<bits/stdc++.h>
using namespace std;

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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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

using num = modnum<998244353>;

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x_=0, T y_=0) : x(x_), y(y_) {}
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

using P = Point<long long>;

template <class P>
bool sameDir(P s, P t) {
	return s.cross(t) == 0 && s.dot(t) > 0;
}
// checks 180 <= s..t < 360?
template <class P>
bool isReflex(P s, P t) {
	auto c = s.cross(t);
	return c ? (c < 0) : (s.dot(t) < 0);
}
// operator < (s,t) for angles in [base,base+2pi)
template <class P>
bool angleCmp(P base, P s, P t) {
	int r = isReflex(base, s) - isReflex(base, t);
	return r ? (r < 0) : (0 < s.cross(t));
}
// is x in [s,t] taken ccw? 1/0/-1 for in/border/out
template <class P>
int angleBetween(P s, P t, P x) {
	if (sameDir(x, s) || sameDir(x, t)) return 0;
	return angleCmp(s, x, t) ? 1 : -1;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<P> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y;
	}

	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	//for (int i = 0; i < N; i++) { for (int j = 0; j < N; j++) { cerr << canAdj[i][j] << ' '; } cerr << '\n'; }

	const int ROOT = 0;
	vector<int> st(N), en(N);
	vector<vector<int>> ch(N);

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		sort(adj[cur].begin(), adj[cur].end(), [&](int a, int b) -> bool {
			return angleCmp(pts[prv] - pts[cur], pts[a] - pts[cur], pts[b] - pts[cur]);
		});
		if (prv != -1) {
			assert(adj[cur][0] == prv);
			adj[cur].erase(adj[cur].begin());
		}
		//cerr << "children of " << cur << "; "; for (int nxt : adj[cur]) { cerr << nxt << ' '; } cerr << '\n';
		for (int nxt : adj[cur]) {
			assert(nxt != prv);
			self(nxt, cur);
		}
		en[cur] = int(preorder.size());
	})(ROOT, -1);

	vector<vector<bool>> canAdj(N, vector<bool>(N));
	for (int rt = 0; rt < N; rt++) {
		vector<P> stk;
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			stk.push_back(pts[cur]);
			canAdj[rt][cur] = true;
			for (P pt : stk) {
				if (pt.cross(pts[rt], pts[cur]) >= 0) {
					// keep going
				} else {
					canAdj[rt][cur] = false;
					break;
				}
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
			stk.pop_back();
		})(rt, -1);
	}


	vector<num> subtreeWays(N);
	vector<num> arcWays(N*N);
	vector<num> curDp(N*N);
	vector<num> prefWays(N, 1);
	vector<num> suffWays(N, 1);

	std::y_combinator([&](auto self, int cur) -> void {
		for (int nxt : adj[cur]) {
			self(nxt);
		}

		subtreeWays[cur] = 1; // just yourself
		for (int nxt : adj[cur]) {
			subtreeWays[cur] *= subtreeWays[nxt];
		}

		for (int nxt : adj[cur]) {
			num curWays = 0;

			for (int nxt2 : adj[cur]) if (nxt2 != nxt) curWays *= subtreeWays[nxt2];
			subtreeWays[cur] += curWays;
		}

		// update arcWays for 2 children
		for (int i = 0; i < int(adj[cur].size()); i++) {
			num topWays = 1;
			for (int j = i+1; j < int(adj[cur].size()); topWays *= subtreeWays[adj[cur][j]], j++) {
				for (int zi = st[i]; zi < en[i]; zi++) {
					for (int zj = st[j]; zj < en[j]; zj++) {
						arcWays[preorder[zi]*N+preo]
					}
				}
			}
		}
	})(ROOT);

	cout << subtreeWays[ROOT] << '\n';

	return 0;
}
