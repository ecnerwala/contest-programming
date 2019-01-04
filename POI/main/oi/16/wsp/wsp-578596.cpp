#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

const double EPS = 1e-9;

typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef complex<double> pt;
#define A first
#define B second
#define X real()
#define Y imag()

const int MAXN = 1e5 + 20;
const int MAXM = 1e6 + 20;

int N, M;
pt V[MAXN];
pii bad[MAXM];

pii chords[MAXN];
pt vert[MAXN]; // intersections along the path

double cross(pt a, pt b) {
	return imag(conj(a) * b);
}

// twice the area 
double area(pt a, pt b, pt c) {
	return cross(b - a, c - a);
}

// strictly counterclockwise
double ccw(pt a, pt b, pt c) {
	return area(a, b, c) >= EPS;
}

// strictly clockwise
double cw(pt a, pt b, pt c) {
	return area(a, b, c) <= -EPS;
}

// collinear
double col(pt a, pt b, pt c) {
	return abs(area(a, b, c)) < EPS;
}

double dot(pt a, pt b) {
	return real(conj(a) * b);
}

pt inter(pt p1, pt p2, pt q1, pt q2) {
	double a1 = p2.Y - p1.Y;
	double b1 = p1.X - p2.X;
	double c1 = a1 * p1.X + b1 * p1.Y;
	//assert(c1 == a1 * p2.X + b1 * p2.Y);
	//cerr << p1 << ' ' << p2 << " -> " << a1 << "x + " << b1 << "y = " << c1 << '\n';
	double a2 = q2.Y - q1.Y;
	double b2 = q1.X - q2.X;
	double c2 = a2 * q1.X + b2 * q1.Y;
	//assert(c2 == a2 * q2.X + b2 * q2.Y);
	//cerr << q1 << ' ' << q2 << " -> " << a2 << "x + " << b2 << "y = " << c2 << '\n';
	double det = a1 * b2 - a2 * b1;
	double x = (c1 * b2 - c2 * b1) / det;
	double y = (a1 * c2 - a2 * c1) / det;
	//cerr << det << ' ' << x << ' ' << y << '\n';
	//assert(c1 == a1 * x + b1 * y);
	//assert(c2 == a2 * x + b2 * y);
	return pt(x, y);
}

int main() {
	//assert(false);
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 1; i <= N; i++) {
		cin >> V[i].X >> V[i].Y;
	}
	for(int i = 0; i < M; i++) {
		cin >> bad[i].A >> bad[i].B;
		bad[i].B *= -1;
	}
	sort(bad, bad + M);
	int far = 1;
	int L = 0; // number of chords
	for(int st = 1, badInd = 0; st < N; st++) {
		int en = N;
		while(badInd < M && bad[badInd].A == st) {
			if(en == -bad[badInd].B) en--;
			badInd++;
		}
		if(en <= far) continue;
		far = en;
		//cerr << st << ' ' << en << '\n';
		//cerr << V[st] << ' ' << V[en] << '\n';
		if(L == 0) {
			vert[L] = V[st];
		} else {
			// we now have chord from st to en
			while(!cw(V[st], V[en], vert[L - 1])) {
				L--;
			}
			pii prv = chords[L - 1];
			vert[L] = inter(V[prv.A], V[prv.B], V[st], V[en]);
		}
		chords[L] = pii(st, en);
		for(int i = 0; i <= L; i++) {
			//cerr << vert[i] << '\n';
			//cerr << chords[i].A << V[chords[i].A] << ' ' << chords[i].B << V[chords[i].B] << '\n';
		}
		L++;
	}
	vert[L] = V[N];
	double res = 0;
	for(int i = 0; i < L; i++) {
		res += abs(vert[i + 1] - vert[i]);
	}
	cout << fixed << setprecision(10) << res << '\n';
	return 0;
}

