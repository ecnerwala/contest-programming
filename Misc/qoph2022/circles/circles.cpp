#include <bits/stdc++.h>

#include "geodeb.h"

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
	friend std::ostream& operator<<(std::ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

typedef Point<double> P;
double ccRadius(const P& A, const P& B, const P& C) {
	return (B-A).dist()*(C-B).dist()*(A-C).dist()/
			std::abs((B-A).cross(C-A))/2;
}
P ccCenter(const P& A, const P& B, const P& C) {
	P b = C-A, c = B-A;
	return A + (b*c.dist2()-c*b.dist2()).perp()/b.cross(c)/2;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	GD_INIT("output.html");

	const int N = 63;
	std::vector<P> ps(N);
	for (int i = 0; i < N; i++) {
		cin >> ps[i].x >> ps[i].y;
		ps[i].y *= -1;
		GD_POINT(ps[i].x, ps[i].y);
	}

	std::vector<bool> done(N);
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			for (int k = j+1; k < N; k++) {
				if (done[i] || done[j] || done[k]) continue;
				double r = ccRadius(ps[i], ps[j], ps[k]);
				P center = ccCenter(ps[i], ps[j], ps[k]);
				std::vector<int> cur;
				for (int l = k+1; l < N; l++) {
					if (std::abs((ps[l] - center).dist() - r) <= 1e-8) {
						if (cur.empty()) {
							cur.push_back(i);
							cur.push_back(j);
							cur.push_back(k);
						}
						cur.push_back(l);
					}
				}
				if (cur.empty()) continue;
				GD_CIRCLE(center.x, center.y, r);
				GD_POINT(center.x, center.y, "red");
				for (int idx : cur) done[idx] = true;
				std::cerr << center.dist() << ' ' << r << '\n';
				std::vector<P> cur_pts; cur_pts.reserve(cur.size());
				for (int idx : cur) cur_pts.push_back(ps[idx] - center);
				std::sort(cur_pts.begin(), cur_pts.end(), [&](P a, P b) { return a.angle() < b.angle(); });
				std::vector<double> angs;
				for (int z = 0; z+1 < 7; z++) {
					angs.push_back((cur_pts[z+1].angle() - cur_pts[z].angle()) * r);
				}
				angs.push_back((cur_pts[0].angle() + 2 * M_PI - cur_pts[6].angle()) * r);
				std::sort(angs.begin(), angs.end());
			}
		}
	}

	return 0;
}
