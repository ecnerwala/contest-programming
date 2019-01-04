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

const double EPS = 1e-9;

struct number {
	double d;

	number(double v) : d(v) {}

	number& operator = (double v) {
		d = v;
		return *this;
	}
};

inline bool operator == (number a, number b) {
	return abs(a.d - b.d) < EPS;
}

const int MAXN = 2e5;

int N;
double X[MAXN], Y[MAXN];
double vals[MAXN * 2]; // stores angle & length
int failure[MAXN * 2]; // failure table

double target[MAXN * 2 * 2];

inline double sq(double d) { return d * d; }

const double PI = atan2(0, -1);

int solve() {
	X[N] = X[0];
	Y[N] = Y[0];
	X[N + 1] = X[1];
	Y[N + 1] = Y[1];
	for(int i = N; i >= 0; i--) {
		X[i + 1] -= X[i];
		Y[i + 1] -= Y[i];
	}
	for(int i = 0; i < N; i++) {
		vals[2 * i] = sq(X[i + 1]) + sq(Y[i + 1]);
		vals[2 * i + 1] = 
			-(
				atan2(
				X[i + 1] * Y[i + 2] - Y[i + 1] * X[i + 2], 
				X[i + 1] * X[i + 2] + Y[i + 1] * Y[i + 2])
				+ PI
			);
	}
	vals[2 * N] = numeric_limits<double>::quiet_NaN();

	kmp_failure(vals, failure, 2 * N + 1);

	//for(int i = 0; i < N; i++) cerr << X[i + 1] << ' '; cerr << '\n';
	//for(int i = 0; i < N; i++) cerr << Y[i + 1] << ' '; cerr << '\n';
	//for(int i = 0; i <= 2 * N; i++) cerr << vals[i] << ' '; cerr << '\n';
	//for(int i = 0; i <= 2 * N; i++) cerr << failure[i] << ' '; cerr << '\n';

	for(int i = 0; i < 2 * N; i++) {
		target[2 * N + i] = target[i] = vals[2 * N - 1 - i];
	}

	if(kmp_search(vals, failure, 2 * N, target, 2 * N * 2) != -1) {
		return 2 * N / (2 * N - failure[2 * N]);
	} else {
		return 0;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	
	int T; cin >> T;
	for(; T > 0; T--) {
		cin >> N;
		for(int i = 0; i < N; i++) cin >> X[i] >> Y[i];

		cout << solve() << '\n';
	}
	return 0;
}
