#include <bits/stdc++.h>
using namespace std;

double const PI = 2 * atan2(1, 0);

// array has length len
// start at st, step of size stp
// root of unity is rt
template <class It, class T> void fft(unsigned int len, T rt, It st, unsigned int stp = 1) {
	if(len == 1) return;
	len >>= 1;
	fft(len, rt * rt, st, stp + stp);
	fft(len, rt * rt, st + stp, stp + stp);

	T v = 1;
	for(unsigned int i = 0; i < len; i++, v *= rt) {
		T a = st[i * stp], b = st[(i + len) * stp];
		st[i * stp] = a + v * b;
		st[(i + len) * stp] = a - v * b;
	}

	cerr << len * 2 << '\n';
	for(int i = 0; i < len * 2; i++) {
		cerr << st[i * stp] << ' ';
	}
	cerr << '\n';
}

vector<double> convolve(vector<double> X, vector<double> Y) {

	unsigned int len = 1;
	while(len < X.size() + Y.size() - 1) len *= 2;

	vector<complex<double>> a(X.begin(), X.end()), b(Y.begin(), Y.end());

	a.resize(len);
	b.resize(len);

	complex<double> rt = polar(1., -2 * PI / len);

	fft(len, rt, a.begin()), fft(len, rt, b.begin());
	for(unsigned int i = 0; i < len; i++) {
		a[i] *= b[i];
	}
	fft(len, -rt, a.begin());

	vector<double> c(X.size() + Y.size() - 1);
	for(unsigned int i = 0; i < c.size(); i++) {
		cerr << a[i].imag() << ' ' << a[i].real() << '\n';
		c[i] = a[i].real() / len;
	}
	vector<double> s(X.size() + Y.size() - 1);
	for(int x = 0; x < int(X.size()); x++) {
		for(int y = 0; y < int(Y.size()); y++) {
			s[x + y] += X[x] * Y[y];
		}
	}

	for(int i = 0; i < int(s.size()); i++) {
		cerr << c[i] << ' ';
	} cerr << '\n';
	for(int i = 0; i < int(s.size()); i++) {
		cerr << s[i] << ' ';
	} cerr << '\n';

	assert(false);
	return c;
}

typedef unsigned char num;

const int MAXN = 1500;
const int MAXM = MAXN * 20;
int N;
int M;

int A[MAXN][MAXN];
int B[MAXN][MAXN];

long long prefA[MAXN][MAXN];
long long prefB[MAXN][MAXN];

int main() {
	vector<complex<double>> v(4);
	v[0] = 1;
	fft(4, complex<double>(0, -1), v.begin());
	for(int i = 0; i < 2; i++) {
		cerr << v[i] << '\n';
	}

	return 0;

	/*
	cin >> N;
	M = N + (N / 2) + (N / 2);

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			cin >> A[i][j];
			A[i][j] -= 128;
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			cin >> B[i][j];
			B[i][j] -= 128;
		}
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			prefA[i + 1][j + 1] = prefA[i][j + 1] + prefA[i + 1][j] - prefA[i][j] + A[i][j] * A[i][j];
			prefB[i + 1][j + 1] = prefB[i][j + 1] + prefB[i + 1][j] - prefB[i][j] + B[i][j] * B[i][j];
		}
	}

	vector<double> a, b;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N / 2; j++) {
			a.push_back(0);
			b.push_back(0);
		}
		for(int j = 0; j < N; j++) {
			a.push_back(A[i][j]);
			b.push_back(B[i][j]);
		}
		for(int j = 0; j < N / 2; j++) {
			a.push_back(0);
			b.push_back(0);
		}
	}
	reverse(b.begin(), b.end());

	vector<double> conv = convolve(a, b);
	
	double res = 1e99;

	int L = M * N;

	assert(int(a.size()) == L);
	assert(int(b.size()) == L);

	for(int x = -(N / 2); x <= N / 2; x++) {
		for(int y = -(N / 2); y <= N / 2; y++) {
			double v = - 2 * conv[L - 1 - (M * x + y)];

			int xmi = max(0, -x);
			int xma = min(0, -x) + N;
			int ymi = max(0, -y);
			int yma = min(0, -y) + N;

			v += double(prefA[xma][yma] + prefA[xmi][ymi] - prefA[xmi][yma] - prefA[xma][ymi]);

			xmi += x;
			xma += x;
			ymi += y;
			yma += y;

			v += double(prefB[xma][yma] + prefB[xmi][ymi] - prefB[xmi][yma] - prefB[xma][ymi]);

			v /= (xma - xmi);
			v /= (yma - ymi);

			res = min(res, v);
		}
	}

	cout << fixed << setprecision(10) << res << '\n';
	*/
}
