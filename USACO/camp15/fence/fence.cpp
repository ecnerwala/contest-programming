#include<bits/stdc++.h>
using namespace std;

inline void wssert(bool b) { if(!b) exit(0); }

typedef long double ld;

const ld INF = 2e9;

typedef pair<ld, ld> pdd;
typedef pair<pdd, pdd> seg;
#define A first
#define B second

const ld EPS = 1e-5;

template<typename T1, typename T2> ostream& operator << (ostream &out, pair<T1, T2> v) {
	return out << '(' << v.A << ',' << ' ' << v.B << ')';
}

bool eq(ld a, ld b) {
	return abs(a - b) < EPS;
}

bool operator < (pdd l, pdd r) {
	return (r.A - l.A > EPS) || (eq(r.A, l.A) && r.B - l.B > EPS);
}

bool operator > (pdd l, pdd r) {
	return r < l;
}

inline ld cross(pdd a, pdd b) { return a.A * b.B - a.B * b.A; }
inline ld dot(pdd a, pdd b) { return a.A * b.A + a.B * b.B; }

// ccw area
ld area(pdd a, pdd b, pdd c) {
	return cross(a, b) + cross(b, c) + cross(c, a);
}

pair<pdd, ld> toline(seg s) {
	pdd vec(s.A.B - s.B.B, s.B.A - s.A.A);
	wssert(eq(dot(vec, s.A), dot(vec, s.B)));
	wssert(!(eq(vec.A, 0) && eq(vec.B, 0)));
	return pair<pdd, ld>(vec, dot(vec,s.A));
}

pdd inter(pair<pdd, ld> a, pair<pdd, ld> b) {
	ld c = cross(a.A, b.A);

	if(eq(c, 0)) return pdd(-INF, -INF);

	a.B /= c;
	b.B /= c;

	pdd res(a.B * b.A.B - b.B * a.A.B, b.B * a.A.A - a.B * b.A.A);

	//assert(eq(res.A * a.A.A + res.B * a.A.B, a.B * c));
	//assert(eq(res.A * b.A.A + res.B * b.A.B, b.B * c));

	return res;
}
// assume they do intersect
pdd inter(seg a, seg b) {
	//cerr << "inter " << a << ' ' << b << '\n';
	return inter(toline(a), toline(b));
}

const int MAXV = 2e5;
int V[2];
pdd P[2][MAXV];

// 0 has smalder y, 1 has bigger y
vector<pdd> hull[2][2];

int main() {
	cin >> V[0] >> V[1];
	for(int t = 0; t < 2; t++) {
		for(int i = 0; i < V[t]; i++) {
			cin >> P[t][i].A >> P[t][i].B;
		}
		int mi = 0;
		int ma = 0;
		for(int i = 1; i < V[t]; i++) {
			wssert(P[t][i] != P[t][mi]);
			if(P[t][i] < P[t][mi]) mi = i;

			wssert(P[t][i] != P[t][ma]);
			if(P[t][i] > P[t][ma]) ma = i;
		}

		//cerr << mi << ' ' << ma << '\n';

		for(int i = mi; i != ma; i++, i %= V[t]) {
			hull[t][1].push_back(P[t][i]);
		}
		hull[t][1].push_back(P[t][ma]);

		for(int i = mi; i != ma; ) {
			hull[t][0].push_back(P[t][i]);

			i--;
			if(i < 0) i += V[t];
		}
		hull[t][0].push_back(P[t][ma]);

		//cerr << hull[t][0].size() << ' ' << hull[t][1].size() << ' ' << V[t] << '\n';

		wssert(int(hull[t][0].size() + hull[t][1].size()) == V[t] + 2);
	}

	ld res = 0;

	for(int t = 0; t < 2; t++) {
		for(int i = 0; i < V[t]; i++) {
			res += cross(P[t][(i + 1) % V[t]], P[t][i]);
		}
	}

	res /= 2;

	int c[2][2] = {{-1,-1},{-1,-1}};
	pdd cur(-INF, -INF);
	while(true) {
		bool done = false;
		for(int t = 0; t < 2; t++) {
			for(int d = 0; d < 2; d++) {
				wssert(c[t][d] < int(hull[t][d].size()));
				while(c[t][d] + 1 < int(hull[t][d].size()) && !(hull[t][d][c[t][d] + 1] > cur)) {
					c[t][d] ++;
				}
				wssert(c[t][d] + 1 == int(hull[t][d].size()) || (hull[t][d][c[t][d] + 1] > cur));
			}
		}

		for(int t = 0; t < 2; t++) {
			for(int d = 0; d < 2; d++) {
				wssert(c[t][d] < int(hull[t][d].size()));
			}
			wssert((c[t][0] == int(hull[t][0].size()) - 1) == (c[t][1] == int(hull[t][1].size()) - 1));
			if(c[t][0] == int(hull[t][0].size()) - 1) {
				wssert(c[t][1] == int(hull[t][1].size()) - 1);
				done = true;
				break;
			}
		}
		if(done) break;

		pdd nxt(INF, INF);

		for(int t = 0; t < 2; t++) {
			for(int d = 0; d < 2; d++) {
				wssert(c[t][d] + 1 >= 0);
				wssert(c[t][d] + 1 < int(hull[t][d].size()));

				pdd val = hull[t][d][c[t][d] + 1];
				wssert(val > cur);
				nxt = min(nxt, val);
			}
		}

		for(int d0= 0; d0 < 2; d0++) {
			for(int d1= 0; d1 < 2; d1++) {
				if(c[0][d0] >= 0 && c[1][d1] >= 0) {
					wssert(c[0][d0] + 1 < int(hull[0][d0].size()));
					wssert(c[1][d1] + 1 < int(hull[1][d1].size()));

					pdd val = inter(
							seg(hull[0][d0][c[0][d0]], hull[0][d0][c[0][d0] + 1]),
							seg(hull[1][d1][c[1][d1]], hull[1][d1][c[1][d1] + 1])
							);

					if(val > cur) {
						nxt = min(nxt, val);
					}
				}
			}
		}

		wssert(cur < nxt);
		//cerr << cur << ' ' << nxt << '\n';

		if(nxt.A - cur.A > EPS) {
			for(int i = 0; i < 2; i++) {
				ld x = (i ? nxt.A : cur.A);
				ld mi = -INF;
				ld ma = INF;
				for(int t = 0; t < 2; t++) {
					for(int d = 0; d < 2; d++) {
						if(c[t][d] == -1) {
							ma = -INF;
							mi = INF;
							//cerr << "hi\n";
							continue;
						}
						seg s(hull[t][d][c[t][d]], hull[t][d][c[t][d] + 1]);
						wssert(s.B.A - s.A.A > EPS);
						pdd intersection = inter(seg(pdd(x, 0), pdd(x, 1)), s);
						wssert(eq(intersection.A, x));
						ld val = intersection.B;
						//cerr << x << ' ' << s << ' ' << val << '\n';
						if(d == 0) {
							mi = max(mi, val);
						} else {
							ma = min(ma, val);
						}
					}
				}
				ld len = (ma - mi) / 2;
				len = max(len, ld(0));

				//cerr << mi << ' ' << ma << ' ' << len << '\n';
				res -= len * (nxt.A - cur.A);
			}
		}

		cur = nxt;
	}

	cout << fixed << setprecision(10) << res << '\n';
}
