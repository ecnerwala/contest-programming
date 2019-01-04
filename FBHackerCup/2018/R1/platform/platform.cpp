#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 4e5;
int N;
int H[MAXN];

int U[MAXN];
int D[MAXN];

const int MAXM = 30;
int M;

bool is_good(double v) {
	assert(v >= 0);
	double mi = H[0] - v;
	double ma = H[0] + v;
	for (int i = 1; i < N; i++) {
		mi -= D[i];
		ma += U[i];

		mi = max(mi, H[i] - v);
		ma = min(ma, H[i] + v);

		if (mi > ma) return false;
	}
	return true;
}

double go() {
	double mi = 0;
	double ma = 1e9;
	for (int z = 0; z < 100; z++) {
		double md = (mi + ma) / 2;
		assert(mi <= md && md <= ma);
		if (is_good(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	assert(abs(ma - mi) < 1e-8);
	return ma;
}

int main() {
	int NUM_CASES; cin >> NUM_CASES;
	for (int case_num = 1; case_num <= NUM_CASES; case_num ++) {

		cin >> N >> M;
		assert(2 <= N && N < MAXN);
		assert(1 <= M && M < MAXM);

		ll W, X, Y, Z;
		cin >> H[0] >> H[1] >> W >> X >> Y >> Z;
		assert(0 <= W && W < Z);
		assert(0 <= X && X < Z);
		assert(0 <= Y && Y < Z);
		assert(1 <= Z && Z <= int(1e6));
		for (int i = 2; i < N; i++) {
			H[i] = int((W * ll(H[i-2]) + X * ll(H[i-1]) + Y) % Z);
		}
		for (int i = 0; i < N; i++) {
			assert(0 <= H[i] && H[i] < Z);
		}

		for (int i = 0; i < N; i++) {
			U[i] = D[i] = int(Z)+1;
		}

		for (int z = 0; z < M; z++) {
			int a, b, u, d; cin >> a >> b >> u >> d;
			a--, b--;
			assert(0 <= a && a < N);
			assert(0 <= b && b < N);
			assert(a != b);
			assert(0 <= u);
			assert(0 <= d);
			if (b < a) {
				swap(a,b);
				swap(u,d);
			}
			assert(a < b);

			for (int j = a+1; j <= b; j++) {
				U[j] = min(U[j], u);
				D[j] = min(D[j], d);
			}
		}

		double res = go();
		cout << "Case #" << case_num << ": " << fixed << setprecision(9) << res << '\n';
	}

	return 0;
}
