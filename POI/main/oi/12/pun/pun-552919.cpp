#include<iostream>
#include<cmath>
#include<cstring>
#include<complex>
#include<algorithm>
#include<limits>

using namespace std;

template <class T> void kmp_failure(T *W, int *F, int N) {
	F[0] = -1;
	F[1] = 0;
	int cnd = 0, pos = 2;
	while(pos < N) {
		if(cnd == -1 || W[pos - 1] == W[cnd]) {
			F[pos] = cnd + 1;
			pos++;
			cnd++;
		} else {
			cnd = F[cnd];
		}
	}
}

// return start point of first occurence, -1 if does not exist
template <class T> int kmp_search(T *W, int *F, int N, T *S, int M) {
	int cnd = 0, pos = 0;
	while(pos < M) {
		if(cnd == -1 || S[pos] == W[cnd]) {
			pos++, cnd++;
			if(cnd == N) {
				return pos - cnd;
			}
		} else {
			cnd = F[cnd];
		}
	}
	return -1;
}

typedef pair<double, double> pdd;

const double EPS = 1e-9;

struct number {
	double d;

	number(double v) : d(v) {}
	number() : d(0) {}

	number& operator = (double v) {
		d = v;
		return *this;
	}
};

inline bool operator == (number a, number b) {
	return abs(a.d - b.d) < EPS;
}

const int MAXK = 5e4;

int K, L;
bool C; //whether there is the centroid
int P; // K - C
double X[MAXK], Y[MAXK];
number vals[MAXK * 2]; // stores angle & length
int failure[MAXK * 2]; // failure table

number target[MAXK * 2 * 2];

inline double sq(double d) { return d * d; }

const double PI = atan2(0, -1);
const double TAU = 2 * PI;

void translate() {
	double cx = 0, cy = 0;
	for(int i = 0; i < K; i++) cx += X[i], cy += Y[i];
	cx /= K, cy /= K;
	for(int i = 0; i < K; i++) {
		X[i] -= cx;
		Y[i] -= cy;
	}
}

void dilate() {
	double d = 0;
	for(int i = 0; i < K; i++) d += sqrt(sq(X[i]) + sq(Y[i]));
	d /= K;
	for(int i = 0; i < K; i++) {
		X[i] /= d;
		Y[i] /= d;
	}
}

bool checkCentroid() {
	for(int i = 0; i < K; i++) {
		if(X[i] == 0 && Y[i] == 0) {
			for(int j = i; j + 1 < K; j++) {
				X[j] = X[j + 1];
				Y[j] = Y[j + 1];
			}
			return true;
		}
	}
	return false;
}

pdd pts[MAXK];
void toVals(number *v) {
	for(int i = 0; i < P; i++) {
		pts[i] = pdd(atan2(Y[i], X[i]), sq(X[i]) + sq(Y[i]));
	}
	sort(pts, pts + P);
	for(int i = 0; i < P; i++) {
		v[2 * i] = -pts[i].second;
		if(i + 1 < P) {
			v[2 * i + 1] = pts[i + 1].first - pts[i].first;
		} else {
			v[2 * i + 1] = pts[0].first - pts[i].first + TAU;
		}
	}
}

void precomp() {
	if(K == 1) {
		return;
	}
	translate();
	dilate();
	C = checkCentroid();
	P = K - C;
	toVals(vals);

	kmp_failure(vals, failure, 2 * P);

	//cerr << C << '\n';
	//for(int i = 0; i < P; i++) cerr << X[i] << ' ' << Y[i] << '\n';
	//for(int i = 0; i < 2 * P; i++) cerr << vals[i] << ' '; cerr << '\n';
}

bool solve() {
	if(K != L) return false;
	if(K == 1) {
		return true;
	}
	translate();
	dilate();
	if(checkCentroid() != C) return false;
	toVals(target);

	for(int i = 0; i < 2 * P; i++) {
		target[2 * P + i] = target[i];
	}

	//for(int i = 0; i < P; i++) cerr << X[i] << ' ' << Y[i] << '\n';
	//for(int i = 0; i < 2 * P * 2; i++) cerr << target[i] << ' '; cerr << '\n';

	if(kmp_search(vals, failure, 2 * P, target, 2 * P * 2) != -1) {
		return true;
	}

	reverse(target, target + 2 * 2 * P);

	if(kmp_search(vals, failure, 2 * P, target, 2 * P * 2) != -1) {
		return true;
	}

	return false;
}

int main() {
	ios_base::sync_with_stdio(0);
	
	cin >> K;
	for(int i = 0; i < K; i++) cin >> X[i] >> Y[i];

	precomp();

	int T; cin >> T;
	for(; T > 0; T--) {
		cin >> L;
		for(int i = 0; i < L; i++) cin >> X[i] >> Y[i];

		cout << (solve() ? "TAK" : "NIE") << '\n';
	}
	return 0;
}
