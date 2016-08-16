#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN = 100;
const int MAXM = 100;
const int MAXK = 100;

int N, M, K;
ld P;
ld V[MAXN];
ld W[MAXK][MAXM];
int D[MAXN][MAXM];

ld val[MAXN];

inline ld get_val(int n, int k) {
	ld res = 0;
	for(int i = 0; i < M; i++) {
		res += val[D[n][i]] * W[k][i];
	}
	res *= P;
	res += V[n];
	return res;
}

int strat[MAXN];

void stabilize() {
	for(int z = 0; z < 100; z++) {
		for(int n = 0; n < N; n++) {
			val[n] = get_val(n, strat[n]);
		}
	}
}

ld go() {
	for(int i = 0; i < N; i++) {
		strat[i] = rand() % K;
	}
	memset(val, 0, sizeof(val));
	stabilize();
	for(int z = 0; z < 2500; z++) {
		int i = rand() % N;
		for(int k = 0; k < K; k++) {
			ld v = get_val(i, k);
			if(v > val[i]) {
				strat[i] = k;
				val[i] = v;
			}
		}
		if(z % 100 == 0) stabilize();
	}
	stabilize();
	return val[0] - V[0];
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K >> P;
	P /= 1e9;
	for(int i = 0; i < N; i++) {
		cin >> V[i];
	}
	for(int i = 0; i < K; i++) {
		for(int j = 0; j < M; j++) {
			cin >> W[i][j];
			W[i][j] /= 1e3;
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			cin >> D[i][j];
			D[i][j] --;
		}
	}
	ld res = 0;
	for(int i = 0; i < 10; i++) {
		res = max(res, go());
	}
	cout << fixed << setprecision(10) << res << '\n';
	return 0;
}
