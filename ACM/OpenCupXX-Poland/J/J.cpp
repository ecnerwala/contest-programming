#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

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
};

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
template <class P>
bool isReflexCW(P s, P t) {
	auto c = s.cross(t);
	return c ? (c > 0) : (s.dot(t) < 0);
}
// operator < (s,t) for angles in [base,base+2pi)
template <class P>
bool angleCmp(P base, P s, P t) {
	int r = isReflex(base, s) - isReflex(base, t);
	return r ? (r < 0) : (0 < s.cross(t));
}
// operator < (s,t) for angles in [base,base+2pi)
template <class P>
bool angleCmpCW(P base, P s, P t) {
	int r = isReflexCW(base, s) - isReflexCW(base, t);
	return r ? (r < 0) : (0 > s.cross(t));
}
// is x in [s,t] taken ccw? 1/0/-1 for in/border/out
template <class P>
int angleBetween(P s, P t, P x) {
	if (sameDir(x, s) || sameDir(x, t)) return 0;
	return angleCmp(s, x, t) ? 1 : -1;
}

using P = Point<ll>;

struct FT {
	vector<ll> s;
	FT(int n) : s(n) {}
	void update(int pos, ll dif) { // a[pos] += dif
		for (; pos < sz(s); pos |= pos + 1) s[pos] += dif;
	}
	ll query(int pos) { // sum of values in [0, pos)
		ll res = 0;
		for (; pos > 0; pos &= pos - 1) res += s[pos-1];
		return res;
	}
	int lower_bound(ll sum) {// min pos st sum of [0, pos] >= sum
		// Returns n if no sum is >= sum, or -1 if empty sum is.
		if (sum <= 0) return -1;
		int pos = 0;
		for (int pw = 1 << 25; pw; pw >>= 1) {
			if (pos + pw <= sz(s) && s[pos + pw-1] < sum)
				pos += pw, sum -= s[pos-1];
		}
		return pos;
	}
};

ll line_count(vector<P> pts, P p1, P p2, P p3, P p4){
	int n = (int)pts.size();
	if(n <= 1) return 0;
	vector<int> ord1(n), ord2(n), ord3(n), ord4(n);
	for(int i = 0; i < n; i++) ord1[i] = ord2[i] = ord3[i] = ord4[i] = i;
	sort(ord1.begin(), ord1.end(), [&](int a, int b) -> bool {
		return angleCmp(p2 - p1, pts[b] - p1, pts[a] - p1);
	});
	sort(ord2.begin(), ord2.end(), [&](int a, int b) -> bool {
		return angleCmp(p1 - p2, pts[a] - p2, pts[b] - p2);
	});
	sort(ord3.begin(), ord3.end(), [&](int a, int b) -> bool {
		return angleCmp(p4 - p3, pts[a] - p3, pts[b] - p3);
	});
	sort(ord4.begin(), ord4.end(), [&](int a, int b) -> bool {
		return angleCmp(p3 - p4, pts[b] - p4, pts[a] - p4);
	});
	vector<int> inv1(n), inv2(n), inv3(n), inv4(n);
	for(int i = 0; i < n; i++) inv1[ord1[i]] = inv2[ord2[i]] = inv3[ord3[i]] = inv4[ord4[i]] = i;

	vector<int> region(n);
	for(int i = 0; i < n; i++){
		bool x = !angleCmpCW(p2 - p1, pts[i] - p1, p3 - p1);
		bool y = angleCmp(p1 - p2, pts[i] - p2, p4 - p2);
		if(!x && y) region[i] = 1;
		if(!x && !y) region[i] = 2;
		if(x && !y) region[i] = 3;
		if(x && y) region[i] = 4;
	}

	ll ret = 0;
	{
		// A (left)
		FT ft(n+1);
		for(int j = 0; j < n; j++){
			int idx = ord3[j];
			if(region[idx] == 1) ret += ft.query(inv4[idx]);
			ft.update(inv4[idx], 1);
		}
	}
	{
		// B (top)
		FT ft(n+1);
		for(int j = 0; j < n; j++){
			int idx = ord3[j];
			if(region[idx] == 2) ret += ft.query(inv2[idx]);
			ft.update(inv2[idx], 1);
		}
	}
	{
		// C (right)
		FT ft(n+1);
		for(int j = 0; j < n; j++){
			int idx = ord1[j];
			if(region[idx] == 3) ret += ft.query(inv2[idx]);
			ft.update(inv2[idx], 1);
		}
	}
	{
		// D (bottom)
		FT ft(n+1);
		for(int j = 0; j < n; j++){
			int idx = ord1[j];
			if(region[idx] == 4) ret += ft.query(inv4[idx]);
			ft.update(inv4[idx], 1);
		}
	}
	return ret;
}

void solve(){
	int n;
	cin >> n;
	vector<P> polygon(n);
	for(int i = 0; i < n; i++) cin >> polygon[i].x >> polygon[i].y;
	polygon.push_back(polygon.front());
	int m;
	cin >> m;
	vector<P> pts(m);
	for(int i = 0; i < m; i++) cin >> pts[i].x >> pts[i].y;
	int q;
	cin >> q;
	vector<ll> answers(q);
	for(int i = 0; i < q; i++){
		int a, b;
		cin >> a >> b;
		vector<P> real_pts;
		for(P p : pts){
			if((p - polygon[b]).cross(polygon[a+1] - polygon[b]) > 0) continue;
			if((p - polygon[a]).cross(polygon[b+1] - polygon[a]) > 0) continue;
			real_pts.push_back(p);
		}
		answers[i] = line_count(real_pts, polygon[a], polygon[a+1], polygon[b], polygon[b+1]);
	}
	for(int i = 0; i < q; i++){
		cout << answers[i] << " \n"[i == (q-1)];
	}
}

int main(){
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T;
	cin >> T;
	while(T--){
		solve();
	}
}
