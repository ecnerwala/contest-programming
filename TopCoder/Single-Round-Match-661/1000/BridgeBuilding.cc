#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class BridgeBuilding {

public:

	static const int INF = 1e9;

	static const int MAXN = 300;
	static const int MAXK = MAXN;
	int N;
	int K;
	int A[MAXN][2];
	int P[MAXN][2]; // coordinates

	int distl[MAXN][MAXN]; // max distance from l..r to l
	int distr[MAXN][MAXN]; // max distance from l..r to r
	int dist[MAXN][MAXN]; // max distance from l..r to r

	int dp[MAXN][MAXK]; // min possible max distance

	bool is_good(int dia) {
		for(int i = 0; i < N; i++) {
			for(int k = 0; k <= K; k++) {
				dp[i][k] = INF;
			}
		}

		assert(K >= 1);
		for(int i = 0; i < N; i++) {
			if(P[i][0] + P[i][1] <= dia) {
				dp[i][1] = max(P[i][0], P[i][1]);
			}

			for(int k = 1; k < K; k++) {
				int v = INF;
				for(int j = 0; j < i; j++) {
					if(dist[j][i] <= dia && dp[j][k] + distl[j][i] <= dia && distr[j][i] <= dia) {
						int cnd = max(dp[j][k] + min(P[i][0] - P[j][0], P[i][1] - P[j][1]), distr[j][i]);
						if(cnd < v) {
							v = cnd;
						}
					}
				}
				dp[i][k + 1] = v;
				//if(v < INF) cerr << dia << ' ' << bj << ' ' << i << ' ' << k + 1 << ' ' << v << '\n';
			}

			for(int k = 1; k <= K; k++) {
				if(dp[i][k] == INF) continue;
				if(dp[i][k] + max(P[N - 1][0] - P[i][0], P[N - 1][1] - P[i][1]) <= dia) return true;
			}

		}

		return false;
	}

	int minDiameter(vector <int> a, vector <int> b, int k) {
		N = a.size() + 1;
		K = k;
		for(int i = 0; i < N - 1; i++) {
			A[i][0] = a[i];
			A[i][1] = b[i];
		}

		for(int t = 0; t < 2; t++) {
			for(int i = 0; i + 1 < N; i++) {
				P[i + 1][t] = P[i][t] + A[i][t];
			}
		}

		for(int i = 0; i < N; i++) {
			for(int j = i + 1; j < N; j++) {
				distl[i][j] = distr[i][j] = 0;
				for(int k = i; k <= j; k++) {
					for(int t = 0; t < 2; t++) {
						distl[i][j] = max(distl[i][j], 
								min(
									P[k][t] - P[i][t], 
									P[j][t] - P[k][t] + P[j][!t] - P[i][!t]));
						distr[i][j] = max(distr[i][j], 
								min(
									P[j][t] - P[k][t], 
									P[k][t] - P[i][t] + P[j][!t] - P[i][!t]));
					}
				}
				dist[i][j] = 0;
				int tot = P[j][1] - P[i][1] + P[j][0] - P[i][0];
				vector<int> lens;
				for(int k = i; k < j; k++) {
					lens.push_back(A[k][0]);
				}
				for(int k = j - 1; k >= i; k--) {
					lens.push_back(A[k][1]);
				}
				for(int k = 0, l = 0, d = 0; k < int(lens.size()); ) {
					assert(d >= 0);
					dist[i][j] = max(dist[i][j], min(d, tot - d));
					if(d <= tot - d) {
						d += lens[k];
						k++;
					} else {
						assert(d > 0);
						assert(k > l);
						d -= lens[l];
						l++;
						assert(l <= k);
					}
				}
				//cerr << i << ' ' << j << ' ' << distl[i][j] << ' ' << distr[i][j] << ' ' << dist[i][j] << '\n';
			}
		}
		int mi = -1;
		int ma = 2e4;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if(is_good(md)) ma = md;
			else mi = md;
		}

		is_good(ma);

		return ma;
	}
};

// vim:ft=cpp
