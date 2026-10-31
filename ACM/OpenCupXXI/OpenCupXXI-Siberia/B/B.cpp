#include <bits/stdc++.h>
using namespace std;

using ld = long double;

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
	ld dist() const { return sqrt((ld)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	ld angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(ld a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

using P = Point<ld>;

using ll = long long;

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int v, t;
	cin >> v >> t;
	vector<P> vert(v);
	vector<int> z(v);

	const int MAXZ = (1 << 20);
	int shift = MAXZ / 2;
	for(int i = 0; i < v; i++){
		cin >> vert[i].x >> vert[i].y >> z[i];
		z[i] += shift;
	}
	vector<ld> face_area(MAXZ, 0);

	vector<array<ld, 3>> seg(MAXZ * 2);
	auto seg_add = [&](int l, int r, auto get_val) {
		auto lv = get_val(l);
		auto rv = get_val(r);
		for (int a = MAXZ+l, b = MAXZ+r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				int nl = ((a+1) << (__builtin_clz(a) - __builtin_clz(MAXZ))) - MAXZ;
				seg[a][0] += lv;
				seg[a][1] += get_val((l + nl) * 0.5);
				seg[a][2] += (lv = get_val(nl));
				l = nl;
				a++;
			}
			if (b & 1) {
				--b;
				int nr = (b << (__builtin_clz(b) - __builtin_clz(MAXZ))) - MAXZ;
				seg[b][2] += rv;
				seg[b][1] += get_val((nr + r) * 0.5);
				seg[b][0] += (rv = get_val(nr));
				r = nr;
			}
		}
	};

	auto seg_push = [&]() {
		for (int a = 1; a < MAXZ; a++) {
			seg[2*a+0][0] += seg[a][0];
			seg[2*a+0][1] += 0.375 * seg[a][0] + 0.75 * seg[a][1] - 0.125 * seg[a][2];
			seg[2*a+0][2] += seg[a][1];
			seg[2*a+1][0] += seg[a][1];
			seg[2*a+1][1] += 0.375 * seg[a][2] + 0.75 * seg[a][1] - 0.125 * seg[a][0];
			seg[2*a+1][2] += seg[a][2];
		}
	};

	for(int _ = 0; _ < t; _++){
		array<int, 3> idx;
		cin >> idx[0] >> idx[1] >> idx[2];
		array<int, 3> wh{0,1,2};
		sort(wh.begin(), wh.end(), [&](int a, int b){
			return z[idx[a]] < z[idx[b]];
		});
		int sign = (wh[1] - wh[0] + 3) % 3;
		if(sign == 2){
			sign = -1;
		}
		sign *= -1;
		array<int, 3> zi;
		array<P, 3> vi;
		for(int i = 0; i < 3; i++){
			zi[i] = z[idx[wh[i]]];
			vi[i] = vert[idx[wh[i]]];
		}
		assert(zi[0] <= zi[1] && zi[1] <= zi[2]);
		if(zi[0] == zi[1] && zi[1] == zi[2]){
			// nothing
			face_area[zi[0]] += abs(vi[0].cross(vi[1], vi[2]));
		}
		if (zi[0] != zi[1]){
			P dm = (vi[2] - vi[0]) / (zi[2] - zi[0]);
			P d1 = (vi[1] - vi[0]) / (zi[1] - zi[0]);
			seg_add(zi[0], zi[1], [&](ld nz) -> ld {
				nz -= zi[0];
				return (d1 * nz + vi[0]).cross(dm * nz + vi[0]) * sign;
			});
		}
		if (zi[1] != zi[2]){
			P dm = (vi[2] - vi[0]) / (zi[2] - zi[0]);
			P d1 = (vi[2] - vi[1]) / (zi[2] - zi[1]);
			seg_add(zi[1], zi[2], [&](ld nz) -> ld {
				nz = zi[2] - nz;
				return (vi[2] - d1 * nz).cross(vi[2] - dm * nz) * sign;
			});
		}
	}
	seg_push();

	ld best = -1, best_loc = -1;
	for(int i = 0; i < MAXZ; i++){
		if (i) {
			ld at_z = (seg[MAXZ+i-1][2] + seg[MAXZ+i][0] + face_area[i]) * 0.5;
			if(at_z > best){
				best = at_z;
				best_loc = i;
			}
		}

		auto [lo, md, hi] = seg[MAXZ+i];
		ld a = (hi + lo) / 2 - md;
		ld b = (hi - lo) / 2;
		ld c = md;

		ld cnd = - b / (2 * a);
		if (-1 < cnd && cnd < 1) {
			ld val = c + cnd * (b + cnd * a);
			if(val > best){
				best = val;
				best_loc = i + (cnd + 1) / 2;
			}
		}
	}
	best_loc -= shift;
	cout << fixed << setprecision(15);
	cout << best_loc << ' ' << best/2 << '\n';
}
