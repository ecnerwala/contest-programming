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
	// const int MAXZ = 210;
	int shift = MAXZ * 0.5;
	for(int i = 0; i < v; i++){
		cin >> vert[i].x >> vert[i].y >> z[i];
		z[i] += shift;
	}
	//vector<ld> delA(MAXZ, 0), delB(MAXZ, 0), delC(MAXZ, 0);
	vector<ld> face_area(MAXZ, 0);

	auto interp = [](ld x0, ld x1, ld x2, ld v0, ld v1, ld v2, ld& qA, ld& qB, ld& qC){
		ld f0 = v0 / (x0 - x1) / (x0 - x2);
		ld f1 = v1 / (x1 - x0) / (x1 - x2);
		ld f2 = v2 / (x2 - x0) / (x2 - x1);
		qA = f0 + f1 + f2;
		qB = -f0 * (x1 + x2) - f1 * (x0 + x2) - f2 * (x0 + x1);
		qC = f0 * x1 * x2 + f1 * x0 * x2 + f2 * x0 * x1;
	};

	vector<array<ld, 3>> seg(MAXZ * 2);
	auto seg_add = [&](int l, int r, ld va, ld vb, ld vc) {
		for (int a = MAXZ+l, b = MAXZ + r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				seg[a][0] += va;
				seg[a][1] += vb;
				seg[a][2] += vc;
				a++;
			}
			if (b & 1) {
				--b;
				seg[b][0] += va;
				seg[b][1] += vb;
				seg[b][2] += vc;
			}
		}
	};
	auto seg_push = [&]() {
		for (int a = 1; a < MAXZ; a++) {
			for (int c : {2 * a, 2 * a + 1}) {
				seg[c][0] += seg[a][0];
				seg[c][1] += seg[a][1];
				seg[c][2] += seg[a][2];
			}
		}
	};

	for(int _ = 0; _ < t; _++){
		vector<int> idx(3);
		cin >> idx[0] >> idx[1] >> idx[2];
		vector<int> wh(3);
		for(int i = 0; i < 3; i++) wh[i] = i;
		sort(wh.begin(), wh.end(), [&](int a, int b){
			return z[idx[a]] < z[idx[b]];
		});
		int sign = (wh[1] - wh[0] + 3) % 3;
		if(sign == 2){
			sign = -1;
		}
		sign *= -1;
		vector<int> zi(3);
		vector<P> vi(3);
		for(int i = 0; i < 3; i++){
			zi[i] = z[idx[wh[i]]];
			vi[i] = vert[idx[wh[i]]];
		}
		assert(zi[0] <= zi[1] && zi[1] <= zi[2]);
		if(zi[0] == zi[1] && zi[1] == zi[2]){
			// nothing
			face_area[zi[0]] += abs(vi[0].cross(vi[1], vi[2]));
		} else {
			ld ratio = ld(zi[1] - zi[0]) / ld(zi[2] - zi[0]);
			P vm = vi[0] + (vi[2] - vi[0]) * ratio;
			if (z[0] != z[1]){
				ld x0 = zi[1];
				ld x1 = (zi[1] + zi[0]) * 0.5;
				ld x2 = zi[0];
				ld f0 = vi[1].cross(vm);
				ld f1 = ((vi[1] + vi[0]) * 0.5).cross((vm + vi[0]) * 0.5);
				ld f2 = 0;
				ld qA, qB, qC;
				interp(x0, x1, x2, f0, f1, f2, qA, qB, qC);
				qA *= sign; qB *= sign; qC *= sign;
				seg_add(zi[0], zi[1], qA, qB, qC);
				/*
				delA[zi[0]] += qA; delB[zi[0]] += qB; delC[zi[0]] += qC;
				delA[zi[1]] -= qA; delB[zi[1]] -= qB; delC[zi[1]] -= qC;
				*/
			}
			if (z[1] != z[2]){
				ld x0 = zi[1];
				ld x1 = (zi[1] + zi[2]) * 0.5;
				ld x2 = zi[2];
				ld f0 = vi[1].cross(vm);
				ld f1 = ((vi[1] + vi[2]) * 0.5).cross((vm + vi[2]) * 0.5);
				ld f2 = 0;
				ld qA, qB, qC;
				interp(x0, x1, x2, f0, f1, f2, qA, qB, qC);
				qA *= sign; qB *= sign; qC *= sign;
				seg_add(zi[1], zi[2], qA, qB, qC);
				/*
				delA[zi[1]] += qA; delB[zi[1]] += qB; delC[zi[1]] += qC;
				delA[zi[2]] -= qA; delB[zi[2]] -= qB; delC[zi[2]] -= qC;
				*/
			}
		}
	}
	seg_push();

	ld curA = 0, curB = 0, curC = 0;
	ld best = -1, best_loc = -1;
	for(int i = 1; i < MAXZ; i++){
		ld curz = i;
		curA = seg[MAXZ+i-1][0];
		curB = seg[MAXZ+i-1][1];
		curC = seg[MAXZ+i-1][2];
		ld prev_val = curC + curz * (curB + curz * curA);
		curA = seg[MAXZ+i][0];
		curB = seg[MAXZ+i][1];
		curC = seg[MAXZ+i][2];
		ld next_val = curC + curz * (curB + curz * curA);
		ld at_z = (prev_val + next_val + face_area[i]) * 0.5;
		if(at_z > best){
			best = at_z;
			best_loc = curz;
		}

		ld z = -curB / curA / 2;
		if(z > curz && z < curz+1){
			ld val = curC + z * (curB + z * curA);
			if(val > best){
				best = val;
				best_loc = z;
			}
		}
	}
	best_loc -= shift;
	cout << fixed << setprecision(15);
	cout << best_loc << ' ' << best/2 << '\n';
}
