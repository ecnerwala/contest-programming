#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;
const i64 INF = 1e18;

const int S = 1 << 18;

const int L = 6;
struct mat {
	i64 v[L][L];
	mat() {}
	static mat I() {
		mat r;
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				r.v[i][j] = (i==j ? 0 : INF);
			}
		}
		return r;
	}
	friend mat operator * (const mat& a, const mat& b) {
		mat r;
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				r.v[i][j] = INF;
				for (int k = 0; k < L; k++) {
					r.v[i][j] = min(r.v[i][j], a.v[i][k] + b.v[k][j]);
				}
			}
		}
		return r;
	}
};

mat seg[2*S];

mat query(int l, int r) {
	mat lans = mat::I();
	mat rans = mat::I();
	for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
		if (a&1) lans = lans * seg[a++];
		if (b&1) rans = seg[--b] * rans;
	}
	return lans * rans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, Q; cin >> N >> Q;
	vector<int> H(N+2);
	vector<int> W(N+2);
	for (int i = 1; i <= N; i++) cin >> H[i];
	for (int i = 1; i <= N; i++) cin >> W[i];

	mat relax;
	for (int x = 0; x < L; x++) {
		for (int y = 0; y < L; y++) {
			relax.v[x][y] = INF;
		}
		relax.v[x][x] = 0;
		if (x < 2) {
			relax.v[x][2] = 0;
			relax.v[x][3] = 0;
		}
		if (x < 4) {
			relax.v[x][4] = 0;
			relax.v[x][5] = 0;
		}
	}

	vector<int> Wtop = W;
	for (int i = 0; i <= N; i++) {
		int h = min(H[i], H[i+1]);
		Wtop[i] += H[i] - h;
		Wtop[i+1] += H[i+1] - h;
		mat& trans = seg[S+2*i+1];
		for (int x = 0; x < L; x++) {
			for (int y = 0; y < L; y++) {
				trans.v[x][y] = INF;
			}
			trans.v[x][x^1] = h;
			trans.v[x][x] = 2*h;
		}
		trans = trans * relax;
	}

	for (int i = 1; i <= N; i++) {
		mat& trans = seg[S+2*i];
		for (int x = 0; x < L; x++) {
			for (int y = 0; y < L; y++) {
				trans.v[x][y] = INF;
			}
			trans.v[x][x] = 0;
			if (x & 1) {
				trans.v[x][x] += Wtop[i];
			} else {
				trans.v[x][x] += 2 * Wtop[i];
			}
			if (bool(x & 2) ^ bool(x & 1)) {
				trans.v[x][x] += W[i];
			} else {
				trans.v[x][x] += 2 * W[i];
			}
		}
	}

	for (int i = S-1; i; i--) {
		seg[i] = seg[2*i] * seg[2*i+1];
	}

	while (Q--) {
		int l, r; cin >> l >> r;
		mat ans = query(2*l-1, 2*r+1);
		cout << ans.v[0][4] << '\n';
	}

	return 0;
}
