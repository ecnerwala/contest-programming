#include <bits/stdc++.h>
using namespace std;

template<class T> struct Point3D {
	typedef Point3D P;
	typedef const P& R;
	T x, y, z;
	explicit Point3D(T x=0, T y=0, T z=0) : x(x), y(y), z(z) {}
	bool operator<(R p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z); }
	bool operator==(R p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z); }
	P operator+(R p) const { return P(x+p.x, y+p.y, z+p.z); }
	P operator-(R p) const { return P(x-p.x, y-p.y, z-p.z); }
	P operator*(T d) const { return P(x*d, y*d, z*d); }
	P operator/(T d) const { return P(x/d, y/d, z/d); }
	T dot(R p) const { return x*p.x + y*p.y + z*p.z; }
	P cross(R p) const {
		return P(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
	}
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
	//Azimuthal angle (longitude) to x-axis in interval [-pi, pi]
	double phi() const { return atan2(y, x); } 
	//Zenith angle (latitude) to the z-axis in interval [0, pi]
	double theta() const { return atan2(sqrt(x*x+y*y),z); }
	P unit() const { return *this/(T)dist(); } //makes dist()=1
	//returns unit vector normal to *this and p
	P normal(P p) const { return cross(p).unit(); }
	//returns point rotated 'angle' radians ccw around axis
	P rotate(double angle, P axis) const {
		double s = sin(angle), c = cos(angle); P u = axis.unit();
		return u*dot(u)*(1-c) + (*this)*c - cross(u)*s;
	}
};

using ld = long double;

const ld EPS = 1e-13;

using P = Point3D<ld>;

const ld PI = 4.0 * atan(1.0);

ld ang(ld t, P p1, P p2, P lamp, P dir){
	P p = (p2 - p1) * t + p1;
	P x = (p - lamp).unit();
	return x.dot(dir);
}

int main(){
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int n, m, k;
	cin >> n >> m >> k;
	vector<P> pts(n+1);
	for(int i = 0; i <= n; i++){
		cin >> pts[i].x >> pts[i].y;
		pts[i].z = 0;
	}
	vector<P> lamps(m);
	vector<P> dirs(m);
	for(int i = 0; i < m; i++){
		cin >> lamps[i].x >> lamps[i].y >> lamps[i].z;
		cin >> dirs[i].x >> dirs[i].y >> dirs[i].z;
		dirs[i] = dirs[i].unit();
	}

	vector<vector<vector<ld> > > saved_break_points(n, vector<vector<ld> >(m));
	for(int dummy = 0; dummy < n; dummy++){
		P p1 = pts[dummy];
		P p2 = pts[dummy+1];
		for(int j = 0; j < m; j++){
			P lamp = lamps[j];
			P dir = dirs[j];
			vector<ld> break_points;
			break_points.push_back(0);
			break_points.push_back(1);
			for(int which = 0; which <= 1; which++){
				ld s = 0;
				ld e = 1;
				for(int q = 0; q < 60; q++){
					ld m1 = (s + s + e) / 3;
					ld m2 = (s + e + e) / 3;
					if((ang(m1, p1, p2, lamp, dir) < ang(m2, p1, p2, lamp, dir)) == which){
						s = m1;
					} else {
						e = m2;
					}
				}
				ld m = (s + e) / 2;
				break_points.push_back(m);
			}
			sort(break_points.begin(), break_points.end());
			saved_break_points[dummy][j] = break_points;
		}
	}

	ld s = 1e-7;
	ld e = PI;
	for(int _ = 0; _ < 30; _++){
		ld check_angle = s * sqrt(e / s);
		ld check_cos = cos(check_angle);
		bool works = true;
		for(int dummy = 0; dummy < n; dummy++){
			vector<pair<ld,int> > events;
			for(int __ = 0; __ < k; __++){
				events.push_back({0, -1});
				events.push_back({1, 1});
			}
			P p1 = pts[dummy];
			P p2 = pts[dummy+1];


			for(int j = 0; j < m; j++){
				P lamp = lamps[j];
				P dir = dirs[j];
				// solve for the zeroes
				// p = t * (p2 - p1) + p1 - lamp
				// p dot dir / sqrt(p dot p) == check_cos
				// (p dot dir)^2 == p dot p * check_cos^2
				// p dot p * (check_cos)^2 - (p dot dir)^2 = 0
				ld ca = (p2 - p1).dist2() * check_cos * check_cos;
				ld cb = 2 * (p2 - p1).dot(p1 - lamp) * check_cos * check_cos;
				ld cc = (p1 - lamp).dot((p1 - lamp)) * check_cos * check_cos;
				ld ba = (p2 - p1).dot(dir);
				ld bb = (p1 - lamp).dot(dir);
				ca -= ba * ba;
				cb -= 2 * ba * bb;
				cc -= bb * bb;
				// we want roots between 0 and 1
				if (ca < 0) {
					ca = -ca, cb = -cb, cc = -cc;
				}
				vector<ld> roots;
				roots.push_back(0);
				if (abs(ca) < EPS) {
					if (abs(cb) < EPS) {
						// no roots
					} else {
						ld v = -cc / cb;
						if (0 < v && v < 1) roots.push_back(v);
					}
				} else {
					ld disc = cb * cb - 4 * ca * cc;
					if (disc >= 0) {
						ld rt = sqrt(disc);
						ld v1 = ((-cb - rt) / 2 / ca);
						ld v2 = ((-cb + rt) / 2 / ca);
						if (0 < v1 && v1 < 1) roots.push_back(v1);
						if (0 < v2 && v2 < 1) roots.push_back(v2);
					}
				}
				roots.push_back(1);
				for (int v = 0; v+1 < int(roots.size()); v++) {
					ld s = roots[v];
					ld e = roots[v+1];
					ld m = (s + e) / 2;
					if (ang(m, p1, p2, lamp, dir) >= check_cos) {
						events.emplace_back(s, 1);
						events.emplace_back(e, -1);
					}
				}
				/*
				vector<ld> break_points = saved_break_points[dummy][j];
				for(int v = 0; v + 1 < (int)break_points.size(); v++){
					ld s = break_points[v];
					ld e = break_points[v+1];
					ld sv = ang(s, p1, p2, lamp, dir);
					ld ev = ang(e, p1, p2, lamp, dir);
					if(sv < check_cos && ev < check_cos) continue;
					if(sv >= check_cos && ev >= check_cos){
						events.push_back({s, 1});
						events.push_back({e, -1});
						continue;
					}
					bool left = (sv >= check_cos);
					for(int q = 0; q < 60; q++){
						ld m = (s + e) / 2;
						ld mv = ang(m, p1, p2, lamp, dir);
						if((mv >= check_cos) == (sv >= check_cos)){
							s = m;
						} else {
							e = m;
						}
					}
					if(left){
						events.push_back({break_points[v], 1});
						events.push_back({s, -1});
					} else {
						events.push_back({e, 1});
						events.push_back({break_points[v+1], -1});
					}
				}
				*/
			}
			for(auto& x : events){
				if(x.second == 1){
					x.first -= EPS;
				}
			}

			sort(events.begin(), events.end());
			int cur = 0;
			for(auto r : events){
				cur += r.second;
				if(cur < 0) works = false;
			}
		}
		if(works){
			e = check_angle;
		} else {
			s = check_angle;
		}
	}

	ld ans = (s + e) / 2;
	ans /= PI;
	ans *= 180.0;
	if(!isfinite(ans)) ans = 180.0;
	ans = max(ans, ld(0.0));
	cout << fixed << setprecision(15);
	cout << ans << '\n';		
}
