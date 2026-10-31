#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ld = long double;

const ld EPS = 1e-10;
bool eq(ld a, ld b) {
	return abs(a-b) < EPS;
}

struct pt {
	ld x, y, z;
	friend pt operator + (const pt& a, const pt& b) {
		return pt{a.x + b.x, a.y + b.y, a.z + b.z};
	}
	friend pt operator - (const pt& a, const pt& b) {
		return pt{a.x - b.x, a.y - b.y, a.z - b.z};
	}
	pt& operator -= (const pt& o) {
		x -= o.x, y -= o.y, z -= o.z;
		return *this;
	}
	pt& operator += (const pt& o) {
		x += o.x, y += o.y, z += o.z;
		return *this;
	}
	pt& operator *= (ld a) {
		x *= a, y *= a, z *= a;
		return *this;
	}
	pt& operator /= (ld a) {
		x /= a, y /= a, z /= a;
		return *this;
	}
	friend pt operator * (const pt& a, ld b) { return pt(a) *= b; }
	friend pt operator / (const pt& a, ld b) { return pt(a) /= b; }
	ld norm() const {
		return x * x + y * y + z * z;
	}
	ld abs() const {
		return sqrt(norm());
	}
};
ld dot(const pt& a, const pt& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
pt cross(const pt& a, const pt& b) {
	return pt{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
ld area(const pt& a, const pt& b, const pt& c) {
	return cross(b-a, c-a).abs();
}

bool inside(const pt& a, const pt& b, const pt& c, const pt& d) {
	cerr << area(a,b,d) + area(a,d,c) + area(d,b,c) - area(a,b,c) << '\n';
	return area(a,b,d) + area(a,d,c) + area(d,b,c) - area(a,b,c) < 1e-7;
}

class SpaceProbes {

	ld ptLine(pt o, pt d) {
		if (0 <= dot(o, d) && dot(o, d) <= d.norm()) {
			// just project
			return double(cross(o, d).abs() / d.abs());
		} else {
			return double(min(o.abs(), (o-d).abs()));
		}
	}
public:

	double focusDistance(vector<long long> x, vector<long long> y, vector<long long> z) {
		assert(x.size() == 5);
		assert(y.size() == 5);
		assert(z.size() == 5);
		vector<pt> pts(5);
		for (size_t i = 0; i < 5; i++) {
			pts[i] = pt{ld(x[i]), ld(y[i]), ld(z[i])};
		}

		pt as = pts[0];
		pt at = pts[1];
		pt bs = pts[2];
		pt bt = pts[3];

		pt o = pts[4] - (as + bs) / 2;
		pt dx = (at - as) / 2;
		pt dy = (bt - bs) / 2;

		if (cross(dx, dy).abs() < EPS) {
			// line case
			return double(min(ptLine(o, dx + dy), ptLine(o - dy, dx)));
		} else {
			pt perp = cross(dx, dy);
			perp /= perp.abs();

			ld oz = dot(o, perp);

			pt oxy = o - perp * oz;
			if (inside(pt{0,0,0}, dx, dy, oxy) || inside(dx+dy, dx, dy, oxy)) {
				return double(oz);
			} else {
				return double(min(min(ptLine(o, dx), ptLine(o, dy)), min(ptLine(o - dx, dy), ptLine(o - dy, dx))));
			}
		}
	}
};

}  // namespace ecnerwala

using ecnerwala::SpaceProbes;

// vim:ft=cpp
