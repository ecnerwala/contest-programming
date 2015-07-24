#include<bits/stdc++.h>
using namespace std;

const double PI2 = atan2(1, 0);
const double TAU = PI2 * 4;

const double EPS = 1e-9;

const double INF = 1e18;

typedef complex<double> cd;


const int MAXT = 1e5;
int D;
int L;

cd rt[2][MAXT];
int rev[MAXT];

void fft(cd A[MAXT], bool inv = false) {
	for(int i = 0; i < L; i++) {
		if(rev[i] < i) {
			swap(A[rev[i]], A[i]);
		}
	}
	// w^n = 1
	for(int n = 2; n <= L; n <<= 1) {
		for(int i = 0; i < L; i += n) {
			for(int j = i, v = 0; j < i + n / 2; j++, v += L / n) {
				cd e = A[j], o = A[j + n / 2] * rt[inv][v];
				A[j] = e + o;
				A[j + n / 2] = e - o;
			}
		}
	}
	if(inv) {
		for(int i = 0; i < L; i++) {
			A[i] /= L;
		}
	}
}

const int MAXN = 70;
const int MAXM = 150;

int N, M;
int T;
int X;

int A[MAXM];
int B[MAXM];
int C[MAXM];
cd P[MAXM][MAXT];

double dp[MAXN][MAXT];
int vis[MAXN];

cd tmp[MAXT];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> T >> X;
	L = 1, D = 0;
	while(L <= T * 3) L <<= 1, D++;
	for(int i = 0; i < L; i++) {
		for(int t = 0, v = i; t < D; t++, v /= 2) {
			rev[i] <<= 1;
			rev[i] += v & 1;
		}
	}

	for(int i = 0; i < L; i++) {
		rt[0][i] = polar(1., TAU / L * i);
		rt[1][i] = polar(1., -TAU / L * i);
	}

	for(int i = 0; i < N; i++) {
		for(int t = 0; t < L; t++) {
			dp[i][t] = INF;
		}
	}
	for(int i = 0; i < L; i++) {
		dp[N - 1][i] = (i <= T) ? 0 : X;
	}
	vis[N - 1] = 1;

	for(int i = 0; i < M; i++) {
		cin >> A[i] >> B[i] >> C[i];
		A[i] --,  B[i] --;
		for(int t = 1; t <= T; t++) {
			double v; cin >> v;
			v *= 1e-5;
			P[i][t] = v;
		}
		//for(int t = 0; t <= T; t++) cerr << P[i][t] << ' '; cerr << '\n';
		fft(P[i]);
	}

	for(int t = 0; t < N; t++) {
		bool changed = false;
		for(int e = 0; e < M; e++) {
			int a = A[e], b = B[e], c = C[e];
			if(!vis[b]) continue;
			if(vis[b] <= (t - 1) * M + e) continue;
			memset(tmp, 0, sizeof(tmp));
			for(int i = 0; i <= T + 1; i++) {
				tmp[2 * T + 1 - i] = dp[b][i];
			}
			for(int i = T + 1; i <= 2 * T + 1; i++) {
				tmp[2 * T + 1 - i] = dp[b][T + 1];
			}
			fft(tmp);
			for(int i = 0; i < L; i++) {
				tmp[i] *= P[e][i];
			}
			fft(tmp, true);
			for(int i = 0; i <= T + 1; i++) {
				double v = tmp[2 * T + 1 - i].real() + c;
				if(v < dp[a][i] - EPS) {
					dp[a][i] = v;
					changed = true;
					vis[a] = t * M + e + 1;
				}
			}
			if(vis[a] <= t * M + e) continue;
			for(int i = T + 1; i > 0; i--) {
				dp[a][i - 1] = min(dp[a][i], dp[a][i - 1]);
			}
		}
		if(!changed) break;
	}
	assert(vis[0]);

	cout << fixed << setprecision(20) << dp[0][0] << '\n';

	return 0;
}
