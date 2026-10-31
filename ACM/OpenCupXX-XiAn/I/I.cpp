#include<bits/stdc++.h>
using namespace std;

const double PI = acos(-1.);
const double TAU = 2 * PI;

template <typename T> int signum(T a) { return (a > 0) - (a < 0); }
template <typename T> T sq(T a) { return a * a; }

template <typename T> T gcd(T a, T b) {
	while (a) {
		b %= a;
		swap(a, b);
	}
	return b;
}

template <typename T> struct Point3D {
	using P = Point3D;

	T x, y, z;
	Point3D() : x(0), y(0), z(0) {}
	Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

	template <typename U> explicit Point3D(const Point3D<U>& p) : x(T(p.x)), y(T(p.y)), z(T(p.z)) {}

	friend istream& operator >> (istream& i, P& p) { return i >> p.x >> p.y >> p.z; }
	friend ostream& operator << (ostream& o, const P& p) { return o << "(" << p.x << "," << p.y << "," << p.z << ")"; }

	friend bool operator == (const P& a, const P& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
	friend bool operator != (const P& a, const P& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

	P& operator += (const P& o) { x += o.x, y += o.y, z += o.z; return *this; }
	P& operator -= (const P& o) { x -= o.x, y -= o.y, z -= o.z; return *this; }
	friend P operator + (const P& a, const P& b) { return P(a) += b; }
	friend P operator - (const P& a, const P& b) { return P(a) -= b; }

	P& operator *= (const T& t) { x *= t, y *= t, z *= t; return *this; }
	P& operator /= (const T& t) { x /= t, y /= t, z /= t; return *this; }
	friend P operator * (const P& p, const T& t) { return P(p) *= t; }
	friend P operator * (const T& t, const P& p) { return P(p) *= t; }
	friend P operator / (const P& a, const T& t) { return P(a) /= t; }

	friend P operator + (const P& a) { return P(+a.x, +a.y, +a.z); }
	friend P operator - (const P& a) { return P(-a.x, -a.y, -a.z); }

	friend T dot(const P& a, const P& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	friend T norm(const P& a) { return dot(a,a); }
	friend P cross(const P& a, const P& b) { return P(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

	friend T int_norm(const P& p) {
		return gcd(gcd(abs(p.x), abs(p.y)), abs(p.z));
	}
	friend P int_unit(const P& p) {
		T g = int_norm(p);
		return g ? p / g : p;
	}

	friend T abs(const P& a) { return sqrt(max(T(0), norm(a))); }
	friend P unit(const P& a) { return a / abs(a); }

	friend T vol(const P& a, const P& b, const P& c, const P& d) { return dot(cross(b-a, c-a), d-a); }

	friend bool lexLess(const P& a, const P& b) { return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z); }

	friend bool parallelSame(const P& a, const P& b) {
		assert(a != P());
		assert(b != P());
		return lexLess(a, P()) == lexLess(b, P());
	}
};

using ll = long long;
using P = Point3D<ll>;

void my_sleep(double t) {
	auto st = clock();
	while ((clock() - st) < t * CLOCKS_PER_SEC) {
		// spin
	}
}

void wssert(bool b) {
#ifndef NDEBUG
	if (!b) {
		cout << "GARBAGE" << '\n';
		exit(0);
	}
#endif
}

double go() {
	int N; cin >> N;
	vector<P> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i];
		pts[i] = int_unit(pts[i]);
	}

	if (N <= 2) {
		cerr << "two pts" << '\n';
		return 1;
	}

	assert(N >= 3);
	P p0;
	bool hasAntipode = false;
	{
		struct cmp { bool operator () (const P& a, const P& b) { return lexLess(a,b); } };
		map<P, int, cmp> mp;
		for (int i = 0; i < N; i++) {
			auto it = mp.find(-pts[i]);
			if (it != mp.end()) {
				hasAntipode = true;

				int j = it->second;
				p0 = pts[i];

				swap(pts[i], pts[N-1]);
				pts.pop_back();
				N--;
				swap(pts[j], pts[N-1]);
				pts.pop_back();
				N--;
				break;
			}
			assert(mp.find(pts[i]) == mp.end());
			mp[pts[i]] = i;
		}
		if (!hasAntipode) {
			p0 = pts.back();
			pts.pop_back();
			N--;
		}
	}

	assert(N >= 1);
	assert(cross(p0, pts.front()) != P(0,0,0));

	{ // sort by angle
		P normal = cross(p0, pts.front());
		sort(pts.begin(), pts.end(), [&](const P& a, const P& b){
			int sa = signum(dot(a, normal));
			int sb = signum(dot(b, normal));
			if (sa == 0) {
				sa = parallelSame(cross(p0, a), normal) ? 1 : -1;
			}
			if (sb == 0) {
				sb = parallelSame(cross(p0, b), normal) ? 1 : -1;
			}
			if (sa != sb) return sa > sb;

			return dot(cross(p0, a), b) > 0;
		});
		assert(dot(pts.front(), normal) == 0 && parallelSame(cross(p0, pts.front()), normal));
	}

	{ // unique by angle
		vector<P> npts;
		npts.reserve(pts.size());
		for (P pt : pts) {
			if (!npts.empty() && dot(cross(p0, npts.back()), pt) == 0 && parallelSame(cross(p0, npts.back()), cross(p0, pt))) {
				assert(cross(npts.back(), pt) != P());
				// compare npts.back() and pt
				if (parallelSame(cross(npts.back(), pt), cross(p0, npts.back()))) {
					npts.back() = pt;
				}
			} else {
				npts.push_back(pt);
			}
		}
		pts = std::move(npts);
		N = int(pts.size());
	}

	assert(N >= 1);

	if (N == 1) {
		cerr << "unique angle" << '\n';
		return 1;
	}
	assert(N >= 2);

	if (N == 2 && dot(cross(p0, pts[0]), pts[1]) == 0) {
		cerr << "planar" << '\n';
		return 1;
	}

	bool wraps = false;
	for (int i = 0; i < N; i++) {
		P perp = cross(p0, pts[i]);
		ll vol = dot(perp, pts[(i+1)%N]);
		if (vol <= 0) {
			wraps = true;
			// pts[i] to pts[(i+1)%N] wraps around
			vector<P> npts;
			npts.insert(npts.end(), pts.begin() + (i+1)%N, pts.end());
			npts.insert(npts.end(), pts.begin(), pts.begin() + (i+1)%N);
			pts = std::move(npts);
			break;
		}
	}

	assert(N >= 2);

	for (int i = 0; i+1 < N; i++) {
		assert(dot(cross(p0, pts[i]), pts[i+1]) > 0);
	}
	assert((dot(cross(p0, pts[N-1]), pts[0]) > 0) == (!wraps));

	if (hasAntipode) {
		if (wraps) {
			if (dot(cross(p0, pts.front()), pts.back()) == 0) {
				return 0.5;
			}
			Point3D<double> a(cross(p0, pts.front()));
			Point3D<double> b(cross(p0, pts.back()));
			return 1 - acos(max(-1., min(1., dot(unit(a),unit(b))))) / TAU;
		} else {
			// there's no cycle-point
			return 0;
		}
	}

	assert(!hasAntipode);

	if (wraps) {
		if (dot(cross(p0, pts.front()), pts.back()) == 0) {
			assert(cross(pts.front(), pts.back()) != P());
			// If it's greater than 180
			if (parallelSame(cross(p0, pts.front()), cross(pts.front(), pts.back()))) {
				// Then it's just the one hemisphere
				return 0.5;
			}
		}
	}

	vector<P> hull;

	auto checkHalf = [&](const P& norm) -> double {
		assert(dot(norm, p0) >= 0);
		for (P pt : pts) {
			if (dot(norm, pt) < 0) return 0;
		}
		return 0.5;
	};

	for (P pt : pts) {
		while (hull.size() >= 2) {
			P a = hull[int(hull.size()) - 2];
			P b = hull[int(hull.size()) - 1];
			P c = pt;
			assert(dot(cross(p0, a), b) > 0);
			assert(dot(cross(p0, b), c) > 0);
			if (dot(cross(p0, a), c) < 0) {
				if (dot(cross(a,b), c) < 0) {
					// not in a hemisphere
					return 0;
				} else if (dot(cross(a,b), c) == 0) {
					return checkHalf(cross(a,b));
				}
				assert(dot(cross(a,b), c) > 0);
			} else if (dot(cross(p0, a), c) == 0) {
				assert(cross(a,c) != P());
				if (parallelSame(cross(p0, a), cross(a, c))) {
					assert(!wraps);
					return 0;
				}
				assert(dot(cross(a,b), c) > 0);
			}

			if (dot(cross(a,b), c) <= 0) {
				hull.pop_back();
				continue;
			}

			break;
		}
		hull.push_back(pt);
	}

	if (wraps) {
		if (dot(cross(hull.front(), hull.back()), p0) != 0) {
			hull.push_back(p0);
		}
	} else {
		int i = 0;
		while (int(hull.size()) - i >= 3) {
			{
				P a = hull[int(hull.size()) - 2];
				P b = hull[int(hull.size()) - 1];
				P c = hull[i];
				assert(dot(cross(p0, a), b) > 0);
				assert(dot(cross(p0, b), c) > 0);
				if (dot(cross(p0, a), c) < 0) {
					if (dot(cross(a,b), c) < 0) {
						return 0;
					} else if (dot(cross(a,b), c) == 0) {
						return checkHalf(cross(a,b));
					}
					assert(dot(cross(a,b), c) > 0);
				} else if (dot(cross(p0, a), c) == 0) {
					assert(cross(a,c) != P());
					if (parallelSame(cross(p0, a), cross(a, c))) {
						return 0;
					}
					assert(dot(cross(a,b), c) > 0);
				}
				if (dot(cross(a,b), c) <= 0) {
					hull.pop_back();
					continue;
				}
			}
			{
				P a = hull[int(hull.size()) - 1];
				P b = hull[i];
				P c = hull[i+1];
				assert(dot(cross(p0, a), b) > 0);
				assert(dot(cross(p0, b), c) > 0);
				if (dot(cross(p0, a), c) < 0) {
					if (dot(cross(a,b), c) < 0) {
						return 0;
					} else if (dot(cross(a,b), c) == 0) {
						return checkHalf(cross(a,b));
					}
					assert(dot(cross(a,b), c) > 0);
				} else if (dot(cross(p0, a), c) == 0) {
					assert(cross(a,c) != P());
					if (parallelSame(cross(p0, a), cross(a, c))) {
						return 0;
					}
					assert(dot(cross(a,b), c) > 0);
				}
				if (dot(cross(a,b), c) <= 0) {
					i++;
					continue;
				}
			}
			break;
		}
		hull.erase(hull.begin(), hull.begin() + i);
	}
	assert(hull.size() >= 3);
	double ans = 1;
	for (int i = 0; i < int(hull.size()); i++) {
		P a = hull[i];
		P b = hull[(i+1)%int(hull.size())];
		P c = hull[(i+2)%int(hull.size())];
		assert(dot(cross(p0, a), b) >= 0);
		assert(dot(cross(a,b), c) > 0);
	}
	while (hull.size() >= 3) {
		P c = hull.back(); hull.pop_back();
		P b = hull.back();
		P a = hull.front();
		assert(dot(cross(a,b), c) > 0);
		Point3D<double> x(cross(a,b));
		x = unit(x);
		Point3D<double> y(cross(b,c));
		y = unit(y);
		Point3D<double> z(cross(c,a));
		z = unit(z);
		auto getAngle = [](const Point3D<double>& ua, const Point3D<double>& ub) {
			return acos(max(-1., min(1., dot(ua,ub))));
		};
		double area = (getAngle(x,-y)+getAngle(y,-z)+getAngle(z,-x) - PI) / (4 * PI);
		ans -= area;
		wssert(area-0 >= -1e-7 && 0.5-area >= -1e-7);
	}

	assert(ans - 0.5 >= -1e-7);

	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	double ans = go();
	cout << fixed << setprecision(12) << ans << '\n';

	return 0;
}
