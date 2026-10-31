#include<bits/stdc++.h>
using namespace std;

using ll = long long;

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

	friend std::ostream& operator << (std::ostream& o, const P& p) {
		return o << "(" << p.x << ", " << p.y << ")";
	}
	friend std::istream& operator >> (std::istream& i, P& p) {
		return i >> p.x >> p.y;
	}
};

template<class P>
bool segmentIntersectionQ(P s1, P e1, P s2, P e2) {
	if (e1 == s1) {
		if (e2 == s2) return e1 == e2;
		swap(s1,s2); swap(e1,e2);
	}
	P v1 = e1-s1, v2 = e2-s2, d = s2-s1;
	auto a = v1.cross(v2), a1 = d.cross(v1), a2 = d.cross(v2);
	if (a == 0) { // parallel
		auto b1 = s1.dot(v1), c1 = e1.dot(v1),
		     b2 = s2.dot(v1), c2 = e2.dot(v1);
		return !a1 && max(b1,min(b2,c2)) <= min(c1,max(b2,c2));
	}
	if (a < 0) { a = -a; a1 = -a1; a2 = -a2; }
	return (0 <= a1 && a1 <= a && 0 <= a2 && a2 <= a);
}

template <class P>
bool segmentIntersectionQ(pair<P, P> a, pair<P, P> b) {
	return segmentIntersectionQ(a.first, a.second, b.first, b.second);
}

using P = Point<ll>;
using vi = vector<int>;
#define sz(a) int((a).size())
#define all(a) (a).begin(), (a).end()
#define trav(a, x) for (auto& a : x)

pair<vi, vi> ulHull(const vector<P>& S) {
	vi Q(sz(S)), U, L;
	iota(all(Q), 0);                                                                                                                                                                         
	// already sorted
	//sort(all(Q), [&S](int a, int b){ return S[a] < S[b]; }); 
	trav(it, Q) {
#define ADDP(C, cmp) while (sz(C) > 1 && S[C[sz(C)-2]].cross(\
	S[it], S[C.back()]) cmp 0) C.pop_back(); C.push_back(it);
		ADDP(U, <=); ADDP(L, >=);
#undef ADDP
	}   
	return {U, L}; 
}
vi convexHull(const vector<P>& S) {
	vi u, l; tie(u, l) = ulHull(S);
	if (sz(S) <= 1) return u;
	if (S[u[0]] == S[u[1]]) return {0};
	l.insert(l.end(), u.rbegin()+1, u.rend()-1);
	return l;
}
vector<P> convexHullPts(const vector<P>& S) {
	vi inds = convexHull(S);
	vector<P> ans; ans.reserve(inds.size());
	for (int i : inds) {
		ans.push_back(S[i]);
	}
	return ans;
}

const int MAXN = 1.1e5;
int N;

vector<int> go() {
	cin >> N;
	vector<pair<P, int>> pts(2*N);
	for (int i = 0; i < 2 * N; i++) {
		cin >> pts[i].first;
		pts[i].second = i;
	}
	vector<int> ans(2*N, -1);
	for (int t = 0; t < N; t++) {
		auto it = min_element(pts.begin(), pts.end());
		P p = it->first;
		int x = it->second;
		pts.erase(it);

		//cerr << p << ' ' << x << '\n';
		sort(pts.begin(), pts.end(), [&](const auto& a, const auto& b) { return (a.first - p).cross(b.first - p) > 0; });

		it = pts.begin() + int(pts.size()) / 2; // midpoint
		//P q = it->first;
		int y = it->second;
		//cerr << q << ' ' << y << '\n';
		pts.erase(it);

		ans[x] = y;
		ans[y] = x;
	}
	assert(pts.empty());
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		auto ans = go();

		cout << "Case #" << case_num << ":";
		for (int i : ans) cout << ' ' << i+1;
		cout << '\n';
	}

	return 0;
}
