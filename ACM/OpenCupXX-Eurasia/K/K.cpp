#include <bits/stdc++.h>
using namespace std;

using ld = long double;

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
	ld dist() const { return sqrt((ld)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	ld angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(ld a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
};


#define eps 1e-10
typedef Point<ld> P;

int lineIntersection(const P& s1, const P& e1, const P& s2,
		const P& e2, P& r) {
	if ((e1-s1).cross(e2-s2)) { //if not parallell
		r = s2-(e2-s2)*(e1-s1).cross(s2-s1)/(e1-s1).cross(e2-s2);
		return 1;
	} else
		return -((e1-s1).cross(s2-s1)==0 || s2==e2);
}



struct Line {
	P P1, P2;
	int color;
	// Right hand side of the ray P1 -> P2
	explicit Line(P a = P(), P b = P(), int c = -1) : P1(a), P2(b), color(c) {};
	P intpo(Line y) {
		P r;
		assert(lineIntersection(P1, P2, y.P1, y.P2, r) == 1);
		return r;
	}
	P dir() {
		return P2 - P1;
	}
	bool contains(P x) {
		return (P2 - P1).cross(x - P1) < eps;
	}
	bool out(P x) {
		return !contains(x);
	}
}; 

template<class T>
bool mycmp(Point<T> a, Point<T> b) {
	// return atan2(a.y, a.x) < atan2(b.y, b.x);
	if (a.x * b.x < 0)	return a.x < 0;
	if (abs(a.x) < eps) {
		if (abs(b.x) < eps) 	return a.y > 0 && b.y < 0;
		if (b.x < 0)	return a.y > 0;
		if (b.x > 0)	return true;
	}
	if (abs(b.x) < eps) {
		if (a.x < 0)	return b.y < 0;
		if (a.x > 0)	return false;
	}
	return a.cross(b) > 0;
}

bool cmp(Line a, Line b) {
	if (abs(a.dir().cross(b.dir())) < eps && a.dir().dot(b.dir()) > 0) {
		return a.out(b.P1);
	} else {
		return mycmp(a.dir(), b.dir());
	}
}
P ans;

bool do_intersection(vector <Line> b) {
	sort(b.begin(), b.end(), cmp);
	int n = (int)b.size();
	int q = 1, h = 0, i;
	vector <Line> c(b.size() + 10);
	for (i = 0; i < n; i++) {
		if (i > 0 && abs(b[i].dir().cross(b[i - 1].dir())) < eps)
			continue;
		while (q < h && b[i].out(c[h].intpo(c[h - 1])))	h--;
		while (q < h && b[i].out(c[q].intpo(c[q + 1])))	q++;
		c[++h] = b[i];
		if (q < h && abs(c[h].dir().cross(c[h - 1].dir())) < eps) {
			h--;
			if (b[i].out(c[h].P1))	c[h] = b[i];
		}
	}
	while (q < h - 1 && c[q].out(c[h].intpo(c[h - 1])))	h--;
	while (q < h - 1 && c[h].out(c[q].intpo(c[q + 1])))	q++;
	// Intersection is empty. This is sometimes different from the case when
	// the intersection area is 0. 

	if (h - q <= 1)	return false;
	c[h + 1] = c[q];
	int color = 0;
	for(int i = q; i <= h; i++){
		cout << c[i].color;
		color |= (1 << c[i].color);
	}
	cout << '\n';
	if(color != 7) return false;
	vector <P> s(3);
	s[0] = P(0, 0);
	s[1] = P(1, 1);
	s[2] = P(2, 2);
	for(int i = q; i <= h; i++){
		if(c[i].color != c[i+1].color){
			s[(c[i].color + c[i+1].color) % 3] = c[i].intpo(c[i+1]);
		}
	}
	ld f0 = (s[0] - s[1]).dist();
	ld f1 = (s[1] - s[2]).dist();
	ld f2 = (s[2] - s[0]).dist();
	if(f0 <= f1 && f0 <= f2){
		ans = (s[0] + s[1]) / 2;
	} else if(f1 <= f2){
		ans = (s[1] + s[2]) / 2;
	} else {
		ans = (s[2] + s[0]) / 2;
	}
	return true;
}

int main(){
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int n;
	cin >> n;
	vector<P> pt(n);
	for(int i = 0; i < n; i++){
		cin >> pt[i].x >> pt[i].y;
	}
	vector<int> r(3);
	for(int i = 0; i < 3; i++){
		cin >> r[i]; r[i]--;
	}
	sort(r.begin(), r.end());
	vector<Line> lines;
	for(int i = 0; i < n; i++){
		int idx = 2;
		if(r[0] <= i && i+1 <= r[1]) idx = 0;
		if(r[1] <= i && i+1 <= r[2]) idx = 1;
		Line l(pt[(i+1)%n], pt[(i)%n], idx);
		lines.push_back(l);
	}
	do_intersection(lines);
	ld s = 0;
	ld e = 1e7;
	for(int i = 0; i < 100; i++){
		ld m = sqrt(s * e);
		vector<Line> new_lines = lines;
		for(Line& x : new_lines){
			P f = (x.P2 - x.P1).normal() * m;
			x.P1 = x.P1 + f;
			x.P2 = x.P2 + f;
		}
		bool ok = do_intersection(new_lines);
		if(ok){
			s = m;
		} else {
			e = m;
		}
	}
	cout << s << endl;
	cout << fixed << setprecision(15);
	cout << "Yes" << '\n';
	cout << ans.x << " " << ans.y << '\n';
}
