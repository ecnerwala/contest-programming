#include<iostream>
#include<iomanip>
#include<vector>
#include<complex>
#include<cassert>
#include<cstdlib>
#include<ctime>
#include<stack>

using namespace std;

typedef long long ll;
typedef complex<ll> pt;
typedef complex<double> pd;
#define X real()
#define Y imag()
typedef pair<pd, double> circ;

const double EPS = 1e-9;

const int MAXN = 200000;
const int MAXX = 1e6;

const circ BAD(pd(0, 0), -1);

int N, M; 
pt P[MAXN];

double D;
int S;
pd Q[MAXN];

const unsigned int RAND_A = 110293;
const unsigned int RAND_B = 19824987;
unsigned int seed = 0;
unsigned int getRandom() {
	return rand();
	return seed = seed * RAND_A + RAND_B;
}

template<class T> void shuffle(T *a, const int &n) {
	for(int i = 0; i < n; i++) {
		int x = getRandom() % n;
		int y = getRandom() % n;
		T p = a[x];
		a[x] = a[y];
		a[y] = p;
	}
}

inline pd toDbl(const pt &p) {
	//return p;
	return pd(p.X, p.Y);
}

bool in(const pt &p, const circ &c) {
	return norm(c.first - toDbl(p)) <= c.second;
}

circ circumcircle(const pt &p) {
	return circ(toDbl(p), 0);
}

circ circumcircle(const pt &a, const pt &b) {
	pd p(a.X + b.X, a.Y + b.Y);
	p /= 2.;
	return circ(p, norm(a - b) / 4.);
}

circ circumcircle(const pt &x, const pt &y, const pt &z) {
	pt c = norm(x) * (z - y) + norm(y) * (x - z) + norm(z) * (y - x);
	//pt div = (x * conj(y) - y * conj(x)) + (y * conj(z) - z * conj(y)) + (z * conj(x) - x * conj(z));
	//ll div = (conj(x) * (z - y)).imag() + (conj(y) * (x - z)).imag() + (conj(z) * (y - x)).imag(); // div is pure imaginary...
	//ll div = 2 * (conj(z - x) * (y - x)).imag(); // it's the area * 4
	ll div = 2 * ((z.X - x.X) * (y.Y - x.Y) - (z.Y - x.Y) * (y.X - x.X)); // woah, fewer multiplications
	//cerr << x << ' ' << y << ' ' << z << '\n';
	//assert(div != 0.);
	pd p (c.Y, -c.X);
	p /= div;
	double r = norm(toDbl(x) - p);
	//cerr << c << ' ' << r << '\n';
	//assert(abs(abs(y - c) - r) <= EPS && abs(abs(z - c) - r) <= EPS);
	return circ(p, r);
}

pt K[MAXN];

int rt;
circ getSed(int a, int len) {
	rt += len;
	for(int i = 0; i < len; i++) {
		K[i] = P[a + i];
	}
	shuffle(K, len);

	circ c = BAD;

	for(int i = 0; i < len; i++) {
		if(!in(K[i], c)) {
			c = circumcircle(K[i]);
			for(int j = 0; j < i; j++) {
				if(!in(K[j], c)) {
					c = circumcircle(K[i], K[j]);
					for(int k = 0; k < j; k++) {
						if(!in(K[k], c)) {
							c = circumcircle(K[i], K[j], K[k]);
						}
					}
				}
			}
		}
	}

	return c;
}

bool check(double d) {
	D = d * d;

	int cur = 0;
	for(S = 0; S < M; S++) {
		if(cur == N) break;

		int min = 0, max = 1;

		while(getSed(cur, max).second <= D) {
			min = max;
			max *= 2;
			if(cur + max > N) {
				max = N - cur + 1;
				break;
			}
		}

		while(max - min > 1) {
			int mid = (min + max) / 2;
			if(getSed(cur, mid).second <= D) {
				min = mid;
			} else {
				max = mid;
			}
		}

		Q[S] = getSed(cur, min).first;
		cur += min;
	}

	//cerr << rt << '\n';

	return cur == N;
}

void go() {
	if(check(0.0)) return;
	double min = 0, max = MAXX * 4;
	while(max - min > EPS) {
		double mid = (min + max) / 2;
		if(check(mid)) max = mid;
		else min = mid;
	}
	check(max);
	D = max;
}

void gen(int n) {
	N = n;
	M = rand() % N + 1;

	for(int i = 0; i < N; i++) {
		P[i].real() = rand() % (MAXX * 2) - MAXX;
		P[i].imag() = rand() % (MAXX * 2) - MAXX;
	}
}

void test() {
	while(true) {
		gen(1e3);
		cerr << "Start" << '\n';
		rt = 0;
		go();
		cerr << rt << '\n';
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	cout << fixed << setprecision(15);

	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> P[i].real() >> P[i].imag();
	}

	go();

	cout << D << '\n';
	cout << S << '\n';
	for(int i = 0; i < S; i++) {
		cout << Q[i].real() << ' ' << Q[i].imag() << '\n';
	}

	//test();

	return 0;
}
