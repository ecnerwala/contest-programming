#define NDEBUG
#include<bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef double num;

num INF = 1e18;
num EPS = 1e-9;

typedef complex<num> pt;
#define X real()
#define Y imag()

typedef pair<pt, num> circ;

const int MAXN = 2e5;
int N;
pt P[MAXN];

const int MAXM = MAXN;
int M;
int S;
pt Q[MAXM];

inline int get_rand() {
	return rand();
}

template<class T> void shuffle(T *a, const int &n) {
	for(int i = 1; i < n; i++) {
		int r = get_rand() % (i+1);
		swap(a[i], a[r]);
	}
}

inline bool in_circ(circ a, pt p) { return norm(a.first - p) - a.second <= EPS; }

inline circ circumcircle(pt p) {
	return circ(p, 0);
}

inline circ circumcircle(pt p, pt q) {
	return circ((p + q) / 2., norm((p - q) / 2.));
}

inline circ circumcircle(pt a, pt b, pt c) {
	//cerr << "circ " << a << ' ' << b << ' ' << c << '\n';
	num na = norm(a), nb = norm(b), nc = norm(c);
	pt center = a * (nb - nc) + b * (nc - na) + c * (na - nb);
	num div = (conj(c - a) * (b - a)).imag() * 2.;
	center /= pt(0, div);
	num r = norm(a - center);
	//cerr << center << ' ' << r << '\n';
	assert(abs(r - norm(b - center)) <= EPS);
	assert(abs(r - norm(c - center)) <= EPS);
	return circ(center, r);
}

pt V[MAXN];

circ seb(int s, int l) {
	//cerr << "seb " << s << ' ' << l << '\n';
	for(int i = 0; i < l; i++) V[i] = P[s+i];
	shuffle(V, l);

	// for(int i = 0; i < l; i++) { cerr << V[i] << ' '; } cerr << '\n';

	circ res(-INF, 0);
	for(int i = 0; i < l; i++) if(!in_circ(res, V[i])) {
		res = circumcircle(V[i]);
		for(int j = 0; j < i; j++) if(!in_circ(res, V[j])) {
			res = circumcircle(V[i], V[j]);
			for(int k = 0; k < j; k++) if(!in_circ(res, V[k])) {
				res = circumcircle(V[i], V[j], V[k]);
			}
		}
	}
	for(int i = 0; i < l; i++) assert(in_circ(res, V[i]));
	return res;
}

int process(num d) {
	num D = d * d;
	//cerr << D << '\n';
	S = 0;
	for(int i = 0; i < N; ) {
		int mi = 1, ma = 2;
		while(true) {
			circ c = seb(i, ma);
			if(c.second <= D) {
				mi = ma;
				ma *= 2;
				if(i + ma > N) {
					ma = N+1 - i;
					break;
				}
			} else break;
		}
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			circ c = seb(i, md);
			if(c.second <= D) {
				mi = md;
			} else {
				ma = md;
			}
		}
		circ c = seb(i, mi);
		assert(c.second <= D);
		Q[S++] = c.first;
		for(int v = 0; v < mi; v++) assert(in_circ(c, P[i+v]));
		i += mi;
	}
	return S;
}

num go() {
	num mi = 0, ma = 1e15;
	for(int z = 0; z < 200; z++) {
		num md = (mi + ma) / 2;
		if(process(md) <= M) {
			ma = md;
		} else {
			mi = md;
		}
	}
	
	return ma + EPS;
}

int main() {
	scanf("%d %d", &N, &M);
	for(int i = 0; i < N; i++) {
		int x, y; scanf("%d %d", &x, &y);
		P[i] = pt(x, y);
	}
	
	num res = go();
	printf("%.9f\n", res);

	process(res);

	printf("%d\n", S);

	for(int i = 0; i < S; i++) {
		printf("%.9f %.9f\n", Q[i].X, Q[i].Y);
	}

	return 0;
}
