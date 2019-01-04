#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const ll INF = ll(1e9) + 7;

const int MAXN = 200;
int N;
int M;
ll D[MAXN][MAXN];
ll P;

ld go() {
	cin >> N >> M >> P;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			D[i][j] = INF;
		}
	}
	for (int i = 0; i < M; i++) {
		int u, v; ll d; cin >> u >> v >> d;
		u--, v--;
		D[u][v] = min(D[u][v], d);
		D[v][u] = min(D[v][u], d);
	}
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
			}
		}
	}

	ld r0 = 0;
	for (int j = 1; j < N; j++) {
		r0 += D[0][j];
	}
	r0 /= N-1;
	ld r1 = 0;
	for (int i = 1; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			r1 += D[i][j];
		}
	}
	r1 /= N-1;
	r1 /= N-1;

	ld res = 0;
	res += (ld(N-1) + pow(-1./(N-1), P-1)) / N;
	res *= (r0-r1) / N * (N-1);
	res += ((r0 - r1) / N + r1) * P;
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		ld res = go();
		cout << "Case #" << case_num << ": " << fixed << setprecision(10) << res << '\n';
	}

	return 0;
}
